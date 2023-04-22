#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>
#include <App/Menu.hpp>
#include <App/DiagramView.hpp>
#include <App/TextView.hpp>

class I2CSniffMonitor : public TextView  {
    public:
        I2CSniffMonitor() : pause_(false) {};
        virtual ~I2CSniffMonitor() {};

        virtual void begin();
        virtual void loop();
        virtual void key_event(uint8_t key);
        virtual void end();

    private:
        bool pause_;
        uint32_t last_parse_;
};

class I2CSniff : public Menu  {
    public:
        I2CSniff() {};
        virtual ~I2CSniff() {};
        virtual void begin(BpodMenu &menu);
        virtual void visible() { diagram_.clear(); notes_.clear(); };

    private:
        I2CSniffMonitor sniff_;
        DiagramView diagram_;
        TextView notes_;
};
