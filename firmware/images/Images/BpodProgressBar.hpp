#pragma once

#include <Adafruit_GFX.h>

#include <Images/BpodScrollEmpty.hpp>
#include <Images/BpodScrollFill.hpp>

class BpodProgressBar
{
    public:
        static int16_t height() { return BpodScrollFill::height; }
        static void draw(Adafruit_GFX &gfx, int16_t x, int16_t y, int16_t w, size_t selected_first_index, size_t selected_size, size_t total_size) {
            int16_t pre_width = 0;
            int16_t fill_width = 0;
            int16_t post_width = 0;
            int16_t fill_x = 0;
            int16_t post_x = 0;
            pre_width = (int16_t)(((size_t)w * selected_first_index) / total_size);
            if ( pre_width >= w )
            {
                pre_width = w - 4;
            }
            fill_width = (int16_t)(((size_t)w * selected_size) / total_size);
            if ( (pre_width + fill_width) > w )
            {
                fill_width = w - pre_width;
            }
            post_width = w - fill_width - pre_width;
            if ( post_width < 4 )
            {
                post_width = 0;
                fill_width = w - pre_width;
            }
            fill_x = x + pre_width;
            post_x = fill_x + fill_width;
            if ( pre_width > 0 )
            {
                BpodScrollEmpty::draw_hline(x, y, pre_width, gfx);
            }
            if ( fill_width > 0 )
            {
                BpodScrollFill::draw_hline(fill_x, y, fill_width, gfx);
            }
            if ( post_width > 0 )
            {
                BpodScrollEmpty::draw_hline(post_x, y, post_width, gfx);
            }
        };
};
