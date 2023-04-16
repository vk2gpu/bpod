#pragma once

#include <App/Menu.hpp>

class MCP23S17 : public Menu  {
    public:
        MCP23S17() : gpio_a_(0x00), gpio_b_(0x00) {};
        virtual ~MCP23S17() {};
        virtual void begin();
        virtual void begin(BpodMenu &menu);
        virtual void end();
        void gpio_reset();
        void gpio_update();
        int gpio_a_pin(uint8_t pin);
        int gpio_b_pin(uint8_t pin);
        void set_gpio_a_pin(uint8_t pin, int value);
        void set_gpio_b_pin(uint8_t pin, int value);

    private:
        uint8_t gpio_a_;
        uint8_t gpio_b_;
};
