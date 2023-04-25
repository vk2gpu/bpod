#include "About.hpp"

#include <Fonts/FreeSansBold9pt7b.h>

#include <Images/BpodTitleBar.hpp>

void About::begin(void)
{
    redraw_ = true;
}

void About::key_event(uint8_t key)
{
    switch( key )
    {
        case APP_KEY_MENU:
            App::manager_end();
            break;
    }
}

void About::draw(Adafruit_GFX &gfx)
{
    if ( redraw_ )
    {
        redraw_ = false;
        esp_chip_info_t chip_info;
        esp_chip_info(&chip_info);
        const char *chip = "UNKNOWN";
        switch(chip_info.model)
        {
            case CHIP_ESP32:
                chip = "ESP32";
                break;
            case CHIP_ESP32S2:
                chip = "ESP32-S2";
                break;
            case CHIP_ESP32S3:
                chip = "ESP32-S3";
                break;
            case CHIP_ESP32C3:
                chip = "ESP32-C3";
                break;
            case CHIP_ESP32H2:
                chip = "ESP32-H2";
                break;
            default:
                break;
        }
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
        BpodTitleBar::draw(gfx, "About", true);
        gfx.fillRect(0, BpodTitleBar::view_y(gfx), gfx.width(), BpodTitleBar::view_height(gfx), 0x8410);
        gfx.setTextColor(0x0000);
        gfx.setTextSize(1);
        gfx.setCursor((gfx.width() / 2) - ((strlen("bPod") * 6) / 2), 32);
        gfx.print("bPod");
        gfx.setTextSize(1);
        gfx.setTextColor(0xffff);
        gfx.setCursor(44, 64);
        gfx.setFont(&FreeSansBold9pt7b);
        gfx.printf("%dMB", spi_flash_get_chip_size() / (1024 * 1024));
        int16_t w = 50;
        int16_t h = 30;
        gfx.drawRoundRect((gfx.width() / 2) - (w / 2), 49, w, h, 5, 0xffff);
        gfx.setFont(nullptr);
        gfx.setTextColor(0xffff);
        gfx.setTextSize(1);
        gfx.setCursor((gfx.width() / 2) - ((strlen(chip) * 6) / 2), 87);
        gfx.print(chip);
        gfx.setTextColor(0x2945);
        gfx.setTextSize(1);
        gfx.setCursor(44, gfx.height() - 48);
        gfx.printf("%dMHz", clockCyclesPerMicrosecond());
        gfx.setTextColor(0x2945);
        gfx.setTextSize(1);
        gfx.setCursor(40, gfx.height() - 36);
        if ( chip_info.cores > 1 )
        {
            gfx.printf("%d cores", chip_info.cores);
        }
        else
        {
            gfx.printf("%d core", chip_info.cores);
        }
        gfx.setTextColor(0x2945);
        gfx.setTextSize(1);
        gfx.setCursor(14, gfx.height() - 24);
        gfx.printf("%02x:%02x:%02x:%02x:%02x:%02x",
            sta_mac[0], sta_mac[1], sta_mac[2], sta_mac[3], sta_mac[4], sta_mac[5]);
        gfx.setTextColor(0x2945);
        gfx.setTextSize(1);
        gfx.setCursor(30, gfx.height() - 12);
        gfx.print(ipsn);
    }
}

void About::end(void)
{

}
