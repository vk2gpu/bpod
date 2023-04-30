#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>
#include <App/Menu.hpp>
#include <App/TextView.hpp>
#include <App/DiagramView.hpp>

class UARTTermBaudRate : public Menu  {
    public:
        UARTTermBaudRate() : baud_(0) {};
        virtual ~UARTTermBaudRate() {};
        virtual void begin(BpodMenu &menu);

        uint32_t baud();

    private:
        uint32_t baud_;
};

class UARTTermConfig : public Menu  {
    public:
        UARTTermConfig() : config_(0) {};
        virtual ~UARTTermConfig() {};
        virtual void begin(BpodMenu &menu);

        uint32_t config();

    private:
        uint32_t config_;
};

class UARTTermOutput : public TextView  {
    public:
        UARTTermOutput() : pause_(false) {};
        virtual ~UARTTermOutput() {};

        virtual void begin();
        virtual void loop();
        virtual void key_event(uint8_t key);
        virtual void keyboard_event(int key);
        virtual void end();

    private:
        bool pause_;
};

class UARTTerm : public Menu  {
    public:
        UARTTerm() {};
        virtual ~UARTTerm() {};
        virtual void begin(BpodMenu &menu);
        virtual void visible();

        uint32_t baud() { return baud_.baud(); }
        uint32_t config() { return config_.config(); };

    private:
        UARTTermBaudRate baud_;
        UARTTermConfig config_;
        UARTTermOutput output_;
        TextView notes_;
        DiagramView diagram_;
};
