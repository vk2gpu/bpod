#pragma once

#include <Adafruit_GFX.h>
#include <Images/BpodLogo.hpp>

class BpodLoadAnimation
{
    public:
        static void run(Adafruit_GFX &gfx) {
            // FUTURE: calculate these values (rather than assume ST7735 screen)
            gfx.fillScreen(0x0000);
            BpodLogo::draw(21, 30, gfx);
            gfx.drawRoundRect(8, 110, 112, 12, 4, 0xffff);
            for ( int16_t w = 0; w < 111; w++)
            {
                gfx.fillRect(9, 111, w, 10, 0xffff);
                delay(25);
            }
        };
};
