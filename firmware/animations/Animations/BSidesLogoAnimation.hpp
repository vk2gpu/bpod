#pragma once

#include <Adafruit_GFX.h>
#include <Images/BSidesLogo.hpp>

class BSidesLogoAnimation
{
    public:
        static void run(Adafruit_GFX &gfx) {
            // FUTURE: calculate these values (rather than assume ST7735 screen)
            BSidesLogo::draw(0, 0, gfx);
            uint16_t color = 0xf800;
            for ( size_t i = 0; i < 200; i++)
            {
                gfx.drawLine(50, 41, 60, 59, color);
                gfx.drawLine(51, 40, 60, 59, color);
                gfx.drawLine(52, 41, 60, 59, color);
                gfx.drawLine(58, 40, 66, 61, color);
                gfx.drawLine(59, 39, 66, 61, color);
                gfx.drawLine(60, 40, 66, 61, color);
                color = ( color >> 1 ) | (( color & 0x1) << 15);
                delay(25);
            }
        };
};
