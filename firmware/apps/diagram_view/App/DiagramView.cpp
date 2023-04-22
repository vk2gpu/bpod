#include "DiagramView.hpp"

#include <Images/BpodTitleBar.hpp>
#include <Images/BpodScrollBar.hpp>

#include <Fonts/Picopixel.h>

#define PIN_COUNT       12
#define PIN_TOP_Y       17
#define PIN_BOTTOM_Y    145
#define PIN_Y(index)    ((((PIN_BOTTOM_Y - PIN_TOP_Y) * (index)) / (PIN_COUNT - 1)) + PIN_TOP_Y)
#define PIN_LABEL_PIXEL_HEIGHT  10
#define PIN_LABEL_PIXEL_WIDTH   5

void DiagramView::begin()
{
    redraw_ = true;
}

void DiagramView::clear()
{
    this->other_name_ = "other";
    for ( size_t i = 0; i < PIN_COUNT; i++ )
    {
        label_[i].pin = 0;
        label_[i].active_low = false;
        label_[i].label = "";
        label_[i].fg = 0xffff;
        label_[i].bg = 0x0000;
        label_[i].show = false;
        label_[i].other_active_low = false;
        label_[i].other_label = label_[i].label;
        label_[i].other_fg = label_[i].fg;
        label_[i].other_bg = label_[i].bg;
        label_[i].show_wiring = false;
    }
    label_[0].pin = 0xff;
    label_[0].label = "GND";
    label_[0].fg = 0xffff;
    label_[0].bg = 0x0000;
    label_[0].other_label = label_[0].label;
    label_[0].other_fg = label_[0].fg;
    label_[0].other_bg = label_[0].bg;
    label_[1].pin = 5;
    label_[2].pin = 6;
    label_[3].pin = 7;
    label_[4].pin = 8;
    label_[5].pin = 9;
    label_[6].pin = 37;
    label_[7].pin = 36;
    label_[8].pin = 35;
    label_[9].pin = 17;
    label_[10].pin = 18;
    label_[11].pin = 0xff;
    label_[11].label = "3V";
    label_[11].fg = 0xffff;
    label_[11].bg = 0xf800;
    label_[11].other_label = label_[11].label;
    label_[11].other_fg = label_[11].fg;
    label_[11].other_bg = label_[11].bg;
}

void DiagramView::add_pin_label(uint8_t pin, const std::string &label, uint16_t fg, uint16_t bg)
{
    for ( size_t i = 0; i < PIN_COUNT; i++ )
    {
        if ( label_[i].pin == pin )
        {
            if ( label[0] == '~' )
            {
                label_[i].label = label.c_str() + 1;
                label_[i].active_low = true;
            }
            else
            {
                label_[i].label = label;
            }
            label_[i].fg = fg;
            label_[i].bg = bg;
            label_[i].show = true;
            redraw_ = true;
            break;
        }
    }
}

void DiagramView::add_wire(uint8_t pin, const std::string &other_label, uint16_t other_fg, uint16_t other_bg)
{
    for ( size_t i = 0; i < PIN_COUNT; i++ )
    {
        if ( label_[i].pin == pin )
        {
            if ( other_label[0] == '~' )
            {
                label_[i].other_label = other_label.c_str() + 1;
                label_[i].other_active_low = true;
            }
            else
            {
                label_[i].other_label = other_label;
            }
            label_[i].other_fg = other_fg;
            label_[i].other_bg = other_bg;
            label_[i].show_wiring = true;
            redraw_ = true;
            break;
        }
    }
}

void DiagramView::key_event(uint8_t key)
{
    switch( key )
    {
        case APP_KEY_MENU:
            App::manager_end();
            break;
    }
}

void DiagramView::draw_pin_label(Adafruit_GFX &gfx, size_t index, const PinLabel &pin)
{
    int16_t x1 = gfx.width();
    int16_t y1 = PIN_Y(index);
    int16_t x0 = x1 - PIN_LABEL_PIXEL_WIDTH;
    int16_t y0 = y1 - (PIN_LABEL_PIXEL_HEIGHT / 2);
    int16_t x2 = x1 - PIN_LABEL_PIXEL_WIDTH;
    int16_t y2 = y1 + (PIN_LABEL_PIXEL_HEIGHT / 2);
    gfx.fillTriangle(x0, y0, x1, y1, x2, y2, pin.bg);
    int16_t width = 6 * pin.label.size();
    x0 = x0 - width - 1;
    gfx.fillRect(x0, y0, x2 - x0 + 1, y2 - y0 + 1, pin.bg);
    gfx.setTextColor(pin.fg, pin.bg);
    gfx.setTextSize(1);
    gfx.setCursor(x0 + 2, y0 + 2);
    if ( pin.active_low )
    {
        gfx.drawLine(x0 + 1, y0 - 1, x0 + width + 1, y0 - 1, pin.bg);
        gfx.drawLine(x0 + 1, y0, x0 + width + 1, y0, pin.fg);
    }
    gfx.print(pin.label.c_str());
}

