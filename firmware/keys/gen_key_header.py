import os
import sys
import binascii
import argparse

def main():
    parser = argparse.ArgumentParser(description='Encode images for the bsides badge as source files')
    parser.add_argument('--in-key-hex', required=True, type=str, help='Input key file containing hex key')
    parser.add_argument('--name', default=None, type=str, help='#define name')
    parser.add_argument('--out-h', required=True, type=str, help='Output C header file')
    args = parser.parse_args()

    if not os.path.exists(os.path.dirname(args.out_h)):
        try:
            os.makedirs(os.path.dirname(args.out_h))
        except:
            pass

    with open(args.in_key_hex, 'rt') as handle:
        key = handle.read().strip()
    key = key.strip()
    assert len(key) == 32
    key = binascii.a2b_hex(key)
    assert len(key) == 16

    with open(args.out_h, 'wt') as handle:
        def out(line):
            handle.write(line + '\n')
        def nl():
            handle.write('\n')
        out('#ifndef _H_GEN_{}_H_'.format(args.name))
        out('#define _H_GEN_{}_H_'.format(args.name))
        nl()
        out('#define {}_SIZE ({})'.format(args.name, len(key)))
        out('#define {} "{}"'.format(args.name, ''.join('\\x{:02x}'.format(v) for v in key)))
        nl()
        out('#endif')

if __name__ == '__main__':
    main()
