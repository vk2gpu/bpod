import os
import sys
import struct
import argparse
from PIL import Image

def camel_case(s):
    from re import sub
    if s.find(' ') == -1 and s.find('_') == -1 and s.find('-') == -1:
        return s[0].upper() + s[1:]
    s = sub(r"(_|-)+", " ", s).title().replace(" ", "")
    s = s.replace('Bsides', 'BSides')
    return s


def encoding_none(args):
    with open(args.in_music, 'rb') as handle:
        music_data = handle.read()

    with open(args.out_hpp, 'wt') as handle:
        global indent
        indent = 0
        def tab():
            global indent
            indent += 4
        def shift_tab():
            global indent
            indent -= 4
        def out(line):
            global indent
            handle.write((' ' * indent) + line + '\n')
        def nl():
            handle.write('\n')
        out('#pragma once')
        nl()
        out('#include <stdint.h>')
        out('#include <stddef.h>')
        nl()
        out('class {}'.format(args.class_name))
        out('{')
        tab()
    
        out('public:')
        tab()
        out('const static size_t data_size = {};'.format(len(music_data)))
        out('inline static const char *data() {')
        tab()
        out('const static char data[{}] = {}'.format(len(music_data), '{'))
        tab()
        i = 0
        while i < len(music_data):
            line = ', '.join(['0x{:02x}'.format(value) for value in music_data[i: i + 16]])
            if (i + 16) < len(music_data):
                line += ','
            out(line)
            i += 16
        shift_tab()
        out('};')
        out('return reinterpret_cast<const char*>(&data[0]);')
        shift_tab()
        out('};')
        shift_tab()
        nl()

        shift_tab()
        out('};')


def main():
    parser = argparse.ArgumentParser(description='Encode music for the bsides badge as source files')
    parser.add_argument('--in-music', required=True, type=str, help='Input music file')
    parser.add_argument('--name', default=None, type=str, help='Music name')
    parser.add_argument('--out-hpp', required=True, type=str, help='Output C++ header file')
    args = parser.parse_args()

    if not os.path.exists(os.path.dirname(args.out_hpp)):
        try:
            os.makedirs(os.path.dirname(args.out_hpp))
        except:
            pass

    if not args.name:
        args.name = os.path.basename(args.out_hpp).split('.')[0].replace(' ', '_').replace('-', '_')

    args.class_name = camel_case(args.name)

    encoding_none(args)


if __name__ == '__main__':
    main()
