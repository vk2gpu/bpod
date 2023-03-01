#pragma once

#include <Adafruit_GFX.h>

class BpodMenuBar
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
                0xbe, 0x4d, 0x9e, 0x4d, 0x9e, 0x4d, 0x5d, 0x45, 0x5c, 0x45, 0x3c, 0x45, 0x1c, 0x45, 0x1b, 0x45,
                0xfb, 0x44, 0xda, 0x44, 0xda, 0x44, 0xb9, 0x44, 0x99, 0x3c, 0x79, 0x3c, 0x78, 0x3c, 0x58, 0x3c,
                0x37, 0x3c, 0x17, 0x3c, 0x16, 0x44, 0x9c, 0xbe
            };
            return *reinterpret_cast<const uint16_t*>(&data[offset]);
        };

};
