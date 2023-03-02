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
    img = Image.open(args.in_image).convert('RGB')
    pixels = list(img.getdata())
    width, height = img.size
    data = b''
    for pixel in pixels:
        r = (pixel[0] >> 3) & 0x1F
        g = (pixel[1] >> 2) & 0x3F
        b = (pixel[2] >> 3) & 0x1F
        pixel565 = (r << 11) + (g << 5) + b
        data += struct.pack('<H', pixel565)
    assert len(data) == (width * height * 2)

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
        out('#include <Adafruit_GFX.h>')
        nl()
        out('class {}'.format(args.class_name))
        out('{')
        tab()
    
        out('public:')
        tab()
        out('const static int16_t width = {};'.format(width))
        out('const static int16_t height = {};'.format(height))
        out('static void draw(int16_t x, int16_t y, Adafruit_GFX &gfx) {')
        tab()
        out('size_t offset = 0;')
        out('for ( int16_t dy = 0; dy < height; dy++) {')
        tab()
        out('for ( int16_t dx = 0; dx < width; dx++) {')
        tab()
        out('gfx.drawPixel(x + dx, y + dy, pixel(offset));')
        out('offset += sizeof(uint16_t);')
        shift_tab()
        out('}')
        shift_tab()
        out('}')
        shift_tab()
        out('};')
        out('inline static uint16_t pixel(size_t offset) {')
        tab()
        out('const static char data[{}] = {}'.format(width * height * 2, '{'))
        tab()
        i = 0
        while i < len(data):
            line = ', '.join(['0x{:02x}'.format(value) for value in data[i: i + 16]])
            if (i + 16) < len(data):
                line += ','
            out(line)
            i += 16
        shift_tab()
        out('};')
        out('return *reinterpret_cast<const uint16_t*>(&data[offset]);')
        shift_tab()
        out('};')
        shift_tab()
        nl()

        shift_tab()
        out('};')


def encoding_fast_hline(args):
    img = Image.open(args.in_image).convert('RGB')
    pixels = list(img.getdata())
    width, height = img.size
    assert width == 1
    data = b''
    for pixel in pixels:
        r = (pixel[0] >> 3) & 0x1F
        g = (pixel[1] >> 2) & 0x3F
        b = (pixel[2] >> 3) & 0x1F
        pixel565 = (r << 11) + (g << 5) + b
        data += struct.pack('<H', pixel565)
    assert len(data) == (height * 2)

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
        out('#include <Adafruit_GFX.h>')
        nl()
        out('class {}'.format(args.class_name))
        out('{')
        tab()

        out('public:')
        tab()
        out('const static int16_t height = {};'.format(height))
        out('static void draw(int16_t x, int16_t y, int16_t w, Adafruit_GFX &gfx) {')
        tab()
        out('size_t offset = 0;')
        out('for ( int16_t dy = 0; dy < height; dy++) {')
        tab()
        out('gfx.drawFastHLine(x, y + dy, w, pixel(offset));')
        out('offset += sizeof(uint16_t);')
        shift_tab()
        out('}')
        shift_tab()
        out('};')
        out('inline static uint16_t pixel(size_t offset) {')
        tab()
        out('const static char data[{}] = {}'.format(width * height * 2, '{'))
        tab()
        i = 0
        while i < len(data):
            line = ', '.join(['0x{:02x}'.format(value) for value in data[i: i + 16]])
            if (i + 16) < len(data):
                line += ','
            out(line)
            i += 16
        shift_tab()
        out('};')
        out('return *reinterpret_cast<const uint16_t*>(&data[offset]);')
        shift_tab()
        out('};')
        shift_tab()
        nl()

        shift_tab()
        out('};')


