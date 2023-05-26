#include "MCP23017.hpp"

#include <Wire.h>
#include <stringdb.h>

static std::string addr_to_str(uint8_t addr)
{
    char buf[10];
    sprintf(buf, STRING(FMT_I2C_ADDR), addr);
    return buf;
}

void MCP23017Gpio::gpio_update()
{
    // IODIRA register - port A to outputs
    Wire.beginTransmission(addr_);
    Wire.write(0x00); 
    Wire.write(0x00);
    Wire.endTransmission();

    // IODIRB register - port B to outputs
    Wire.beginTransmission(addr_);
    Wire.write(0x01);
    Wire.write(0x00);
    Wire.endTransmission();

    // PORTA value
    Wire.beginTransmission(addr_);
    Wire.write(0x12);
    Wire.write(gpio_a_);
    Wire.endTransmission();

    // PORTB value
    Wire.beginTransmission(addr_);
    Wire.write(0x13);
    Wire.write(gpio_b_);
    Wire.endTransmission();
}

int MCP23017Gpio::gpio_a_pin(uint8_t pin)
{
    return (gpio_a_ >> pin) & 0x1;
}

int MCP23017Gpio::gpio_b_pin(uint8_t pin)
{
    return (gpio_b_ >> pin) & 0x1;
}

void MCP23017Gpio::set_gpio_a_pin(uint8_t pin, int value)
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

void MCP23017Gpio::set_gpio_b_pin(uint8_t pin, int value)
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

void MCP23017Gpio::begin() {
    Wire.begin();
    gpio_a_ = 0x00;
    gpio_b_ = 0x00;

    // Use exisiting output values of PORTA
    Wire.beginTransmission(addr_);
    Wire.write(0x12);
    Wire.endTransmission();
    Wire.requestFrom((int)addr_, 1);
    gpio_a_ = Wire.read();
    Wire.endTransmission();

    // Use exisiting output values of PORTB
    Wire.beginTransmission(addr_);
    Wire.write(0x13);
    Wire.endTransmission();
    Wire.requestFrom((int)addr_, 1);
    gpio_b_ = Wire.read();
    Wire.endTransmission();

    gpio_update();
    Menu::begin();
}

void MCP23017Gpio::end() {
    Menu::end();
    Wire.end();
}

void MCP23017Gpio::begin(BpodMenu &menu) {
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

void MCP23017::begin(BpodMenu &menu) {
    select_addr_.selectable();
    select_addr_.set_addr(gpio_.addr());
    menu.set_title(STRING(STRING_MCP23017_UPPER));
    STRING_CACHE();
    menu.add(STRING(STRING_I2C_ADDR_LABEL) + addr_to_str(select_addr_.addr()), [this](){
        App::manager_begin(select_addr_);
    });
    STRING_CLEAR();
    menu.add(STRING(STRING_GPIO_UPPER), [this](){
        App::manager_begin(gpio_);
    });
    menu.add(STRING(STRING_DIAGRAM), [this](){
        diagram_.set_title(STRING(STRING_DIAGRAM));
        diagram_.set_other_name(STRING(STRING_MCP23017_LOWER));
        diagram_.add_gnd_label();
        diagram_.add_3v3_label();
        diagram_.add_pin_label(9, STRING(DIAGRAM_SCK), 0xffff, 0x8170);
        diagram_.add_pin_label(8, STRING(DIAGRAM_SDA), 0x0000, 0x07e0);
        diagram_.add_wire_gnd();
        diagram_.add_wire_3v3();
        diagram_.add_wire(9, STRING(DIAGRAM_SCK), 0xffff, 0x8170);
        diagram_.add_wire(8, STRING(DIAGRAM_SDA), 0x0000, 0x07e0);
        App::manager_begin(diagram_);
    });
    menu.add(STRING(STRING_DATASHEET), [this](){
        datasheet_.set_title(STRING(STRING_DATASHEET));
        datasheet_.set_text(STRING(MCP23017_DATASHEET_URL));
        App::manager_begin(datasheet_);
    });
    menu.add(STRING(STRING_NOTES), [this](){
        notes_.set_title(STRING(STRING_NOTES));
        notes_.set_text(std::string(STRING(MCP23017_NOTES)));
        App::manager_begin(notes_);
    });
}

void MCP23017::visible()
{
    gpio_.set_addr(select_addr_.addr());
    STRING_CACHE();
    Menu::set(0, STRING(STRING_I2C_ADDR_LABEL) + addr_to_str(select_addr_.addr()));
    STRING_CLEAR();
    diagram_.clear();
    notes_.clear();
    Menu::visible();
};
