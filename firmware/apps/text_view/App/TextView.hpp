#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>

#include <viewerc.h>

class TextView : public App  {
    public:
        TextView() : draw_gfx(nullptr), scroll_by_page_(true), redraw_(true), redraw_text_(true),
            scroll_bar_(false), move_to_end_(false), pico_font_(false), text_width_(0), text_height_(0) {};
        virtual ~TextView() {};

        void set_title(const std::string &title) {
            redraw_ = true;
            title_ = title;
        };
        void set_text(const std::string &text) {
            redraw_ = true;
            move_to_end_ = false;
            text_ = text;
        };
        void append_text(const std::string &text, size_t max) {
            if ( text.size() == 0 )
            {
                // easy win
                return;
            }
            if ( text.size() > max )
            {
                text_ = text.substr(text.size() - max, max);
            }
            else
            {
                if ( (text_.size() + text.size()) > max )
                {
                    text_ = text_.substr(text_.size() - (max - text.size()), max - text.size()) + text;
                }
                else
                {
                    text_ += text;
                }
            }
            redraw_ = true;
            move_to_end_ = true;
        };
        void clear() {
            redraw_ = true;
            move_to_end_ = false;
            text_.erase();
        };
        void scroll_by_line() { scroll_by_page_ = false; }
        void scroll_by_page() { scroll_by_page_ = true; }

        virtual void begin(void);
        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);
        virtual void end(void);

        // internal use
        ssize_t read(size_t offset, char *text, size_t text_size);
        void calc_string_view(const char *text, size_t *width, size_t *height);
        void draw_string(size_t x, size_t y, const char *text);

    private:
        std::string title_;
        std::string text_;
        Adafruit_GFX *draw_gfx;
        bool scroll_by_page_;
        bool redraw_;
        bool redraw_text_;
        bool scroll_bar_;
        bool move_to_end_;
        bool pico_font_;
        size_t text_width_;
        size_t text_height_;
        uint8_t viewerc_[VWRC_CALC_DATA_SIZE(100)];
};
