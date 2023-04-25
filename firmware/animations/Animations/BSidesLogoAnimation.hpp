#pragma once

#include <Adafruit_GFX.h>
#include <Images/BSidesLogo.hpp>

class BSidesLogoAnimation
{
    public:
        static void run(Adafruit_GFX &gfx) {
            uint8_t sta_mac[6];
            esp_efuse_mac_get_default(sta_mac);
            uint32_t devid = 0;
            devid |= (((uint32_t)sta_mac[5]) <<  0) & 0x000000ff;
            devid |= (((uint32_t)sta_mac[4]) <<  8) & 0x0000ff00;
            devid |= (((uint32_t)sta_mac[3]) << 16) & 0x00ff0000;
            devid |= (((uint32_t)sta_mac[2]) << 24) & 0xff000000;
            char ipsn[12];
            ipsn[0] = 'F';
            ipsn[1] = '7';
            ipsn[2] = 'Z';
            uint32_t value = devid;
            for (size_t i = 0; i < 5; i++ )
            {
                uint32_t digit = value % 34;
                value = value / 34;
                if ( digit < 10 )
                {
                    ipsn[3 + i] = '0' + digit;
                }
                else
                {
                    ipsn[3 + i] = 'A' + (digit - 10);
                }
            }
            ipsn[8] = '3';
            ipsn[9] = 'Q';
            ipsn[10] = 'U';
            ipsn[11] = '\0';
            // FUTURE: calculate these values (rather than assume ST7735 screen)
            gfx.setTextColor(0x4208, 0x0000);
            gfx.setTextSize(1);
            gfx.setCursor(30, gfx.height() - 12);
            gfx.print(ipsn);
            BSidesLogo::draw(0, 0, gfx);
            gfx.setTextColor(0x4208, 0x0000);
            gfx.setTextSize(1);
            gfx.setCursor(30, gfx.height() - 12);
            gfx.print(ipsn);
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
