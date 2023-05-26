#include "I2CDetect.hpp"

#include <Images/BpodTitleBar.hpp>
#include <Fonts/Picopixel.h>
#include <stringdb.h>

void I2CDetect::begin(void)
{
    redraw_ = true;
    memset(detected_, 0, sizeof(detected_));
    memset(prev_detected_, 0, sizeof(prev_detected_));
    Wire.begin();
    scan_addr_ = 0;
    if ( selectable_ && ( addr_ > 0x77 ) )
    {
        addr_ = 0x20;
    }
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

bool I2CDetect::addr_selected(uint8_t addr)
{
    if ( addr >= 128 ) return false;
    return selectable_ && ( addr == addr_ );
}

bool I2CDetect::addr_state_changed(uint8_t addr)
{
    if ( addr >= 128 ) return false;
    if ( ((detected_[addr >> 3] >> (addr & 0x7)) & 0x1) != 
        ((prev_detected_[addr >> 3] >> (addr & 0x7)) & 0x1) )
    {
        return true;
    }
    if ( selectable_ && ( addr_ != prev_addr_ ) )
    {
        if ( ( addr == addr_ ) || ( addr == prev_addr_ ) )
        {
            return true;
        }
    }
    return false;
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
        case APP_KEY_SCROLL_CLOCKWISE:
            if ( selectable_ )
            {
                addr_ = addr_ >= 0x77 ? 0x00 : addr_ + 1;
            }
            break;

        case APP_KEY_SCROLL_ANTICLOCKWISE:
            if ( selectable_ )
            {
                addr_ = addr_ == 0 ? 0x77 : addr_ - 1;
            }
            break;

        case APP_KEY_OK:
            if ( selectable_ )
            {
                App::manager_end();
            }
            break;

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
        prev_addr_ = 0xff; // force a selection update
        BpodTitleBar::draw(gfx, STRING(STRING_I2CDETECT));
        gfx.fillRect(0, BpodTitleBar::view_y(gfx), gfx.width(), BpodTitleBar::view_height(gfx), 0xffff);
        gfx.setTextColor(0x0000);
        gfx.setFont(&Picopixel);
        gfx.setTextSize(1);
        printf(STRING(CONSOLE_CLEAR));
        STRING_CACHE();
        printf(STRING(FMT_TITLE), STRING(STRING_I2CDETECT));
        STRING_CLEAR();

        // column headers
        for ( uint8_t addr = 0; addr < 8; addr++ )
        {
            int16_t x = 26 + 4 + (((addr & 0x7) * 3) * 4);
            int16_t y = 30;
            gfx.setCursor(x, y);
            gfx.printf(STRING(I2CDETECT_GFX_FMT_COLUMN), addr);
        }
        printf(STRING(I2CDETECT_CONSOLE_TAB));
        for ( size_t i = 0; i < 0x10; i++ )
        {
            printf(STRING(I2CDETECT_CONSOLE_FMT_COLUMN), i);
        }
        printf("\n");

        // row headers
        for ( uint8_t addr = 0; addr <= 0x70; addr += 8 )
        {
            int16_t x = 4;
            int16_t y = 40 + (((addr >>  3) * 1) * (6 + 2));
            gfx.setCursor(x, y);
            gfx.printf(STRING(I2CDETECT_GFX_FMT_ROW), addr);
        }
        gfx.setFont(nullptr);
        for ( size_t i = 0; i < 0x80; i += 0x10 )
        {
            printf(STRING(I2CDETECT_CONSOLE_FMT_ROW), i);
        }

        // redraw grid
        memset(prev_detected_, 0xff, sizeof(prev_detected_));
        memset(detected_, 0x00, sizeof(prev_detected_));
    }

    if ( ( 0 != memcmp(detected_, prev_detected_, sizeof(detected_)) ) ||
         ( addr_ != prev_addr_ ) )
    {
        // update grid
        gfx.setFont(&Picopixel);
        gfx.setTextSize(1);
        for ( uint8_t addr = 0; addr <= 0x77; addr++ )
        {
            int16_t x = 26 + (((addr & 0x7) * 3) * 4);
            int16_t y = 40 + (((addr >>  3) * 1) * (6 + 2));
            if ( addr_state_changed(addr) )
            {
                if ( addr_selected(addr) )
                {
                    // selected
                    gfx.fillRect(x - 1, y - 5, 9, 7, 0x051f);
                    gfx.setTextColor(0xffff);
                    printf(STRING(CONSOLE_CURSOR_STRING), 3 + (addr >> 4), 4 + ((addr & 0xf) * 3), "[");
                    printf(STRING(CONSOLE_CURSOR_STRING), 3 + (addr >> 4), 7 + ((addr & 0xf) * 3), "]");
                }
                else
                {
                    // not selected
                    gfx.fillRect(x - 1, y - 5, 9, 7, 0xffff);
                    gfx.setTextColor(0x0000);
                    if ( ( (addr_ + 1) == addr ) && ((addr & 0xf) >= 1) )
                    {
                        // don't write over selected bracked
                    }
                    else
                    {
                        printf(STRING(CONSOLE_CURSOR_STRING), 3 + (addr >> 4), 4 + ((addr & 0xf) * 3), " ");
                    }
                    printf(STRING(CONSOLE_CURSOR_STRING), 3 + (addr >> 4), 7 + ((addr & 0xf) * 3), " ");
                }
                gfx.setCursor(x, y);
                if ( addr_present(addr) || (addr == addr_) )
                {
                    gfx.printf(STRING(FMT_HEX_BYTE_UPPER), addr);
                    printf(STRING(CONSOLE_CURSOR_HEX_BYTE), 3 + (addr >> 4), 5 + ((addr & 0xf) * 3), addr);
                }
                else
                {
                    if ( addr >= 3 )
                    {
                        STRING_CACHE();
                        gfx.print(STRING(I2CDETECT_ADDR_NOT_FOUND));
                        printf(STRING(CONSOLE_CURSOR_STRING), 3 + (addr >> 4), 5 + ((addr & 0xf) * 3), STRING(I2CDETECT_ADDR_NOT_FOUND));
                        STRING_CLEAR();
                    }
                    else
                    {
                        STRING_CACHE();
                        printf(STRING(CONSOLE_CURSOR_STRING), 3 + (addr >> 4), 5 + ((addr & 0xf) * 3), STRING(I2CDETECT_ADDR_NOT_USED));
                        STRING_CLEAR();
                    }
                }
            }
        }
        gfx.setFont(nullptr);
        memcpy(prev_detected_, detected_, sizeof(detected_));
        prev_addr_ = addr_;
    }
}

void I2CDetect::end(void)
{
    Wire.end();
}
