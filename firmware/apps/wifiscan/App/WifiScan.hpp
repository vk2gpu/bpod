#pragma once

#include <App/Menu.hpp>

class WifiScan : public Menu  {
    public:
        WifiScan() : last_scan_(0), scan_state_(0), ap_count_(0) {};
        virtual ~WifiScan() {};
        virtual void begin();
        virtual void begin(BpodMenu &menu);
        virtual void loop();
        virtual void end();

        void scan();
    
    private:
        uint32_t last_scan_;
        int scan_state_;
        int ap_count_;
        pthread_t scan_thread_;
};
