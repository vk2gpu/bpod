#pragma once

#include <string>
#include <vector>
#include <functional>
#include <Adafruit_GFX.h>

#include <Images/BpodTitleBar.hpp>
#include <Images/BpodMenuBar.hpp>
#include <Images/BpodMenuArrow.hpp>
#include <Images/BpodScrollEmpty.hpp>
#include <Images/BpodScrollFill.hpp>


class BpodMenu
{
    public:
        BpodMenu() : redraw_(true), pos_(0), prev_pos_(0), view_(0) {};
        ~BpodMenu() {};
        void clear() {
            redraw_ = true;
            title_ = "";
            pos_ = 0;
            prev_pos_ = 0;
            view_ = 0;
            name_.clear();
            action_.clear();
        };
        void set_title(const std::string &title) {
            redraw_ = true;
            title_ = title;
        };
        void add(const std::string &name, const std::function<void(void)> &action) {
            redraw_ = true;
            name_.push_back(name);
            action_.push_back(action);
        };
        size_t size() const { return name_.size(); }
        void move_to(size_t pos) {
            if ( name_.size() == 0 )
            {
                prev_pos_ = 0;
                pos_ = 0;
                view_ = 0;
                return;
            }
            pos_ = pos >= name_.size() ? name_.size() - 1 : pos;
        };
        void move_clicks(int16_t clicks) {
            if ( 0 == clicks ) {
                return;
            }
            if ( clicks > 0 ) {
                move_to(pos_ + clicks);
            } else {
                clicks *= -1;
                if ( pos_ > clicks ) {
                    move_to(pos_ - clicks);
                } else {
                    move_to(0);
                }
            }
        }
        void move_up() { move_clicks(-1); };
        void move_down() { move_clicks(1); };
        void click() {
            if ( pos_ >= action_.size() )
            {
                return;
            }
            action_[pos_]();
        }
        int16_t scroll_bar_x(Adafruit_GFX &gfx) const { return gfx.width() - scroll_bar_width(gfx); };
        int16_t scroll_bar_y(Adafruit_GFX &gfx) const { return BpodTitleBar::height; };
        int16_t scroll_bar_width(Adafruit_GFX &gfx) const { return BpodScrollEmpty::width; };
        int16_t scroll_bar_height(Adafruit_GFX &gfx) const { return gfx.height() - scroll_bar_y(gfx); };
        bool scroll_bar_visible(Adafruit_GFX &gfx) const { return view_row_count(gfx) < name_.size(); };
        int16_t view_x(Adafruit_GFX &gfx) const { return 0; };
        int16_t view_y(Adafruit_GFX &gfx) const { return BpodTitleBar::height; };
        int16_t view_width(Adafruit_GFX &gfx) const { return gfx.width() - ( scroll_bar_visible(gfx) ? scroll_bar_width(gfx) : 0); }
        int16_t view_height(Adafruit_GFX &gfx) const { return (gfx.height() - view_y(gfx)); };
        int16_t view_row_x(Adafruit_GFX &gfx) { return view_x(gfx); };
        int16_t view_row_y(Adafruit_GFX &gfx, size_t row) { return view_y(gfx) + (view_row_height(gfx) * row); };
        int16_t view_row_width(Adafruit_GFX &gfx) const { return view_width(gfx); };
        int16_t view_row_height(Adafruit_GFX &gfx) const { return BpodMenuBar::height; };
        int16_t view_row_count(Adafruit_GFX &gfx) const { return view_height(gfx) / view_row_height(gfx); };
        void draw_title(Adafruit_GFX &gfx)
        {
            int16_t x1 = 0;
            int16_t y1 = 0;
            uint16_t w = 0;
            uint16_t h = 0;
            gfx.setTextColor(0x0000);
            gfx.setTextSize(2);
            gfx.getTextBounds(title_.c_str(), 0, 0, &x1, &y1, &w, &h);
            BpodTitleBar::draw(0, 0, gfx.width(), gfx);
            if ( title_.size() > 0 )
            {
                x1 = (gfx.width() / 2) - (w / 2);
                y1 = (BpodTitleBar::height / 2) - (h / 2);
                gfx.setCursor(x1, y1);
                gfx.print(title_.c_str());
            }
        }
        void draw_scroll_bar(Adafruit_GFX &gfx) {
            int16_t pre_height = 0;
            int16_t fill_height = 0;
            int16_t post_height = 0;
            int16_t pre_y = 0;
            int16_t fill_y = 0;
            int16_t post_y = 0;
            if ( !scroll_bar_visible(gfx) )
            {
                return;
            }
            pre_height = (scroll_bar_height(gfx) * view_) / name_.size();
            if ( pre_height >= scroll_bar_height(gfx) )
            {
                pre_height = scroll_bar_height(gfx) - 4;
            }
            fill_height = (scroll_bar_height(gfx) * view_row_count(gfx)) / name_.size();
            if ( (pre_height + fill_height) > scroll_bar_height(gfx) )
            {
                fill_height = scroll_bar_height(gfx) - pre_height;
            }
            post_height = scroll_bar_height(gfx) - fill_height - pre_height;
            if ( post_height < 4 )
            {
                post_height = 0;
                fill_height = scroll_bar_height(gfx) - pre_height;
            }
            pre_y = scroll_bar_y(gfx);
            fill_y = pre_y + pre_height;
            post_y = fill_y + fill_height;
            if ( pre_height > 0 )
            {
                BpodScrollEmpty::draw(scroll_bar_x(gfx), pre_y, pre_height, gfx);
            }
            if ( fill_height > 0 )
            {
                BpodScrollFill::draw(scroll_bar_x(gfx), fill_y, fill_height, gfx);
            }
            if ( post_height > 0 )
            {
                BpodScrollEmpty::draw(scroll_bar_x(gfx), post_y, post_height, gfx);
            }
        }
        void draw_row(Adafruit_GFX &gfx, size_t row) {
            int16_t margin = 4;
            int16_t x1 = 0;
            int16_t y1 = 0;
            uint16_t w = 0;
            uint16_t h = 0;
            size_t pos = 0;
            if ( row >= view_row_count(gfx) )
            {
                // off screen, don't draw
                return;
            }
            pos = view_ + row;
            if ( pos >= name_.size() )
            {
                // empty row
                gfx.fillRect(view_row_x(gfx), view_row_y(gfx, row), view_row_width(gfx), view_row_height(gfx), 0xffff);
                return;
            }
            if ( pos == pos_ )
            {
                // selected
                BpodMenuBar::draw(view_row_x(gfx), view_row_y(gfx, row), view_row_width(gfx), gfx);
                BpodMenuArrow::draw(view_row_x(gfx) + view_row_width(gfx) - BpodMenuArrow::width - margin, view_row_y(gfx, row) + 4, gfx);
                gfx.setTextColor(0xffff);
            }
            else
            {
                // unselected
                gfx.fillRect(view_row_x(gfx), view_row_y(gfx, row), view_row_width(gfx), view_row_height(gfx), 0xffff);
                gfx.setTextColor(0x0000);
            }
            gfx.setTextSize(2);
            gfx.getTextBounds(name_[pos].c_str(), 0, 0, &x1, &y1, &w, &h);
            x1 = margin;
            y1 = view_row_y(gfx, row) + ( (view_row_height(gfx) / 2) - (h / 2) );
            gfx.setCursor(x1, y1);
            gfx.print(name_[pos].c_str());
        }
        void redraw(Adafruit_GFX &gfx) {
            redraw_ = true;
            draw(gfx);
        };
        void draw(Adafruit_GFX &gfx) {
            size_t row = 0;

            // redraw full
            if ( redraw_ )
            {
                // draw all the components for the menu
                gfx.fillScreen(0xffff);
                draw_title(gfx);
                for ( row = 0; row < view_row_count(gfx); row++ )
                {
                    draw_row(gfx, row);
                }
                draw_scroll_bar(gfx);

                // update state
                prev_pos_ = pos_;
                redraw_ = false;

                // done
                return;
            }

            // shift the view of the menu (based on the selected entry)
            size_t prev_view = view_;
            if ( pos_ < view_ )
            {
                view_ = pos_;
            }
            if ( pos_ >= ( view_ + view_row_count(gfx) ) )
            {
                // 6 >= 0 + 7 (do nothing)
                // 7 >= 0 + 7 -> view = (7 - 7 + 1) -> view = 1
                // 8 >= 0 + 7 -> view = (8 - 7 + 1) -> view = 2
                view_ = pos_ - view_row_count(gfx) + 1;
            }

            // update view (or just move the selection)
            if ( prev_view != view_ )
            {
                // up all entries (our view of the menu has moved)
                for ( row = 0; row < view_row_count(gfx); row++ )
                {
                    draw_row(gfx, row);
                }
                draw_scroll_bar(gfx);
            }
            else
            {
                // move selection in current view
                if ( prev_pos_ != pos_ )
                {
                    // un-select previous row
                    draw_row(gfx, prev_pos_ - view_);

                    // select row
                    draw_row(gfx, pos_ - view_);
                }
            }

            // update state
            prev_pos_ = pos_;
            redraw_ = false;
        };
    private:
        bool redraw_;
        size_t pos_;
        size_t prev_pos_;
        size_t view_;
        size_t prev_view_;
        std::string title_;
        std::vector<std::string> name_;
        std::vector<std::function<void(void)>> action_;
};
