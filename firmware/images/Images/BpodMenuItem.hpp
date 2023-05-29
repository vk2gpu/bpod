#pragma once

#include <Adafruit_GFX.h>

#include <Images/BpodMenuItemBackground.hpp>
#include <Images/BpodMenuItemArrow.hpp>

class BpodMenuItem
{
    public:
        static int16_t height() { return BpodMenuItemBackground::height; }
        static const GFXfont *font();
        static void draw(Adafruit_GFX &gfx, int16_t y, int16_t width, const std::string &text, bool selected) {
            int16_t margin = 4;
            int16_t x1 = 0;
            int16_t y1 = 0;
            uint16_t w = 0;
            uint16_t h = 0;
            size_t pos = 0;
            int16_t x = 0;
            if ( selected )
            {
                // selected
                BpodMenuItemBackground::draw_hline(x, y, width, gfx);
                BpodMenuItemArrow::draw(x + width - BpodMenuItemArrow::width - margin, y + 4, gfx);
                gfx.setTextColor(0xffff);
            }
            else
            {
                // unselected
                gfx.fillRect(x, y, width, BpodMenuItemBackground::height, 0xffff);
                gfx.setTextColor(0x0000);
            }
            gfx.setTextSize(1); gfx.setFont(BpodMenuItem::font()); //gfx.setTextSize(2);
            gfx.getTextBounds(text.c_str(), 0, 0, &x1, &y1, &w, &h);
            x1 = margin;
            y1 = y + 14;//( (BpodMenuItemBackground::height / 2) - (h / 2) );
            gfx.setCursor(x1, y1);
            gfx.print(text.c_str());
            gfx.setFont(nullptr);
        };
    static void draw(Adafruit_GFX &gfx, int16_t y, int16_t width, size_t row, const std::string &text, bool selected) {
        size_t new_y = (size_t)y + ((size_t)height() * row);
        if ( ( new_y + (size_t)height() ) > (size_t)gfx.height() ) return;
        draw(gfx, (int16_t)new_y, width, text, selected);
    }
};
