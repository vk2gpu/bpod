#pragma once

#include <Adafruit_GFX.h>

class BpodMenuArrow
{
    public:
        const static int16_t width = 8;
        const static int16_t height = 11;
        static void draw(int16_t x, int16_t y, Adafruit_GFX &gfx) {
            size_t offset = 0;
            for ( int16_t dy = 0; dy < height; dy++) {
                for ( int16_t dx = 0; dx < width; dx++) {
                    gfx.drawPixel(x + dx, y + dy, pixel(offset));
                    offset += sizeof(uint16_t);
                }
            }
        };
        inline static uint16_t pixel(size_t offset) {
            const static char data[176] = {
                0x9d, 0x5d, 0x9e, 0xa6, 0xbd, 0x65, 0x5c, 0x45, 0x5c, 0x45, 0x5c, 0x45, 0x5c, 0x45, 0x5c, 0x45,
                0x5d, 0x96, 0xdf, 0xf7, 0x3e, 0xcf, 0x9d, 0x65, 0x3c, 0x45, 0x3c, 0x45, 0x3c, 0x45, 0x3c, 0x45,
                0x3c, 0x4d, 0xfe, 0xbe, 0xff, 0xff, 0x7f, 0xdf, 0x7c, 0x5d, 0x1c, 0x45, 0x1c, 0x45, 0x1c, 0x45,
                0x1b, 0x45, 0x5b, 0x55, 0xde, 0xb6, 0xff, 0xff, 0x3e, 0xd7, 0x9c, 0x65, 0x1b, 0x45, 0x1b, 0x45,
                0xfb, 0x44, 0xfb, 0x44, 0x5b, 0x55, 0xdd, 0xb6, 0xff, 0xff, 0x3e, 0xd7, 0x5b, 0x5d, 0xfb, 0x44,
                0xda, 0x44, 0xda, 0x44, 0xda, 0x44, 0xfb, 0x4c, 0x3e, 0xcf, 0xff, 0xff, 0x3e, 0xcf, 0xfb, 0x4c,
                0xda, 0x44, 0xda, 0x44, 0xfa, 0x4c, 0x5d, 0x9e, 0xdf, 0xf7, 0x5e, 0xdf, 0x9b, 0x6d, 0xda, 0x44,
                0xb9, 0x44, 0xda, 0x4c, 0x7d, 0xa6, 0xdf, 0xff, 0x5e, 0xdf, 0x7b, 0x6d, 0xb9, 0x44, 0xb9, 0x44,
                0xb9, 0x44, 0x3c, 0x9e, 0xdf, 0xf7, 0x9f, 0xe7, 0x7b, 0x75, 0x99, 0x44, 0x99, 0x3c, 0x99, 0x3c,
                0xdb, 0x85, 0xdf, 0xf7, 0x3e, 0xd7, 0x5a, 0x6d, 0x79, 0x3c, 0x79, 0x3c, 0x79, 0x3c, 0x79, 0x3c,
                0xd9, 0x54, 0x9d, 0xb6, 0x3a, 0x6d, 0x78, 0x3c, 0x78, 0x3c, 0x78, 0x3c, 0x78, 0x3c, 0x78, 0x3c
            };
            return *reinterpret_cast<const uint16_t*>(&data[offset]);
        };

};