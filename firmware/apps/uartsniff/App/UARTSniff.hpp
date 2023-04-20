#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>
#include <App/Menu.hpp>
#include <App/TextView.hpp>
#include <App/DiagramView.hpp>

class UARTSniffBaudRate : public Menu  {
    public:
        UARTSniffBaudRate() : baud_(0) {};
        virtual ~UARTSniffBaudRate() {};
        virtual void begin(BpodMenu &menu);

        uint32_t baud();

    private:
        uint32_t baud_;
};

class UARTSniffConfig : public Menu  {
    public:
        UARTSniffConfig() : config_(0) {};
        virtual ~UARTSniffConfig() {};
        virtual void begin(BpodMenu &menu);

        uint32_t config();

    private:
        uint32_t config_;
};

class UARTSniffOutput : public TextView  {
    public:
        UARTSniffOutput() : pause_(false) {};
        virtual ~UARTSniffOutput() {};

        virtual void begin();
        virtual void loop();
        virtual void key_event(uint8_t key);
        virtual void end();

    private:
        bool pause_;
};

class UARTSniff : public Menu  {
    public:
        UARTSniff() {};
        virtual ~UARTSniff() {};
        virtual void begin(BpodMenu &menu);
        virtual void visible();

        uint32_t baud() { return baud_.baud(); }
        uint32_t config() { return config_.config(); };

    private:
        UARTSniffBaudRate baud_;
        UARTSniffConfig config_;
        UARTSniffOutput output_;
        TextView notes_;
        DiagramView diagram_;
};
