#include "MCP23S17.hpp"

#include <SPI.h>


void MCP23S17::gpio_update()
{
    // IODIRA register - port A to outputs
    digitalWrite(5, LOW);    
    SPI.transfer(0x40);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    digitalWrite(5, HIGH);

    // IODIRB register - port B to outputs
    digitalWrite(5, LOW);    
    SPI.transfer(0x40);
    SPI.transfer(0x01);
    SPI.transfer(0x00);
    digitalWrite(5, HIGH);

    // PORTA value
    digitalWrite(5, LOW);
    SPI.transfer(0x40);
    SPI.transfer(0x12);
    SPI.transfer(gpio_a_); 
    digitalWrite(5, HIGH);

    // PORTB value
    digitalWrite(5, LOW);
    SPI.transfer(0x40);
    SPI.transfer(0x13);
    SPI.transfer(gpio_b_); 
    digitalWrite(5, HIGH);
}

int MCP23S17::gpio_a_pin(uint8_t pin)
{
    return (gpio_a_ >> pin) & 0x1;
}

int MCP23S17::gpio_b_pin(uint8_t pin)
{
    return (gpio_b_ >> pin) & 0x1;
}

void MCP23S17::set_gpio_a_pin(uint8_t pin, int value)
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

void MCP23S17::set_gpio_b_pin(uint8_t pin, int value)
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

void MCP23S17::begin() {
    pinMode(5, OUTPUT); 
    digitalWrite(5,HIGH);
    SPI.begin();
    gpio_a_ = 0x00;
    gpio_b_ = 0x00;

    // Use exisiting output values of PORTA
    digitalWrite(5, LOW);    
    SPI.transfer(0x41);
    SPI.transfer(0x12);
    gpio_a_ = SPI.transfer(0x12);    
    digitalWrite(5, HIGH); 

    // Use exisiting output values of PORTB
    digitalWrite(5, LOW);    
    SPI.transfer(0x41);
    SPI.transfer(0x13);
    gpio_b_ = SPI.transfer(0x13);    
    digitalWrite(5, HIGH); 

    gpio_update();
    Menu::begin();
}

void MCP23S17::end() {
    Menu::end();
    SPI.end();
}

void MCP23S17::begin(BpodMenu &menu) {
    menu.set_title("MCP23S17");
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
