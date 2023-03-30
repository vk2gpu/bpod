#pragma once

#include <Adafruit_GFX.h>

#include <Images/BpodScrollEmpty.hpp>
#include <Images/BpodScrollFill.hpp>

class BpodScrollBar
{
    public:
        static int16_t width() { return BpodScrollFill::width; }
        static int16_t view_x(Adafruit_GFX &gfx) { return 0; };
        static int16_t view_width(Adafruit_GFX &gfx) { return gfx.width() - BpodScrollFill::width; };
        static void draw(Adafruit_GFX &gfx, int16_t y, size_t selected_first_index, size_t selected_size, size_t total_size) {
            int16_t pre_height = 0;
            int16_t fill_height = 0;
            int16_t post_height = 0;
            int16_t pre_y = 0;
            int16_t fill_y = 0;
            int16_t post_y = 0;
            int16_t scroll_bar_x = gfx.width() - BpodScrollFill::width;
            int16_t scroll_bar_height = gfx.height() - y;
            pre_height = (int16_t)(((size_t)scroll_bar_height * selected_first_index) / total_size);
            if ( pre_height >= scroll_bar_height )
            {
                pre_height = scroll_bar_height - 4;
            }
            fill_height = (int16_t)(((size_t)scroll_bar_height * selected_size) / total_size);
            if ( (pre_height + fill_height) > scroll_bar_height )
            {
                fill_height = scroll_bar_height - pre_height;
            }
            post_height = scroll_bar_height - fill_height - pre_height;
            if ( post_height < 4 )
            {
                post_height = 0;
                fill_height = scroll_bar_height - pre_height;
            }
            pre_y = y;
            fill_y = pre_y + pre_height;
            post_y = fill_y + fill_height;
            if ( pre_height > 0 )
            {
                BpodScrollEmpty::draw(scroll_bar_x, pre_y, pre_height, gfx);
            }
            if ( fill_height > 0 )
            {
                BpodScrollFill::draw(scroll_bar_x, fill_y, fill_height, gfx);
            }
            if ( post_height > 0 )
            {
                BpodScrollEmpty::draw(scroll_bar_x, post_y, post_height, gfx);
            }
        };
};
