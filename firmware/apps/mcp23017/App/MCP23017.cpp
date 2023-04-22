#include "MCP23017.hpp"

#include <Wire.h>

const static char *DATASHEET_URL = "https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf";

const static char *NOTES = "" \
    "The MCP23017 is a GPIO expander chip. To control the GPIO, follow the wiring diagram " \
    "to connect the needed power and I2C lines. Each I2C device has its own address ranging " \
    "from 0x00 to 0x7f. The MCP23017 allows you to set its address using pins A0 A1 A2. If you " \
    "connect A0 A1 A2 to GND, the address is 0x20. If you connect A0 A1 A2 to 3V, the address " \
    "is 0x2f. Hence you can have multiple devices connected to the one pair of SDA/SCK wires. Make sure " \
    "you select the correct I2C address so your GPIO commands go to the right chip.";

static std::string addr_to_str(uint8_t addr)
{
    char buf[10];
    sprintf(buf, "0x%02x", addr);
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
    menu.set_title("GPIO");
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

void MCP23017::begin(BpodMenu &menu) {
    select_addr_.selectable();
    select_addr_.set_addr(gpio_.addr());
    menu.set_title("MCP23017");
    menu.add("Addr: " + addr_to_str(select_addr_.addr()), [this](){
        App::manager_begin(select_addr_);
    });
    menu.add("GPIO", [this](){
        App::manager_begin(gpio_);
    });
    menu.add("Diagram", [this](){
        diagram_.set_title("Diagram");
        diagram_.set_other_name("mcp23017");
        diagram_.add_gnd_label();
        diagram_.add_3v3_label();
        diagram_.add_pin_label(9, "SCK", 0xffff, 0x8170);
        diagram_.add_pin_label(8, "SDA", 0x0000, 0x07e0);
        diagram_.add_wire_gnd();
        diagram_.add_wire_3v3();
        diagram_.add_wire(9, "SCK", 0xffff, 0x8170);
        diagram_.add_wire(8, "SDA", 0x0000, 0x07e0);
        App::manager_begin(diagram_);
    });
    menu.add("Datasheet", [this](){
        datasheet_.set_title("Datasheet");
        datasheet_.set_text(DATASHEET_URL);
        App::manager_begin(datasheet_);
    });
    menu.add("Notes", [this](){
        notes_.set_title("Notes");
        notes_.set_text(std::string(NOTES));
        App::manager_begin(notes_);
    });
}

void MCP23017::visible()
{
    gpio_.set_addr(select_addr_.addr());
    Menu::set(0, "Addr: " + addr_to_str(select_addr_.addr()));
    diagram_.clear();
    notes_.clear();
    Menu::visible();
};
