#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>
#include <App/Menu.hpp>
#include <App/TextView.hpp>

class SerialMonitorBaudRate : public Menu  {
    public:
        SerialMonitorBaudRate() : baud_(0) {};
        virtual ~SerialMonitorBaudRate() {};
        virtual void begin(BpodMenu &menu);

        uint32_t baud();

    private:
        uint32_t baud_;
};

class SerialMonitorConfig : public Menu  {
    public:
        SerialMonitorConfig() : config_(0) {};
        virtual ~SerialMonitorConfig() {};
        virtual void begin(BpodMenu &menu);

        uint32_t config();

    private:
        uint32_t config_;
};

class SerialMonitorOutput : public TextView  {
    public:
        SerialMonitorOutput() : pause_(false) {};
        virtual ~SerialMonitorOutput() {};

        virtual void begin();
        virtual void loop();
        virtual void key_event(uint8_t key);
        virtual void end();

    private:
        bool pause_;
};

class SerialMonitor : public Menu  {
    public:
        SerialMonitor() {};
        virtual ~SerialMonitor() {};
        virtual void begin(BpodMenu &menu);
        virtual void visible();

        uint32_t baud() { return baud_.baud(); }
        uint32_t config() { return config_.config(); };

    private:
        SerialMonitorBaudRate baud_;
        SerialMonitorConfig config_;
        SerialMonitorOutput output_;
};
