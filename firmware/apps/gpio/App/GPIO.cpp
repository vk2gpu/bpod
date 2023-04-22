#include "GPIO.hpp"

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
    menu.set_title("GPIO");
    for ( size_t i = 0; i < GPIO_COUNT; i++ )
    {
        uint8_t pin = gpio_pin[i];
        menu.add("IO " + std::to_string(pin) + " " + std::string(digitalRead(pin) ? "ON" : "OFF"), [&menu, i, pin, this](){ 
            digitalWrite(pin, digitalRead(pin) ? 0 : 1);
            menu.set(i, "IO " + std::to_string(pin) + " " + std::string(digitalRead(pin) ? "ON" : "OFF"));
        });
    }
}
