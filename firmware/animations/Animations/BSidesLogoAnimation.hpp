#pragma once

#include <Adafruit_GFX.h>
#include <Images/BSidesLogo.hpp>

#include <stringdb.h>

#define BSIDES2023_LOGO_ASCII "" \
    "888888b.    .d8888b.  d8b      888                    .d8888b.   .d8888b.   .d8888b.   .d8888b.\n" \
    "888  \"88b  d88P  Y88b Y8P      888                   d88P  Y88b d88P  Y88b d88P  Y88b d88P  Y88b\n" \
    "888  .88P  Y88b.               888                          888 888    888        888      .d88P\n" \
    "8888888K.   \"Y888b.   888  .d88888  .d88b.  .d8888b       .d88P 888    888      .d88P     8888\"\n" \
    "888  \"Y88b     \"Y88b. 888 d88\" 888 d8P  Y8b 88K       .od888P\"  888    888  .od888P\"       \"Y8b.\n" \
    "888    888       \"888 888 888  888 88888888 \"Y8888b. d88P\"      888    888 d88P\"      888    888\n" \
    "888   d88P Y88b  d88P 888 Y88b 888 Y8b.          X88 888\"       Y88b  d88P 888\"       Y88b  d88P\n" \
    "8888888P\"   \"Y8888P\"  888  \"Y88888  \"Y8888   88888P' 888888888   \"Y8888P\"  888888888   \"Y8888P\"\n" \
    "\n" \
    "\n" \
    "\n" \
    "888      8888888b.               888\n" \
    "888      888   Y88b              888\n" \
    "888      888    888              888\n" \
    "88888b.  888   d88P .d88b.   .d88888\n" \
    "888 \"88b 8888888P\" d88\"\"88b d88\" 888\n" \
    "888  888 888       888  888 888  888\n" \
    "888 d88P 888       Y88..88P Y88b 888\n" \
    "88888P\"  888        \"Y88P\"   \"Y88888\n" \
    "\n"                                             

class BSidesLogoAnimation
{
    public:
        static void run(Adafruit_GFX &gfx) {
            printf("\x1b[2J");
            printf(BSIDES2023_LOGO_ASCII);
            uint8_t sta_mac[6];
            esp_efuse_mac_get_default(sta_mac);
            uint32_t devid = 0;
            devid |= (((uint32_t)sta_mac[5]) <<  0) & 0x000000ff;
            devid |= (((uint32_t)sta_mac[4]) <<  8) & 0x0000ff00;
            devid |= (((uint32_t)sta_mac[3]) << 16) & 0x00ff0000;
            devid |= (((uint32_t)sta_mac[2]) << 24) & 0xff000000;
            char ipsn[12];
            STRING_MEMCPY(&ipsn[0], SERIAL_3BYTE_PREFIX);
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
            STRING_STRCPY(&ipsn[8], SERIAL_3BYTE_SUFFIX);
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
