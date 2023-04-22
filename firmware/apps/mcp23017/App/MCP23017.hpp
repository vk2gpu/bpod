#pragma once

#include <App/Menu.hpp>
#include <App/DiagramView.hpp>
#include <App/QrCodeView.hpp>
#include <App/TextView.hpp>
#include <App/I2CDetect.hpp>

class MCP23017Gpio : public Menu  {
    public:
        MCP23017Gpio() : addr_(0x20), gpio_a_(0x00), gpio_b_(0x00) {};
        virtual ~MCP23017Gpio() {};
        virtual void begin();
        virtual void begin(BpodMenu &menu);
        virtual void end();
        void gpio_reset();
        void gpio_update();
        int gpio_a_pin(uint8_t pin);
        int gpio_b_pin(uint8_t pin);
        void set_gpio_a_pin(uint8_t pin, int value);
        void set_gpio_b_pin(uint8_t pin, int value);

        uint8_t addr() const { return addr_; };
        void set_addr(uint8_t addr) { addr_ = addr; };

    private:
        uint8_t addr_;
        uint8_t gpio_a_;
        uint8_t gpio_b_;
};

class MCP23017 : public Menu  {
    public:
        MCP23017() {};
        virtual ~MCP23017() {};
        virtual void begin(BpodMenu &menu);
        virtual void visible();

    private:
        I2CDetect select_addr_;
        MCP23017Gpio gpio_;
        DiagramView diagram_;
        QrCodeView datasheet_;
        TextView notes_;
};
