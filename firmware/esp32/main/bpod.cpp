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

#include <Animations/BpodLoadAnimation.hpp>
#include <Animations/BSidesLogoAnimation.hpp>

#include <Menu/BpodMenu.hpp>

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

    // touch wheel
    wheel.begin();

    // bPod boot logo
    BpodLoadAnimation::run(tft);

    // BSides 2023 lazer magpie
    BSidesLogoAnimation::run(tft);

    // Main menu
    BpodMenu main_menu;
    main_menu.set_title("bPod");
    for ( size_t i = 0; i < 50; i++ )
    {
        main_menu.add("Menu " + std::to_string(i), [](){});
    }

    while ( 1 )
    {
        int16_t clicks = 0;
        wheel.read();
        clicks = wheel.wheel_pop_clicks();
        main_menu.move_clicks(clicks);
        while ( clicks > 0 )
        {
            printf("------>\n");
            clicks -= 1;
        }
        while ( clicks < 0 )
        {
            printf("<------\n");
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
        main_menu.draw(tft);
        delay(1);
    }
}
