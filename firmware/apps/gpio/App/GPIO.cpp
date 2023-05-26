#include "GPIO.hpp"

#include <stringdb.h>

#define GPIO_COUNT (3)
static const uint8_t gpio_pin[GPIO_COUNT] = {
    5,
    6,
    7
};

void GPIOs::begin()
{    
    for ( size_t i = 0; i < GPIO_COUNT; i++ )
    {
        pinMode(gpio_pin[i], OUTPUT);
    }
    Menu::begin();
}

void GPIOs::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_GPIO_UPPER));
    for ( size_t i = 0; i < GPIO_COUNT; i++ )
    {
        uint8_t pin = gpio_pin[i];
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
