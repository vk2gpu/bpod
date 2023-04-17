#include "Bpod.hpp"

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_ST7735.h>
#include <TouchWheel.h>

#include <Animations/BpodLoadAnimation.hpp>
#include <Animations/BSidesLogoAnimation.hpp>

#include <App/App.hpp>
#include <App/MainMenu.hpp>

#define CS_PIN  34 // IO34
#define DC_PIN  33 // IO33
#define RST_PIN 38 // IO38
#define BKL_PIN 45 // IO45

#define TOUCH_000DEG  1 // TOUCH1 IO1
#define TOUCH_120DEG  2 // TOUCH2 IO2
#define TOUCH_240DEG  3 // TOUCH3 IO3
#define TOUCH_OK      4 // TOUCH4 IO4

SPIClass fspi(FSPI);
Adafruit_ST7735 tft(&fspi, CS_PIN, DC_PIN, RST_PIN);
TouchWheel wheel(TOUCH_000DEG, TOUCH_120DEG, TOUCH_240DEG, TOUCH_OK, false);
MainMenu main_menu;

void Bpod::begin()
{
    pinMode(BKL_PIN, OUTPUT);
    digitalWrite(BKL_PIN, LOW);
    pinMode(CS_PIN, OUTPUT);
    pinMode(DC_PIN, OUTPUT);
    pinMode(RST_PIN, OUTPUT);
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST77XX_BLACK);
    tft.setRotation(2);
    tft.fillScreen(ST77XX_BLACK);
    digitalWrite(BKL_PIN, HIGH);

    // LEDs
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    pinMode(16, OUTPUT);
    digitalWrite(16, HIGH);
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);

    // LED4, connected to GPIO46, cannout be used as an output hence, LED4 is not working
    // pinMode(46, OUTPUT);
    // digitalWrite(46, HIGH);

    // touch wheel
    wheel.begin();

    // bPod boot logo
    BpodLoadAnimation::run(tft);

    // BSides 2023 lazer magpie
    BSidesLogoAnimation::run(tft);

    // Main menu
    App::manager_begin(main_menu);
}

void Bpod::loop()
{
    int16_t clicks = 0;
    wheel.read();
    clicks = wheel.wheel_pop_clicks();
    while ( clicks > 0 )
    {
        App::manager_key_event(APP_KEY_SCROLL_CLOCKWISE);
        clicks -= 1;
    }
    while ( clicks < 0 )
    {
        App::manager_key_event(APP_KEY_SCROLL_ANTICLOCKWISE);
        clicks += 1;
    }
    clicks = wheel.ok_pop_clicks();
    while ( clicks > 0 )
    {
        App::manager_key_event(APP_KEY_OK);
        clicks -= 1;
    }
    clicks = wheel.forward_pop_clicks();
    while ( clicks > 0 )
    {
        App::manager_key_event(APP_KEY_FORWARD);
        clicks -= 1;
    }
    clicks = wheel.menu_pop_clicks();
    while ( clicks > 0 )
    {
        App::manager_key_event(APP_KEY_MENU);
        clicks -= 1;
    }
    clicks = wheel.back_pop_clicks();
    while ( clicks > 0 )
    {
        App::manager_key_event(APP_KEY_BACK);
        clicks -= 1;
    }
    clicks = wheel.play_pop_clicks();
    while ( clicks > 0 )
    {
        App::manager_key_event(APP_KEY_PLAY);
        clicks -= 1;
    }
    App::manager_loop();
    App::manager_draw(tft);
    yield();
}
