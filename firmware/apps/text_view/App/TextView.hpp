#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>

#include <viewerc.h>

class TextView : public App  {
    public:
        TextView() : draw_gfx(nullptr), scroll_by_page_(true), redraw_(true), redraw_text_(true), scroll_bar_(false),
            text_width_(0), text_height_(0) {};
        virtual ~TextView() {};

        void set_title(const std::string &title) {
            redraw_ = true;
            title_ = title;
        };
        void set_text(const std::string &text) {
            redraw_ = true;
            text_ = text;
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
        void draw_title(Adafruit_GFX &gfx);
        void draw_scroll_bar(Adafruit_GFX &gfx);

        std::string title_;
        std::string text_;
        Adafruit_GFX *draw_gfx;
        bool scroll_by_page_;
        bool redraw_;
        bool redraw_text_;
        bool scroll_bar_;
        size_t text_width_;
        size_t text_height_;
        uint8_t viewerc_[VWRC_CALC_DATA_SIZE(100)];
};