def encoding_fast_vline(args):
    img = Image.open(args.in_image).convert('RGB')
    pixels = list(img.getdata())
    width, height = img.size
    assert height == 1
    data = b''
    for pixel in pixels:
        r = (pixel[0] >> 3) & 0x1F
        g = (pixel[1] >> 2) & 0x3F
        b = (pixel[2] >> 3) & 0x1F
        pixel565 = (r << 11) + (g << 5) + b
        data += struct.pack('<H', pixel565)
    assert len(data) == (width * 2)

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
        out('#include <Adafruit_GFX.h>')
        nl()
        out('class {}'.format(args.class_name))
        out('{')
        tab()

        out('public:')
        tab()
        out('const static int16_t width = {};'.format(width))
        out('static void draw(int16_t x, int16_t y, int16_t h, Adafruit_GFX &gfx) {')
        tab()
        out('size_t offset = 0;')
        out('for ( int16_t dx = 0; dx < width; dx++) {')
        tab()
        out('gfx.drawFastVLine(x + dx, y, h, pixel(offset));')
        out('offset += sizeof(uint16_t);')
        shift_tab()
        out('}')
        shift_tab()
        out('};')
        out('inline static uint16_t pixel(size_t offset) {')
        tab()
        out('const static char data[{}] = {}'.format(width * height * 2, '{'))
        tab()
        i = 0
        while i < len(data):
            line = ', '.join(['0x{:02x}'.format(value) for value in data[i: i + 16]])
            if (i + 16) < len(data):
                line += ','
            out(line)
            i += 16
        shift_tab()
        out('};')
        out('return *reinterpret_cast<const uint16_t*>(&data[offset]);')
        shift_tab()
        out('};')
        shift_tab()
        nl()

        shift_tab()
        out('};')


def encoding_vpattern(args):
    img = Image.open(args.in_image).convert('RGB')
    pixels = list(img.getdata())
    width, height = img.size
    data = b''
    for pixel in pixels:
        r = (pixel[0] >> 3) & 0x1F
        g = (pixel[1] >> 2) & 0x3F
        b = (pixel[2] >> 3) & 0x1F
        pixel565 = (r << 11) + (g << 5) + b
        data += struct.pack('<H', pixel565)
    assert len(data) == (width * height * 2)

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
        out('#include <Adafruit_GFX.h>')
        nl()
        out('class {}'.format(args.class_name))
        out('{')
        tab()

        out('public:')
        tab()
        out('const static int16_t width = {};'.format(width))
        out('static void draw(int16_t x, int16_t y, int16_t h, Adafruit_GFX &gfx) {')
        tab()
        out('size_t offset = y * width * sizeof(uint16_t);')
        out('for ( int16_t dy = 0; dy < h; dy++) {')
        tab()
        out('for ( int16_t dx = 0; dx < width; dx++) {')
        tab()
        out('if ( offset >= {}) offset = 0;'.format(width * height * 2))
        out('gfx.drawPixel(x + dx, y + dy, pixel(offset));')
        out('offset += sizeof(uint16_t);')
        shift_tab()
        out('}')
        shift_tab()
        out('}')
        shift_tab()
        out('};')
        out('inline static uint16_t pixel(size_t offset) {')
        tab()
        out('const static char data[{}] = {}'.format(width * height * 2, '{'))
        tab()
        i = 0
        while i < len(data):
            line = ', '.join(['0x{:02x}'.format(value) for value in data[i: i + 16]])
            if (i + 16) < len(data):
                line += ','
            out(line)
            i += 16
        shift_tab()
        out('};')
        out('return *reinterpret_cast<const uint16_t*>(&data[offset]);')
        shift_tab()
        out('};')
        shift_tab()
        nl()

        shift_tab()
        out('};')


def main():
    parser = argparse.ArgumentParser(description='Encode images for the bsides badge as source files')
    parser.add_argument('--in-image', required=True, type=str, help='Input image file')
    parser.add_argument('--name', default=None, type=str, help='Image name')
    parser.add_argument('--out-hpp', required=True, type=str, help='Output C++ header file')
    args = parser.parse_args()

    if not args.name:
        args.name = os.path.basename(args.out_hpp).split('.')[0].replace(' ', '_').replace('-', '_')

    args.class_name = camel_case(args.name)



    img = Image.open(args.in_image).convert('RGB')
    width, height = img.size

    if width == 1:
        encoding_fast_hline(args)
    elif height == 1:
        encoding_fast_vline(args)
    elif args.in_image.find('scroll-fill') != -1:
        encoding_vpattern(args)
    else:
        encoding_none(args)


if __name__ == '__main__':
    main()