void DiagramView::draw_wire(Adafruit_GFX &gfx, size_t index, const PinLabel &wire)
{
    int16_t width = 6 * wire.label.size();
    int16_t x0 = 1;
    int16_t y0 = 31 + (index * (PIN_LABEL_PIXEL_HEIGHT + 2));
    int16_t x2 = x0 + width + 1;
    int16_t y2 = y0 + PIN_LABEL_PIXEL_HEIGHT;
    int16_t x1 = x2 + PIN_LABEL_PIXEL_WIDTH;
    int16_t y1 = y0 + (PIN_LABEL_PIXEL_HEIGHT / 2);
    gfx.fillRect(x0, y0, x2 - x0 + 1, y2 - y0 + 1, wire.bg);
    gfx.fillTriangle(x2, y0, x1, y1, x2, y2, wire.bg);
    gfx.setTextColor(wire.fg, wire.bg);
    gfx.setTextSize(1);
    gfx.setCursor(x0 + 2, y0 + 2);
    if ( wire.active_low )
    {
        gfx.drawLine(x0 + 1, y0 - 1, x0 + width + 1, y0 - 1, wire.bg);
        gfx.drawLine(x0 + 1, y0, x0 + width + 1, y0, wire.fg);
    }
    gfx.print(wire.label.c_str());
    int16_t wire_x0 = x1;
    int16_t wire_y = y1;
    width = 6 * wire.other_label.size();
    x2 = 90;
    x0 = x2 - width - 2;
    x1 = x0 - PIN_LABEL_PIXEL_WIDTH;
    gfx.fillRect(x0, y0, x2 - x0 + 1, y2 - y0 + 1, wire.other_bg);
    gfx.fillTriangle(x0, y0, x1, y1, x0, y2, wire.other_bg);
    gfx.setTextColor(wire.other_fg, wire.other_bg);
    gfx.setTextSize(1);
    gfx.setCursor(x0 + 2, y0 + 2);
    if ( wire.other_active_low )
    {
        gfx.drawLine(x0 + 1, y0 - 1, x0 + width + 1, y0 - 1, wire.other_bg);
        gfx.drawLine(x0 + 1, y0, x0 + width + 1, y0, wire.other_fg);
    }
    gfx.print(wire.other_label.c_str());
    int16_t wire_x2 = x1;
    int16_t wire_x1 = wire_x0 + ((wire_x2 - wire_x0) / 2);
    gfx.drawLine(wire_x0, wire_y, wire_x1, wire_y, wire.bg);
    gfx.drawLine(wire_x1, wire_y, wire_x2, wire_y, wire.other_bg);
}

void DiagramView::draw(Adafruit_GFX &gfx)
{
    if ( redraw_ )
    {
        redraw_ = false;
        BpodTitleBar::draw(gfx, title_);
        gfx.fillRect(0, BpodTitleBar::view_y(gfx), gfx.width(), BpodTitleBar::view_height(gfx), 0xffff);
        for ( size_t i = 0; i < PIN_COUNT; i++ )
        {
            if ( label_[i].show )
            {
                draw_pin_label(gfx, i, label_[i]);
            }
        }
        size_t w = 0;
        for ( size_t i = 0; i < PIN_COUNT; i++ )
        {
            if ( label_[i].show_wiring )
            {
                draw_wire(gfx, w, label_[i]);
                w++;
            }
        }
        if ( w > 0 )
        {
            gfx.setTextColor(0x0000, 0xffff);
            gfx.setTextSize(1);
            gfx.setCursor(2, 22);
            gfx.print("bPod");
            gfx.setTextColor(0x0000, 0xffff);
            gfx.setTextSize(1);
            int16_t x = 60;
            if ( other_name_.size() > 7 )
            {
                x -= 4 * (other_name_.size() - 7);
            }
            gfx.setCursor(x, 22);
            gfx.print(other_name_.c_str());
        }
    }
}

void DiagramView::end(void)
{

}
