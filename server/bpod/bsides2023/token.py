import os
import sys
import zlib
import struct
import binascii
from Crypto.Cipher import AES

class Token(object):
    BLOCK_SIZE = 16
    SCORE_CODE_DEBUG = 0x11
    SCORE_CODE_SNAKE = 0x12
    SCORE_CODE_TETRIS = 0x13

    def __init__(self):
        self.device_id = 0
        self.game_code = 0
        self.score = 0
        self.random = False

    def randomise(self):
        self.device_id = struct.unpack('<I', os.urandom(4))[0]
        self.random = True

    @property
    def ipsn(self):
        model_enc = '3QU'
        if self.random:
            model_enc = '37S'
        value = self.device_id
        device_id_enc = ''
        for _ in range(0, 5):
            digit = value % 34
            value = int(value / 34)
            if digit < 10:
                device_id_enc = chr((ord('0') + digit)) + device_id_enc
            else:
                device_id_enc = chr((ord('A') + (digit - 10))) + device_id_enc
        assert len(device_id_enc) == 5
        assert len(model_enc) == 3
        return 'F7Z' + device_id_enc + model_enc

    @property
    def game(self):
        if self.game_code == self.SCORE_CODE_DEBUG:
            return 'debug'
        elif self.game_code == self.SCORE_CODE_SNAKE:
            return 'snake'
        elif self.game_code == self.SCORE_CODE_TETRIS:
            return 'tetris'
        else:
            return hex(self.game_code)

    def __str__(self):
        return '{}\n{} {} pts'.format(self.ipsn, self.game, self.score)

    @classmethod
    def decode_key(cls, key, key2):
        new_key = b''
        for i in range(0, len(key)):
            value = key[i]
            value ^= i
            value ^= 0xaa
            value ^= key2[i]
            new_key += struct.pack('<B', value)
        assert len(new_key) == len(key)
        return new_key

    @classmethod
    def decrypt_aes_128_cbc_no_iv_single_block(cls, key, data):
        assert len(data) == cls.BLOCK_SIZE
        assert len(key) == cls.BLOCK_SIZE
        iv = b'\x00' * 16
        aes = AES.new(key, AES.MODE_CBC, iv)
        return aes.decrypt(data)

    @classmethod
    def encrypt_aes_128_cbc_no_iv_single_block(cls, key, data):
        assert len(data) == cls.BLOCK_SIZE
        assert len(key) == cls.BLOCK_SIZE
        iv = b'\x00' * 16
        aes = AES.new(key, AES.MODE_CBC, iv)
        return aes.encrypt(data)

    @classmethod
    def crc32(cls, data):
        return ((zlib.crc32(data) % 2**32) + 1) & 0xffffffff

    @classmethod
    def fb64_encode_char(cls, prev, value):
        if 0 != (prev & 0x1):
            if 0 == (prev & 0x2):
                if value < 10:
                    return chr(ord('9') - value)
                elif value == 10:
                    return '-'
                elif value == 11:
                    return '_'
                elif value > 11:
                    return chr(ord('v') + (value - 11))
            else:
                if 0 == (prev & 0x4):
                    return chr(ord('a') + value)
                else:
                    return chr(ord('A') + value)
        else:
            if 0 == (prev & 0x4):
                return chr(ord('j') + value)
            else:
                return chr(ord('K') + value)

    @classmethod
    def fb64_encode(cls, data):
        out = ''
        assert len(data) == 16
        data = data + os.urandom(1)
        data = data + struct.pack('<I', cls.crc32(data))
        assert len(data) == 21
        prev = 0
        value = 0
        j = 0
        for i in range(0, 21):
            value = (data[i] >> 4) & 0xf
            out += cls.fb64_encode_char(prev, value)
            j += 1
            prev = value
            value = (data[i] >> 0) & 0xf
            out += cls.fb64_encode_char(prev, value)
            j += 1
            prev = value
        out += out[prev]
        out += '='
        assert len(out) == 44
        return out

    @classmethod
    def fb64_decode_char(cls, prev, letter):
        if 0 != (prev & 0x1):
            if 0 == (prev & 0x2):
                if ord(letter) >= ord('0') and ord(letter) <= ord('9'):
                    return ord('9') - ord(letter)
                elif letter == '-':
                    return 10
                elif letter == '_':
                    return 11
                else:
                    return (ord(letter) - ord('v')) + 11
            else:
                if 0 == (prev & 0x4):
                    return ord(letter) - ord('a')
                else:
                    return ord(letter) - ord('A')
        else:
            if 0 == (prev & 0x4):
                return ord(letter) - ord('j')
            else:
                return ord(letter) - ord('K')

    @classmethod
    def fb64_decode(cls, data):
        assert len(data) == 44
        data_tmp = data.replace(b'-', b'/').replace(b'_', b'+')
        assert len(binascii.a2b_base64(data_tmp)) == 32
        prev = 0
        out = b''
        for i in range(0, 42, 2):
            letter = chr(data[i])
            value = cls.fb64_decode_char(prev, letter)
            assert value >= 0
            assert value <= 0xf
            value_high = value
            prev = value
            letter = chr(data[i + 1])
            value = cls.fb64_decode_char(prev, letter)
            assert value >= 0, 'Letter "{}" ord={} decoded to {}'.format(letter, data[i + 1], value)
            assert value <= 0xf, 'Letter "{}" ord={} decoded to {}'.format(letter, data[i + 1], value)
            value_low = value
            prev = value
            value = ((value_high << 4) & 0xf0) | ((value_low << 0) & 0x0f)
            assert value >= 0
            assert value <= 0xff
            out += struct.pack('<B', value)
        assert len(out) == 21
        assert data[prev] == data[42], "{} == {}".format(chr(data[prev]), data[42])
        assert ord('=') == data[43], "{} == {}".format('=', data[43])
        expected = cls.crc32(out[:17])
        result = struct.unpack('<I', out[17: 21])[0]
        assert expected == result, "{} == {}".format(hex(expected), hex(result))
        return out[:16]

    @classmethod
    def decode(cls, key, key2, data):
        obj = cls()
        key = cls.decode_key(key, key2)
        if isinstance(data, str):
            data = data.encode('ascii')
        data = cls.fb64_decode(data)
        data = cls.decrypt_aes_128_cbc_no_iv_single_block(key, data)
        obj.game_code = struct.unpack('<B', data[0: 1])[0]
        obj.device_id = struct.unpack('<I', data[4: 8])[0]
        obj.score = struct.unpack('<I', data[8: 12])[0]
        assert obj.score % 0x80 == 0  # sent score is a multiple of 0x80
        obj.score = int(obj.score / 0x80) * 100
        expected = cls.crc32(data[:12] + data[:4])
        result = struct.unpack('<I', data[12: 16])[0]
        assert expected == result, "{} == {}".format(hex(expected), hex(result))
        assert obj.score % 100 == 0  # scores are a multiple of 100
        if obj.game == 'snake':
            assert obj.score <= 39500 # thanks to Alex for finding this bug
        elif obj.game == 'tetris':
            assert obj.score <= 57600 * 100 # (60 * 60 * 24 * 2) / 3 seconds per 100 points == 57600
        elif obj.game == 'debug':
            pass  # ignore for debug
        else:
            assert obj.score == 0
        return obj

    def encode(self, key, key2):
        key = self.decode_key(key, key2)
        v1, v2, v3 = struct.unpack('<BBB', os.urandom(3))
        data = struct.pack('<BBBBII', self.game_code, v1, v2, v3, self.device_id, int(self.score / 100) * 0x80)
        data += struct.pack('<I', self.crc32(data + data[:4]))
        assert len(data) == 16
        data = self.encrypt_aes_128_cbc_no_iv_single_block(key, data)
        text = self.fb64_encode(data)
        return text
