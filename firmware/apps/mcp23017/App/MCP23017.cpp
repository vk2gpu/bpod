#include "MCP23017.hpp"

#include <Wire.h>


void MCP23017::gpio_update()
{
    // IODIRA register - port A to outputs
    Wire.beginTransmission(0x20);
    Wire.write(0x00); 
    Wire.write(0x00);
    Wire.endTransmission();

    // IODIRB register - port B to outputs
    Wire.beginTransmission(0x20);
    Wire.write(0x01);
    Wire.write(0x00);
    Wire.endTransmission();

    // PORTA value
    Wire.beginTransmission(0x20);
    Wire.write(0x12);
    Wire.write(gpio_a_);
    Wire.endTransmission();

    // PORTB value
    Wire.beginTransmission(0x20);
    Wire.write(0x13);
    Wire.write(gpio_b_);
    Wire.endTransmission();
}

int MCP23017::gpio_a_pin(uint8_t pin)
{
    return (gpio_a_ >> pin) & 0x1;
}

int MCP23017::gpio_b_pin(uint8_t pin)
{
    return (gpio_b_ >> pin) & 0x1;
}

void MCP23017::set_gpio_a_pin(uint8_t pin, int value)
{
    if ( value )
    {
        gpio_a_ |= 1 << pin;
    }
    else
    {
        gpio_a_ &= ~(1 << pin);
    }
    gpio_update();
}

void MCP23017::set_gpio_b_pin(uint8_t pin, int value)
{
    if ( value )
    {
        gpio_b_ |= 1 << pin;
    }
    else
    {
        gpio_b_ &= ~(1 << pin);
    }
    gpio_update();
}

void MCP23017::begin() {
    Wire.begin();
    gpio_a_ = 0x00;
    gpio_b_ = 0x00;

    // Use exisiting output values of PORTA
    Wire.beginTransmission(0x20);
    Wire.write(0x12);
    Wire.endTransmission();
    Wire.requestFrom(0x20, 1);
    gpio_a_ = Wire.read();
    Wire.endTransmission();

    // Use exisiting output values of PORTB
    Wire.beginTransmission(0x20);
    Wire.write(0x13);
    Wire.endTransmission();
    Wire.requestFrom(0x20, 1);
    gpio_b_ = Wire.read();
    Wire.endTransmission();

    gpio_update();
    Menu::begin();
}

void MCP23017::end() {
    Menu::end();
    Wire.end();
}

void MCP23017::begin(BpodMenu &menu) {
    menu.set_title("MCP23017");
    for ( size_t i = 0; i < 8; i++ )
    {
        menu.add("IOA" + std::to_string(i) + " " + std::string(gpio_a_pin(i) ? "ON" : "OFF"), [&menu, i, this](){ 
            set_gpio_a_pin(i, !gpio_a_pin(i));
            menu.set(i, "IOA" + std::to_string(i) + " " + std::string(gpio_a_pin(i) ? "ON" : "OFF"));
        });
    }
    for ( size_t i = 0; i < 8; i++ )
    {
        menu.add("IOB" + std::to_string(i) + " " + std::string(gpio_b_pin(i) ? "ON" : "OFF"), [&menu, i, this](){ 
            set_gpio_b_pin(i, !gpio_b_pin(i));
            menu.set(i + 8, "IOB" + std::to_string(i) + " " + std::string(gpio_b_pin(i) ? "ON" : "OFF"));
        });
    }
}
