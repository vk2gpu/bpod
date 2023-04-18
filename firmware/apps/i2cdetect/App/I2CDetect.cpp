#include "I2CDetect.hpp"

#include <Images/BpodTitleBar.hpp>

#include <Fonts/Picopixel.h>

void I2CDetect::begin(void)
{
    redraw_ = true;
    memset(detected_, 0, sizeof(detected_));
    memset(prev_detected_, 0, sizeof(prev_detected_));
    Wire.begin();
    scan_addr_ = 0;
}

void I2CDetect::set_addr_state(uint8_t addr, bool present)
{
    if ( addr >= 128 ) return;
    if ( present )
    {
        detected_[addr >> 3] |= 1 << (addr & 0x7);
    }
    else
    {
        detected_[addr >> 3] &=~ (1 << (addr & 0x7));
    }
}

bool I2CDetect::addr_present(uint8_t addr)
{
    if ( addr >= 128 ) return false;
    return ((detected_[addr >> 3] >> (addr & 0x7)) & 0x1) != 0;
}

bool I2CDetect::addr_state_changed(uint8_t addr)
{
    if ( addr >= 128 ) return false;
    return ((detected_[addr >> 3] >> (addr & 0x7)) & 0x1) != 
        ((prev_detected_[addr >> 3] >> (addr & 0x7)) & 0x1);
}

void I2CDetect::loop(void)
{
    if ( scan_addr_ < 3 ) scan_addr_ = 3;
    if ( scan_addr_ > 0x77) scan_addr_ = 3;

    Wire.beginTransmission(scan_addr_);
    uint8_t error = Wire.endTransmission();
    if ( error == 0 )
    {
        set_addr_state(scan_addr_, true);
    }
    else if ( error != 2 )
    {
        // error - just say not detected
        set_addr_state(scan_addr_, false);
    }
    else
    {
        set_addr_state(scan_addr_, false);
    }

    scan_addr_++;
}

void I2CDetect::key_event(uint8_t key)
{
    switch( key )
    {
        case APP_KEY_MENU:
            App::manager_end();
            break;
    }
}

void I2CDetect::draw(Adafruit_GFX &gfx)
{
    if ( redraw_ )
    {
        redraw_ = false;
        BpodTitleBar::draw(gfx, "i2cdetect");
        gfx.fillRect(0, BpodTitleBar::view_y(gfx), gfx.width(), BpodTitleBar::view_height(gfx), 0xffff);
        gfx.setTextColor(0x0000);
        gfx.setFont(&Picopixel);
        gfx.setTextSize(1);

        // column headers
        for ( uint8_t addr = 0; addr < 8; addr++ )
        {
            int16_t x = 26 + 4 + (((addr & 0x7) * 3) * 4);
            int16_t y = 30;
            gfx.setCursor(x, y);
            gfx.printf("%01X", addr);
        }

        // row headers
        for ( uint8_t addr = 0; addr <= 0x70; addr += 8 )
        {
            int16_t x = 4;
            int16_t y = 40 + (((addr >>  3) * 1) * (6 + 2));
            gfx.setCursor(x, y);
            gfx.printf("%02X", addr);
        }
        gfx.setFont(nullptr);

        // redraw grid
        memset(prev_detected_, 0xff, sizeof(prev_detected_));
        memset(detected_, 0x00, sizeof(prev_detected_));
    }

    if ( 0 != memcmp(detected_, prev_detected_, sizeof(detected_)))
    {
        // update grid
        gfx.setFont(&Picopixel);
        gfx.setTextSize(1);
        gfx.setTextColor(0x0000);
        for ( uint8_t addr = 3; addr <= 0x77; addr++ )
        {
            int16_t x = 26 + (((addr & 0x7) * 3) * 4);
            int16_t y = 40 + (((addr >>  3) * 1) * (6 + 2));
            if ( addr_state_changed(addr) )
            {
                gfx.fillRect(x - 1, y - 5, 9, 7, 0xffff);
                gfx.setCursor(x, y);
                if ( addr_present(addr) )
                {
                    gfx.printf("%02X", addr);
                }
                else
                {
                    gfx.print("--");
                }
            }
        }
        gfx.setFont(nullptr);
        memcpy(prev_detected_, detected_, sizeof(detected_));
    }
}

void I2CDetect::end(void)
{
    Wire.end();
}
