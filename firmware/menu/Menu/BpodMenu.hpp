#pragma once

#include <string>
#include <vector>
#include <functional>
#include <Adafruit_GFX.h>

#include <Images/BpodTitleBar.hpp>
#include <Images/BpodScrollBar.hpp>
#include <Images/BpodMenuItem.hpp>

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
        bool scroll_bar_visible(Adafruit_GFX &gfx) const { return view_row_count(gfx) < name_.size(); };
        int16_t view_row_count(Adafruit_GFX &gfx) const { return BpodTitleBar::view_height(gfx) / BpodMenuItem::height(); };
        void redraw(Adafruit_GFX &gfx) {
            redraw_ = true;
            draw(gfx);
        };
        void draw(Adafruit_GFX &gfx) {
            size_t row = 0;
            size_t pos = 0;
            int16_t view_width = 0;
            int16_t empty_height = 0;

            // redraw full
            if ( redraw_ )
            {
                // draw all the components for the menu
                BpodTitleBar::draw(gfx, title_);
                view_width = scroll_bar_visible(gfx) ? BpodScrollBar::view_width(gfx) : gfx.width();
                for ( row = 0; row < view_row_count(gfx); row++ )
                {
                    pos = view_ + row;
                    if ( pos >= name_.size() ) break;
                    BpodMenuItem::draw(gfx, BpodTitleBar::view_y(gfx), view_width, row, name_[pos], pos == pos_);
                }
                if ( scroll_bar_visible(gfx) )
                {
                    BpodScrollBar::draw(gfx, BpodTitleBar::view_y(gfx), view_, view_row_count(gfx), name_.size());
                }
                empty_height = BpodTitleBar::view_height(gfx) - (row * BpodMenuItem::height());
                if ( empty_height )
                {
                    gfx.fillRect(0, gfx.height() - empty_height, view_width, empty_height, 0xffff);
                }

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
                view_width = scroll_bar_visible(gfx) ? BpodScrollBar::view_width(gfx) : gfx.width();

                // up all entries (our view of the menu has moved)
                for ( row = 0; row < view_row_count(gfx); row++ )
                {
                    pos = view_ + row;
                    if ( pos >= name_.size() ) break;
                    BpodMenuItem::draw(gfx, BpodTitleBar::view_y(gfx), view_width, row, name_[pos], pos == pos_);
                }
                if ( scroll_bar_visible(gfx) )
                {
                    BpodScrollBar::draw(gfx, BpodTitleBar::view_y(gfx), view_, view_row_count(gfx), name_.size());
                }
                empty_height = BpodTitleBar::view_height(gfx) - (row * BpodMenuItem::height());
                if ( empty_height )
                {
                    gfx.fillRect(0, gfx.height() - empty_height, view_width, empty_height, 0xffff);
                }
            }
            else
            {
                view_width = scroll_bar_visible(gfx) ? BpodScrollBar::view_width(gfx) : gfx.width();

                // move selection in current view
                if ( prev_pos_ != pos_ )
                {
                    // un-select previous row
                    if ( prev_pos_ < name_.size() )
                    {
                        row = prev_pos_ - view_;
                        BpodMenuItem::draw(gfx, BpodTitleBar::view_y(gfx), view_width, row, name_[prev_pos_], false);
                    }

                    // select row
                    if ( pos_ < name_.size() )
                    {
                        row = pos_ - view_;
                        BpodMenuItem::draw(gfx, BpodTitleBar::view_y(gfx), view_width, row, name_[pos_], true);
                    }
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
