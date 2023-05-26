#include "MCP23S17.hpp"

#include <SPI.h>
#include <stringdb.h>

#define SPI_PIN_CS     (7)

void MCP23S17Gpio::gpio_update()
{
    // IODIRA register - port A to outputs
    digitalWrite(SPI_PIN_CS, LOW);    
    SPI.transfer(0x40);
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    digitalWrite(SPI_PIN_CS, HIGH);

    // IODIRB register - port B to outputs
    digitalWrite(SPI_PIN_CS, LOW);    
    SPI.transfer(0x40);
    SPI.transfer(0x01);
    SPI.transfer(0x00);
    digitalWrite(SPI_PIN_CS, HIGH);

    // PORTA value
    digitalWrite(SPI_PIN_CS, LOW);
    SPI.transfer(0x40);
    SPI.transfer(0x12);
    SPI.transfer(gpio_a_); 
    digitalWrite(SPI_PIN_CS, HIGH);

    // PORTB value
    digitalWrite(SPI_PIN_CS, LOW);
    SPI.transfer(0x40);
    SPI.transfer(0x13);
    SPI.transfer(gpio_b_); 
    digitalWrite(SPI_PIN_CS, HIGH);
}

int MCP23S17Gpio::gpio_a_pin(uint8_t pin)
{
    return (gpio_a_ >> pin) & 0x1;
}

int MCP23S17Gpio::gpio_b_pin(uint8_t pin)
{
    return (gpio_b_ >> pin) & 0x1;
}

void MCP23S17Gpio::set_gpio_a_pin(uint8_t pin, int value)
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

void MCP23S17Gpio::set_gpio_b_pin(uint8_t pin, int value)
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

void MCP23S17Gpio::begin() {
    pinMode(SPI_PIN_CS, OUTPUT); 
    digitalWrite(SPI_PIN_CS,HIGH);
    // SPI.begin();
    SPI.setFrequency(10000);  // so our sniffing tool will work
    gpio_a_ = 0x00;
    gpio_b_ = 0x00;

    // Use exisiting output values of PORTA
    digitalWrite(SPI_PIN_CS, LOW);    
    SPI.transfer(0x41);
    SPI.transfer(0x12);
    gpio_a_ = SPI.transfer(0x12);    
    digitalWrite(SPI_PIN_CS, HIGH);

    // Use exisiting output values of PORTB
    digitalWrite(SPI_PIN_CS, LOW);    
    SPI.transfer(0x41);
    SPI.transfer(0x13);
    gpio_b_ = SPI.transfer(0x13);    
    digitalWrite(SPI_PIN_CS, HIGH); 

    gpio_update();
    Menu::begin();
}

void MCP23S17Gpio::end() {
    Menu::end();
    SPI.setFrequency(1000000);  // restore
    // SPI.end();
}

void MCP23S17Gpio::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_GPIO_UPPER));
    for ( size_t i = 0; i < 8; i++ )
    {
        STRING_CACHE();
        menu.add(STRING(STRING_IOA) + std::to_string(i) + " " + std::string(gpio_a_pin(i) ? STRING(STRING_ON) : STRING(STRING_OFF)), [&menu, i, this](){ 
            set_gpio_a_pin(i, !gpio_a_pin(i));
            STRING_CACHE();
            menu.set(i, STRING(STRING_IOA) + std::to_string(i) + " " + std::string(gpio_a_pin(i) ? STRING(STRING_ON) : STRING(STRING_OFF)));
            STRING_CLEAR();
        });
        STRING_CLEAR();
    }
    for ( size_t i = 0; i < 8; i++ )
    {
        STRING_CACHE();
        menu.add(STRING(STRING_IOB) + std::to_string(i) + " " + std::string(gpio_b_pin(i) ? STRING(STRING_ON) : STRING(STRING_OFF)), [&menu, i, this](){ 
            set_gpio_b_pin(i, !gpio_b_pin(i));
            STRING_CACHE();
            menu.set(i + 8, STRING(STRING_IOB) + std::to_string(i) + " " + std::string(gpio_b_pin(i) ? STRING(STRING_ON) : STRING(STRING_OFF)));
            STRING_CLEAR();
        });
        STRING_CLEAR();
    }
}

void MCP23S17::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_MCP23S17_UPPER));
    menu.add(STRING(STRING_GPIO_UPPER), [this](){
        App::manager_begin(gpio_);
    });
    menu.add(STRING(STRING_DIAGRAM), [this](){
        diagram_.set_title(STRING(STRING_DIAGRAM));
        diagram_.set_other_name(STRING(STRING_MCP23S17_LOWER));
        diagram_.add_gnd_label();
        diagram_.add_3v3_label();
        diagram_.add_pin_label(SPI_PIN_CS, STRING(STRING_IO) + std::to_string(SPI_PIN_CS), 0x0000, 0xffe0);
        diagram_.add_pin_label(37, STRING(DIAGRAM_MISO), 0xffff, 0x001f);
        diagram_.add_pin_label(36, STRING(DIAGRAM_CLK), 0xffff, 0x8170);
        diagram_.add_pin_label(35, STRING(DIAGRAM_MOSI), 0x0000, 0x07e0);
        diagram_.add_wire_gnd();
        diagram_.add_wire_3v3();
        diagram_.add_wire(SPI_PIN_CS, STRING(DIAGRAM_CS), 0x0000, 0xffe0);
        diagram_.add_wire(37, STRING(DIAGRAM_MISO) + 2, 0xffff, 0x001f);
        diagram_.add_wire(36, STRING(DIAGRAM_SCK), 0xffff, 0x8170);
        diagram_.add_wire(35, STRING(DIAGRAM_MOSI) + 2, 0x0000, 0x07e0);
        App::manager_begin(diagram_);
    });
    menu.add(STRING(STRING_DATASHEET), [this](){
        datasheet_.set_title(STRING(STRING_DATASHEET));
        datasheet_.set_text(STRING(MCP23S17_DATASHEET_URL));
        App::manager_begin(datasheet_);
    });
    menu.add(STRING(STRING_NOTES), [this](){
        notes_.set_title(STRING(STRING_NOTES));
        notes_.set_text(std::string(STRING(MCP23S17_NOTES)));
        App::manager_begin(notes_);
    });
}
