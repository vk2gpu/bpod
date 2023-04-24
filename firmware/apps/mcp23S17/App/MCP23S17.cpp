#include "MCP23S17.hpp"

#include <SPI.h>

#define SPI_PIN_CS     (7)

const static char *DATASHEET_URL = "https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf";

const static char *NOTES = "" \
    "The MCP23S17 is a GPIO expander chip. To control the GPIO, follow the wiring diagram " \
    "to connect the needed power and SPI lines. MISO stands for MASTER IN SLAVE OUT and needs " \
    "to be connected to the SLAVE OUT (SO). MOSI stands for MASTER OUT SLAVE IN and needs " \
    "to be connected to the SLAVE IN (IN). A clock signal controlled by the master (bPod) signals " \
    "when data is transfered. Clock is commonly labeled SCK or CLK. To select the slave device, a " \
    "Chip Select (CS) wire is used. CS is often ACTIVE LOW - meaning that the slave is selected " \
    "when the CS line is low or no voltage (instead of high/3V). In schematics, this is shown with " \
    "a line drawn across the top of CS label.";

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

void MCP23S17::begin(BpodMenu &menu) {
    menu.set_title("MCP23S17");
    menu.add("GPIO", [this](){
        App::manager_begin(gpio_);
    });
    menu.add("Diagram", [this](){
        diagram_.set_title("Diagram");
        diagram_.set_other_name("mcp23S17");
        diagram_.add_gnd_label();
        diagram_.add_3v3_label();
        diagram_.add_pin_label(SPI_PIN_CS, "IO" + std::to_string(SPI_PIN_CS), 0x0000, 0xffe0);
        diagram_.add_pin_label(37, "MISO", 0xffff, 0x001f);
        diagram_.add_pin_label(36, "CLK", 0xffff, 0x8170);
        diagram_.add_pin_label(35, "MOSI", 0x0000, 0x07e0);
        diagram_.add_wire_gnd();
        diagram_.add_wire_3v3();
        diagram_.add_wire(SPI_PIN_CS, "~CS", 0x0000, 0xffe0);
        diagram_.add_wire(37, "SO", 0xffff, 0x001f);
        diagram_.add_wire(36, "SCK", 0xffff, 0x8170);
        diagram_.add_wire(35, "SI", 0x0000, 0x07e0);
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
