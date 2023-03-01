#pragma once

#include <Adafruit_GFX.h>

class BpodTitleBar
{
    public:
        const static int16_t height = 20;
        static void draw(int16_t x, int16_t y, int16_t w, Adafruit_GFX &gfx) {
            size_t offset = 0;
            for ( int16_t dy = 0; dy < height; dy++) {
                gfx.drawFastHLine(x, y + dy, w, pixel(offset));
                offset += sizeof(uint16_t);
            }
        };
        inline static uint16_t pixel(size_t offset) {
            const static char data[40] = {
                0x5d, 0xe7, 0xdf, 0xf7, 0xdf, 0xf7, 0x9e, 0xf7, 0x9e, 0xf7, 0x7d, 0xef, 0x5d, 0xef, 0x3c, 0xe7,
                0x1c, 0xe7, 0xfb, 0xde, 0xdb, 0xd6, 0xba, 0xd6, 0x9a, 0xce, 0x79, 0xce, 0x59, 0xc6, 0x38, 0xc6,
                0x18, 0xbe, 0xf7, 0xb5, 0xf7, 0xb5, 0x96, 0xad
            };
            return *reinterpret_cast<const uint16_t*>(&data[offset]);
        };

};
