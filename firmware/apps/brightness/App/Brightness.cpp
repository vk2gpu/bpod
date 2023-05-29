#include "Brightness.hpp"

#include <Images/BpodTitleBar.hpp>
#include <Images/BpodProgressBar.hpp>
#include <stringdb.h>

static int g_bkl_pin = 0;
static uint8_t g_bkl_percent = 0; 

void Brightness::setup(int pin)
{
    g_bkl_pin = pin;
    Brightness::write(100);
}

uint8_t Brightness::read()
{
    if ( !g_bkl_pin )
    {
        return 0;
    }
    return g_bkl_percent;
}

void Brightness::write(uint8_t percent)
{
    if ( percent >= 100 )
    {
        percent == 100;
    }
    g_bkl_percent = percent;
    if ( !g_bkl_pin )
    {
        return;
    }
    analogWrite(g_bkl_pin, g_bkl_percent);
}

void Brightness::begin(void)
{
    redraw_ = true;
    brightness_ = Brightness::read();
}

void Brightness::key_event(uint8_t key)
{
    switch( key )
    {
        case APP_KEY_MENU:
            App::manager_end();
            break;

        case APP_KEY_SCROLL_CLOCKWISE:
            if ( brightness_ <= 90 )
            {
                brightness_ += 10;
            }
            else
            {
                brightness_ = 100;
            }
            break;

        case APP_KEY_SCROLL_ANTICLOCKWISE:
            if ( brightness_ >= 10 )
            {
                brightness_ -= 10;
            }
            else
            {
                brightness_ = 0;
            }
            break;
    }
}

void Brightness::draw(Adafruit_GFX &gfx)
{
    bool force = false;

    if ( redraw_ )
    {
        BpodTitleBar::draw(gfx, STRING(STRING_BRIGHTNESS));
        gfx.fillRect(0, BpodTitleBar::view_y(gfx), gfx.width(), BpodTitleBar::view_height(gfx), 0xffff);
        printf(STRING(CONSOLE_CLEAR));
        STRING_CACHE();
        printf(STRING(FMT_TITLE), STRING(STRING_BRIGHTNESS));
        STRING_CLEAR();
        redraw_ = false;
        force = true;
    }

    if ( force || (brightness_ != Brightness::read() ) )
    {
        Brightness::write(brightness_);
        brightness_ = Brightness::read();
        BpodProgressBar::draw(gfx, 10, 85, 110, 0, brightness_, 100);
        printf(STRING(CONSOLE_CURSOR_PERCENT), 2, 1, brightness_);
    }
}
