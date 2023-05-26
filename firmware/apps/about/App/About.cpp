#include "About.hpp"

#include <Fonts/FreeSansBold9pt7b.h>

#include <Images/BpodTitleBar.hpp>

#include <stringdb.h>

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
        char chip[16];
        STRING_STRCPY(chip, STRING_UNKNOWN);
        switch(chip_info.model)
        {
            case CHIP_ESP32:
                STRING_STRCPY(chip, CHIP_ESP32);
                break;
            case CHIP_ESP32S2:
                STRING_STRCPY(chip, CHIP_ESP32S2);
                break;
            case CHIP_ESP32S3:
                STRING_STRCPY(chip, CHIP_ESP32S3);
                break;
            case CHIP_ESP32C3:
                STRING_STRCPY(chip, CHIP_ESP32C3);
                break;
            case CHIP_ESP32H2:
                STRING_STRCPY(chip, CHIP_ESP32H2);
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
        BpodTitleBar::draw(gfx, STRING(STRING_ABOUT), true);
        gfx.fillRect(0, BpodTitleBar::view_y(gfx), gfx.width(), BpodTitleBar::view_height(gfx), 0x8410);
        printf(STRING(CONSOLE_CLEAR));
        STRING_CACHE();
        printf(STRING(FMT_TITLE), STRING(STRING_ABOUT));
        STRING_CLEAR();
        gfx.setTextColor(0x0000);
        gfx.setTextSize(1);
        gfx.setCursor((gfx.width() / 2) - ((STRING_STRLEN(STRING_BPOD) * 6) / 2), 32);
        gfx.print(STRING(STRING_BPOD));
        STRING_CACHE();
        printf(STRING(FMT_ABOUT_STRING), STRING(STRING_MODEL), STRING(STRING_BPOD));
        STRING_CLEAR();
        gfx.setTextSize(1);
        gfx.setTextColor(0xffff);
        gfx.setCursor(44, 64);
        gfx.setFont(&FreeSansBold9pt7b);
        gfx.printf(STRING(FMT_SIZE_IN_MB), spi_flash_get_chip_size() / (1024 * 1024));
        STRING_CACHE();
        printf(STRING(FMT_ABOUT_SIZE), STRING(STRING_FLASH), spi_flash_get_chip_size() / (1024 * 1024));
        STRING_CLEAR();
        int16_t w = 50;
        int16_t h = 30;
        gfx.drawRoundRect((gfx.width() / 2) - (w / 2), 49, w, h, 5, 0xffff);
        gfx.setFont(nullptr);
        gfx.setTextColor(0xffff);
        gfx.setTextSize(1);
        gfx.setCursor((gfx.width() / 2) - ((strlen(chip) * 6) / 2), 87);
        gfx.print(chip);
        STRING_CACHE();
        printf(STRING(FMT_ABOUT_STRING), STRING(STRING_CHIP), chip);
        STRING_CLEAR();
        gfx.setTextColor(0x2945);
        gfx.setTextSize(1);
        gfx.setCursor(44, gfx.height() - 48);
        gfx.printf(STRING(FMT_FREQ_IN_MHZ), clockCyclesPerMicrosecond());
        STRING_CACHE();
        printf(STRING(FMT_ABOUT_FREQ), STRING(STRING_CLOCK), clockCyclesPerMicrosecond());
        STRING_CLEAR();
        gfx.setTextColor(0x2945);
        gfx.setTextSize(1);
        gfx.setCursor(44, gfx.height() - 36);
        gfx.printf(STRING(FMT_CORE_S), chip_info.cores, chip_info.cores > 1 ? "s" : "");
        STRING_CACHE();
        printf(STRING(FMT_ABOUT_NUMBER), STRING(STRING_CORES), chip_info.cores);
        STRING_CLEAR();
        gfx.setTextColor(0x2945);
        gfx.setTextSize(1);
        gfx.setCursor(14, gfx.height() - 24);
        STRING_CACHE();
        gfx.printf(STRING(FMT_MAC_ADDR),
            sta_mac[0], sta_mac[1], sta_mac[2], sta_mac[3], sta_mac[4], sta_mac[5]);
        STRING_CLEAR();
        STRING_CACHE();
        printf(STRING(FMT_ABOUT_MAC), STRING(STRING_MAC),
            sta_mac[0], sta_mac[1], sta_mac[2], sta_mac[3], sta_mac[4], sta_mac[5]);
        STRING_CLEAR();
        gfx.setTextColor(0x2945);
        gfx.setTextSize(1);
        gfx.setCursor(30, gfx.height() - 12);
        gfx.print(ipsn);
        STRING_CACHE();
        printf(STRING(FMT_ABOUT_STRING), STRING(STRING_SERIAL), ipsn);
        STRING_CLEAR();
    }
}

void About::end(void)
{

}
