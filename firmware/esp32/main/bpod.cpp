#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_ST7735.h>
#include <TouchWheel.h>

#include <Images/BpodLogo.hpp>
#include <Images/BSidesLogo.hpp>
#include <Images/BpodTitleBar.hpp>
#include <Images/BpodMenuBar.hpp>
#include <Images/BpodMenuArrow.hpp>
#include <Images/BpodScrollEmpty.hpp>
#include <Images/BpodScrollFill.hpp>

#define CS_PIN  27 // IO27
#define DC_PIN  25 // IO25
#define RST_PIN 26 // IO26

#define TOUCH_000DEG  4 // TOUCH0 IO4
#define TOUCH_120DEG  33 // TOUCH8 IO33
#define TOUCH_240DEG  32 // TOUCH9 IO32
#define TOUCH_OK      0 // TOUCH1 IO0

SPIClass hspi(HSPI);
Adafruit_ST7735 tft(&hspi, CS_PIN, DC_PIN, RST_PIN);
TouchWheel wheel(TOUCH_000DEG, TOUCH_120DEG, TOUCH_240DEG, TOUCH_OK, true);

extern "C" void app_main(void)
{
    initArduino();

    printf("The beginnings of bPod!\n");

    pinMode(CS_PIN, OUTPUT);
    pinMode(DC_PIN, OUTPUT);
    pinMode(RST_PIN, OUTPUT);
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST77XX_BLACK);
    tft.setRotation(2);
    tft.fillScreen(ST77XX_BLACK);

    /*
    wheel.begin();

    Serial.begin(115200);
    Serial.print(F("Hello! ST77xx TFT Test\n"));

    while ( 1 )
    {
        int16_t clicks = 0;
        wheel.read();
        clicks = wheel.wheel_pop_clicks();
        while ( clicks > 0 )
        {
            printf("<------\n");
            clicks -= 1;
        }
        while ( clicks < 0 )
        {
            printf("------>\n");
            clicks += 1;
        }
        clicks = wheel.ok_pop_clicks();
        while ( clicks > 0 )
        {
            printf("OK\n");
            clicks -= 1;
        }
        clicks = wheel.forward_pop_clicks();
        while ( clicks > 0 )
        {
            printf(">>|\n");
            clicks -= 1;
        }
        clicks = wheel.menu_pop_clicks();
        while ( clicks > 0 )
        {
            printf("MENU\n");
            clicks -= 1;
        }
        clicks = wheel.back_pop_clicks();
        while ( clicks > 0 )
        {
            printf("|<<\n");
            clicks -= 1;
        }
        clicks = wheel.play_pop_clicks();
        while ( clicks > 0 )
        {
            printf("|>||\n");
            clicks -= 1;
        }
        delay(1);
    }
    */

    // bPod boot logo
    tft.fillScreen(ST77XX_BLACK);
    BpodLogo::draw(21, 30, tft);
    tft.drawRoundRect(8, 110, 112, 12, 4, 0xffff);
    for ( int16_t w = 0; w < 111; w++)
    {
        tft.fillRect(9, 111, w, 10, 0xffff);
        delay(25);
    }

    // BSides 2023 lazer magpie
    BSidesLogo::draw(0, 0, tft);
    uint16_t color = 0xf800;
    for ( size_t i = 0; i < 200; i++)
    {
        tft.drawLine(50, 41, 60, 59, color);
        tft.drawLine(51, 40, 60, 59, color);
        tft.drawLine(52, 41, 60, 59, color);
        tft.drawLine(58, 40, 66, 61, color);
        tft.drawLine(59, 39, 66, 61, color);
        tft.drawLine(60, 40, 66, 61, color);
        color = ( color >> 1 ) | (( color & 0x1) << 15);
        delay(25);
    }

    // Menu cycling
    tft.fillScreen(0xffff);
    BpodTitleBar::draw(0, 0, 128, tft);
    BpodScrollEmpty::draw(128 - 8, 20, 160 - 20, tft);
    BpodScrollFill::draw(128 - 8, 57, 95, tft);
    size_t pos = 0;
    while ( 1 )
    {
        BpodMenuBar::draw(0, 20 + (pos * 20), 128 - 8, tft);
        BpodMenuArrow::draw(128 - 16 - 4, 20 + (pos * 20) + 4, tft);
        delay(1000);
        tft.fillRect(0, 20 + (pos * 20), 128 - 8, 20 + (pos * 20) + 20, 0xffff);
        pos++;
        if ( pos >= 7 )
        {
            pos = 0;
        }
    }

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
