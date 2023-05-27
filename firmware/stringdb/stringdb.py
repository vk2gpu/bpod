import os
import sys
import json
import struct
import string
import binascii
import argparse
import io
import shutil
from Crypto.Cipher import AES


def obfuscate(data, offset, size):
    key = struct.pack('<I', offset) + struct.pack('<I', size) + (b'\x00' * 8)
    aes = AES.new(key, AES.MODE_CBC, key)
    mask = aes.encrypt(key)
    enc_data = b''
    for i in range(0, len(data)):
        value = data[i] ^ mask[i & 0xf]
        mask = mask[:i & 0xf] + struct.pack('<B', (mask[i & 0xf] + i) & 0xff) + mask[(i & 0xf) + 1:]
        enc_data += struct.pack('<B', value)
    assert len(data) == len(enc_data)
    return enc_data


def main():
    parser = argparse.ArgumentParser(description='Encode strings for the bsides badge as source files')
    parser.add_argument('--in-json', required=True, type=str, help='Input strings json file')
    parser.add_argument('--out-macro-h', required=True, type=str, help='Output C header file')
    parser.add_argument('--out-data-h', required=True, type=str, help='Output C header file')
    args = parser.parse_args()

    if not os.path.exists(os.path.dirname(args.out_macro_h)):
        try:
            os.makedirs(os.path.dirname(args.out_macro_h))
        except:
            pass

    if not os.path.exists(os.path.dirname(args.out_data_h)):
        try:
            os.makedirs(os.path.dirname(args.out_data_h))
        except:
            pass

    with open(args.in_json, 'rt') as handle:
        db = json.loads(handle.read())

    max_string_len = 0

    data = b''
    with open(args.out_macro_h, 'wt') as handle:
        def out(line):
            handle.write(line + '\n')
        def nl():
            handle.write('\n')
        out('#ifndef _H_STRINGDB_MACRO_GEN_H_')
        out('#define _H_STRINGDB_MACRO_GEN_H_')
        nl()
        for s in db:
            if s['encoding'] == 'ascii':
                s_data = s['data'].encode('ascii')
            elif s['encoding'] == 'hex':
                s_data = binascii.a2b_hex(s['data'].encode('ascii'))
            elif s['encoding'] == 'file_text':
                file_path = s['data']
                assert file_path.startswith('..')
                file_path = file_path.replace('/', os.sep).replace('\\', os.sep)
                file_path = os.path.realpath(os.path.join(os.path.dirname(__file__), file_path))
                with open(file_path, 'rt') as in1:
                    text = in1.read()
                while text.find('\r\n') != -1:
                    text = text.replace('\r\n', '\n')
                s_data = text.encode('ascii')
            else:
                raise NotImplementedError("encoding: {}".format(s['encoding']))
            out('#define SDB_START_{} ((size_t){})'.format(s['macro'], len(data)))
            out('#define SDB_END_{} ((size_t){})'.format(s['macro'], len(data) + len(s_data)))
            if len(s_data) > max_string_len:
                max_string_len = len(s_data)
            s_data = obfuscate(s_data, len(data), len(s_data))
            data += s_data
        nl()
        out('#endif')

    with open(args.out_data_h, 'wt') as handle:
        def out(line):
            handle.write(line + '\n')
        def nl():
            handle.write('\n')
        out('#ifndef _H_STRINGDB_DATA_GEN_H_')
        out('#define _H_STRINGDB_DATA_GEN_H_')
        nl()
        out('#define STRINGDB_MAX_STRING_LEN ((size_t){})'.format(max_string_len))
        nl()
        out('#define STRINGDB_DATA_SIZE ((size_t){})'.format(len(data)))
        out('static const char STRINGDB_DATA[STRINGDB_DATA_SIZE] = {')
        col = 0
        text = ''
        ascii_text = ''
        printable = bytes(string.printable, 'ascii')
        not_printable = bytes('\n\t\r\m\x0b', 'ascii')
        lines = list()
        for data_byte in data:
            if data_byte in printable and data_byte not in not_printable and data_byte < 0x7f and data_byte >= 0x20:
                ascii_text += chr(data_byte)
            else:
                ascii_text += '.'
            line = "'\\x{:02x}',".format(data_byte)
            col += 1
            if col == 16:
                col = 0
                line += '// {}\n'.format(ascii_text)
                ascii_text = ''
            lines.append(line)
        text += ''.join(lines)
        if not text.endswith('\n'):
            if len(ascii_text) != 0:
                text += '// {}\n'.format(ascii_text)
            else:
                text += '\n'
        text += '};\n'
        for line in text.split('\n'):
            out(line)
        nl()
        out('#endif')


if __name__ == '__main__':
    main()
