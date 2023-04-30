#include "WifiScan.hpp"

#include <WiFi.h>

void WifiScan::begin()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    last_scan_ = 0;
    scan_state_ = 0;
    ap_count_ = 0;
    Menu::begin();
}

void WifiScan::begin(BpodMenu &menu) {
    menu.set_title("WifiScan");
}

static void *scan_thread(void *ctx)
{
    reinterpret_cast<WifiScan*>(ctx)->scan();
    return NULL;
}

void WifiScan::scan()
{
    ap_count_ =  WiFi.scanNetworks();
    scan_state_ = 2;
}

void WifiScan::loop()
{
    if ( 0 == scan_state_ )
    {
        if ( ( 0 == last_scan_ ) || ( (millis() - last_scan_) > 5000 ) )
        {
            last_scan_ = millis();
            scan_state_ = 1;
            pthread_create(&scan_thread_, NULL, scan_thread, reinterpret_cast<void*>(this));
        }
    }

    if ( 2 == scan_state_ )
    {
        pthread_join(scan_thread_, NULL);
        scan_state_ = 0;
        int n = ap_count_;
        last_scan_ = millis();
        size_t pos = Menu::pos();
        Menu::menu().clear();
        Menu::menu().set_title("WifiScan");
        for (int i = 0; i < n; ++i) {
            Menu::menu().add(WiFi.SSID(i).c_str(), [](){} );
        }
        Menu::set_pos(pos);
        WiFi.scanDelete();
        ap_count_ = 0;
    }
    Menu::loop();
}

void WifiScan::end()
{
    if ( scan_state_ > 0 )
    {
        // stop thread
        pthread_join(scan_thread_, NULL);

        // throw away results
        WiFi.scanDelete();
    }

    // prevent new threads
    scan_state_ = 4;

    Menu::end();
}
