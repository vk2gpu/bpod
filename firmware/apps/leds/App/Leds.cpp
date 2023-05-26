#include "Leds.hpp"
#include <stringdb.h>

#define LED_COUNT (3)
static const uint8_t led_pin[LED_COUNT] = {
    10,
    16,
    21
};

void Leds::begin() {
    for ( size_t i = 0; i < LED_COUNT; i++ )
    {
        pinMode(led_pin[i], OUTPUT);
        digitalWrite(led_pin[i], HIGH);
    }
    Menu::begin();
}

void Leds::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_LEDS));
    for ( size_t i = 0; i < LED_COUNT; i++ )
    {
        uint8_t pin = led_pin[i];
        STRING_CACHE();
        menu.add(STRING(STRING_IO) + std::to_string(pin) + " " + std::string(digitalRead(pin) ? STRING(STRING_ON) : STRING(STRING_OFF)), [&menu, i, pin, this](){ 
            digitalWrite(pin, digitalRead(pin) ? 0 : 1);
            STRING_CACHE();
            menu.set(i, STRING(STRING_IO) + std::to_string(pin) + " " + std::string(digitalRead(pin) ? STRING(STRING_ON) : STRING(STRING_OFF)));
            STRING_CLEAR();
        });
        STRING_CLEAR();
    }
}
