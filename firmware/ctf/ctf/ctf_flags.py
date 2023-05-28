import os
import sys
import json
import yaml
import struct
import string
import hashlib
import binascii
import argparse
import io
import shutil

DECODE_MACRO = """
#define CTF_FLAG_DECODE__(buf, a, b, size) \\
    for ( size_t __i = 0; __i < size; __i++ ) \\
    { \\
        buf[__i] = a[__i] + b[__i]; \\
    } \\
    buf[size] = '\\0';
#define CTF_FLAG_DECODE(buf, name)  CTF_FLAG_DECODE__(buf, name##_PART_A, name##_PART_B, name##_LENGTH)
"""

def main():
    parser = argparse.ArgumentParser(description='Encode flags for the bsides badge as source files')
    parser.add_argument('--in-manifest', required=True, nargs='+', help='Input MANIFEST.yaml files')
    parser.add_argument('--out-h', required=True, type=str, help='Output C header file')
    args = parser.parse_args()

    if not os.path.exists(os.path.dirname(args.out_h)):
        try:
            os.makedirs(os.path.dirname(args.out_h))
        except:
            pass

    args.in_manifest = [os.path.realpath(path) for path in args.in_manifest]
    for path in args.in_manifest:
        assert os.path.exists(path)

    with open(args.out_h, 'wt') as handle:
        def out(line):
            handle.write(line + '\n')
        def nl():
            handle.write('\n')
        out('#ifndef _H_CTF_FLAGS_GEN_H_')
        out('#define _H_CTF_FLAGS_GEN_H_')
        nl()
        max_buffer_size = 1
        for path in args.in_manifest:
            with open(path, 'rt') as manifest_handle:
                manifest = yaml.safe_load(manifest_handle)
            flag = manifest['challenge']['flags'][0]['content']
            name = manifest['challenge']['name']
            assert len(name.strip()) > 0
            assert flag.startswith('cybears{')
            assert flag.endswith('}')
            flag_pt = flag.encode('ascii')
            if (len(flag_pt) + 1) > max_buffer_size:
                max_buffer_size = len(flag_pt) + 1
            macro_prefix = 'CTF_FLAG_{}'.format(name.upper().replace(' ', '_'))
            out('#define {}_CSTR "{}"'.format(macro_prefix, flag))
            out('#define {}_LENGTH ((size_t){})'.format(macro_prefix, len(flag_pt)))
            flag_part_b = b''
            i = 0
            while len(flag_part_b) < len(flag_pt):
                flag_part_b += hashlib.md5(flag_pt + chr(ord('a') + i).encode('ascii')).digest()
                i += 1
            flag_part_b = flag_part_b[:len(flag_pt)]
            assert len(flag_part_b) == len(flag_pt)
            flag_part_a = b''
            for i in range(0, len(flag_pt)):
                value = (flag_pt[i] - flag_part_b[i]) & 0xff
                assert value >= 0
                assert value <= 0xff
                flag_part_a += struct.pack('<B', value)
            assert len(flag_part_a) == len(flag_part_b)
            flag_part_a_hex = ''.join(["\\x{:02x}".format(v) for v in flag_part_a])
            flag_part_b_hex = ''.join(["\\x{:02x}".format(v) for v in flag_part_b])
            assert flag_part_a_hex != flag_part_b_hex
            assert len(flag_part_a_hex) == len(flag_part_b_hex)
            out('#define {}_PART_A "{}"'.format(macro_prefix, flag_part_a_hex))
            out('#define {}_PART_B "{}"'.format(macro_prefix, flag_part_b_hex))
        out('#define CTF_FLAG_MAX_BUFFER_SIZE ((size_t){})'.format(max_buffer_size))
        out(DECODE_MACRO)
        nl()
        out('#endif')


if __name__ == '__main__':
    main()
