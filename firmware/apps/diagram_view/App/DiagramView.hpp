#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>

struct PinLabel {
    uint8_t pin;
    bool active_low;
    std::string label;
    uint16_t fg;
    uint16_t bg;
    bool show;
    bool other_active_low;
    std::string other_label;
    uint16_t other_fg;
    uint16_t other_bg;
    bool show_wiring;
};

class DiagramView : public App  {
    public:
        DiagramView() : redraw_(true) { clear(); };
        virtual ~DiagramView() {};

        void set_title(const std::string &title) {
            redraw_ = true;
            title_ = title;
        };
        void set_other_name(const std::string &other_name) {
            other_name_ = other_name;
            redraw_ = true;
        };
        void add_gnd_label() { label_[0].show = true; redraw_ = true; };
        void add_3v3_label() { label_[11].show = true; redraw_ = true; };
        void add_pin_label(uint8_t pin, const std::string &label, uint16_t fg, uint16_t bg);
        void add_wire_gnd() { label_[0].show_wiring = true; redraw_ = true; };
        void add_wire_3v3() { label_[11].show_wiring = true; redraw_ = true; };
        void add_wire(uint8_t pin, const std::string &other_label, uint16_t other_fg, uint16_t other_bg);

        void clear();

        virtual void begin(void);
        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);
        virtual void end(void);

    private:
        void draw_pin_label(Adafruit_GFX &gfx, size_t index, const PinLabel &pin);
        void draw_wire(Adafruit_GFX &gfx, size_t index, const PinLabel &wire);

        std::string title_;
        std::string other_name_;
        bool redraw_;
        PinLabel label_[12];
};
