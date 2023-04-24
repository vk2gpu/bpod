#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>
#include <App/Menu.hpp>
#include <App/DiagramView.hpp>
#include <App/TextView.hpp>

class SPISniffMonitor : public TextView  {
    public:
        SPISniffMonitor() : pause_(false) {};
        virtual ~SPISniffMonitor() {};

        virtual void begin();
        virtual void loop();
        virtual void key_event(uint8_t key);
        virtual void end();

    private:
        bool pause_;
        uint32_t last_parse_;
};

class SPISniff : public Menu  {
    public:
        SPISniff() {};
        virtual ~SPISniff() {};
        virtual void begin(BpodMenu &menu);
        virtual void visible() { diagram_.clear(); notes_.clear(); Menu::visible(); };

    private:
        SPISniffMonitor sniff_;
        DiagramView diagram_;
        TextView notes_;
};
