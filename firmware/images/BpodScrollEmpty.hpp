#pragma once

#include <Adafruit_GFX.h>

class BpodScrollEmpty
{
    public:
        const static int16_t width = 8;
        static void draw(int16_t x, int16_t y, int16_t h, Adafruit_GFX &gfx) {
            size_t offset = 0;
            for ( int16_t dx = 0; dx < width; dx++) {
                gfx.drawFastVLine(x + dx, y, h, pixel(offset));
                offset += sizeof(uint16_t);
            }
        };
        inline static uint16_t pixel(size_t offset) {
            const static char data[16] = {
                0xb6, 0xbd, 0x9a, 0xd6, 0x58, 0xce, 0x79, 0xce, 0xfb, 0xde, 0x3c, 0xe7, 0x9a, 0xd6, 0x34, 0xad
            };
            return *reinterpret_cast<const uint16_t*>(&data[offset]);
        };

};
