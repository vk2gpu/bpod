import os
import sys
import json
import struct
import string
import binascii
import argparse
import io
import shutil
import subprocess


def main():
    parser = argparse.ArgumentParser(description='Encode githash for the bsides badge as source file')
    parser.add_argument('--out-h', required=True, type=str, help='Output C header file')
    args = parser.parse_args()

    if not os.path.exists(os.path.dirname(args.out_h)):
        try:
            os.makedirs(os.path.dirname(args.out_h))
        except:
            pass

    p = subprocess.Popen(['git', 'rev-parse', 'HEAD'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, _ = p.communicate()
    githash = b'\x00\x00\x00\x00'
    try:
        githash_hex = stdout.decode('ascii').strip()[:8]
        githash = binascii.a2b_hex(githash_hex)
    except:
        pass
    assert len(githash) == 4

    with open(args.out_h, 'wt') as handle:
        def out(line):
            handle.write(line + '\n')
        def nl():
            handle.write('\n')
        out('#ifndef _H_GITHASH_GEN_H_')
        out('#define _H_GITHASH_GEN_H_')
        nl()
        assert len(githash) == 4
        out('#define GITHASH_GEN_DATA "{}"'.format(''.join(['\\x{:02x}'.format(v) for v in githash])))
        nl()
        out('#endif')


if __name__ == '__main__':
    main()
