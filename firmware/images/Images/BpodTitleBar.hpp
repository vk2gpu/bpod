#pragma once

#include <Adafruit_GFX.h>

#include <Images/BpodTitleBarBackground.hpp>
#include <Images/BpodTitleBarBackgroundDark.hpp>

class BpodTitleBar
{
    public:
        static int16_t height() { return BpodTitleBarBackground::height; }
        static int16_t view_y(Adafruit_GFX &gfx) { return BpodTitleBarBackground::height; };
        static int16_t view_height(Adafruit_GFX &gfx) { return gfx.height() - BpodTitleBarBackground::height; };
        static const GFXfont *font();
        static void draw(Adafruit_GFX &gfx, const std::string &title, bool dark = false) {
            int16_t x1 = 0;
            int16_t y1 = 0;
            uint16_t w = 0;
            uint16_t h = 0;
            gfx.setTextSize(1); gfx.setFont(BpodTitleBar::font()); //gfx.setTextSize(2);
            gfx.getTextBounds(title.c_str(), 0, 0, &x1, &y1, &w, &h);
            if ( dark )
            {
                gfx.setTextColor(0xffff);
                BpodTitleBarBackgroundDark::draw_hline(0, 0, gfx.width(), gfx);
            }
            else
            {
                gfx.setTextColor(0x0000);
                BpodTitleBarBackground::draw_hline(0, 0, gfx.width(), gfx);
            }
            if ( title.size() > 0 )
            {
                x1 = (gfx.width() / 2) - (w / 2);
                y1 = 14; //y1 = (BpodTitleBarBackground::height / 2) - (h / 2);
                gfx.setCursor(x1, y1);
                gfx.print(title.c_str());
            }
            gfx.setFont(nullptr);
        };
};
