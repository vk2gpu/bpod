#pragma once

#include <Adafruit_GFX.h>
#include <Images/BpodLogo.hpp>

#include <stringdb.h>

#define BPOD_LOGO_ASCII "" \
    "                                               /##&@@@@@@@@&%%#/.\n" \
    "                                          ,#@@@@@@@@@@@@@@@@@@@@@@%%/\n" \
    "                                       ,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@(\n" \
    "                                     (@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#.\n" \
    "                                   *&@@@@@@@@@@@&#,        ./%%@@@@@@@@@@@&/\n" \
    "                                  (@@@@@@@@@@&/                ,%%@@@@@@&&&&(\n" \
    "                                 /@@@@@@@@@&*                    .&&&&&&&&&&/\n" \
    "                                ,&@@@@@@@@&,                       &&&&&&&&&&,\n" \
    "              .@@@@@/           *@@@@@@@@@(         &@@@&#          &&&&&&&&&*\n" \
    "           /@@@@@@@%%,           *@@@@@@@@@(         %%&&&&%%.\n" \
    "         *&@@@@@@&%%,            *&@@@@@@@@&           //\n" \
    "        (@@@&&&%%%%(               /@@@@&&&&&%%\n" \
    "       ,((((((#.                  (&&&&&&&&&%%*\n" \
    "        /(*.                       /&&&&&&&&&%%*\n" \
    "                                    *%%&&&&&&&%%(.\n" \
    "  ,%%%%%%%%%%%%%%%%%%.         /&@&%%*         #&&%%%%%%%%%%%%%%*\n" \
    "  &@@@@@@@@@.        /&&&&&&         /%%%%%%%%%%%%%%%%&/\n" \
    "  *@@@@@@@@@,          *%%(.         .#%%%%%%%%%%%%%%%%%%*\n" \
    "  .@@@@@@@@@@,                      (%%%%%%%%%%%%%%%%%%(\n" \
    "   ,@@@@@@@@@@&.                  *#%%%%%%%%%%%%###(\n" \
    "    ,&@@@@@@@@@&&(             .(%%%%%%%%%%%%%%####(\n" \
    "      (@@@@@@&&&&&&&&#((((//%%%%%%%%%%%%%%%%%%%%#####/\n" \
    "        (&@@&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%######/\n" \
    "          .%%&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%#####/\n" \
    "              *(%%&&&%%%%%%%%%%%%%%%%%%%%%%#%%#*,\n" \
    "                     .,,,,,,\n"

class BpodLoadAnimation
{
    public:
        static void run(Adafruit_GFX &gfx) {
            printf(STRING(CONSOLE_CLEAR));
            printf(BPOD_LOGO_ASCII);
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
            gfx.fillScreen(0x0000);
            BpodLogo::draw(21, 30, gfx);
            gfx.setTextColor(0x4208, 0x0000);
            gfx.setTextSize(1);
            gfx.setCursor(30, gfx.height() - 12);
            gfx.print(ipsn);
            gfx.drawRoundRect(8, 110, 112, 12, 4, 0xffff);
            for ( int16_t w = 0; w < 111; w++)
            {
                gfx.fillRect(9, 111, w, 10, 0xffff);
                delay(25);
            }
        };
};
