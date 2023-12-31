#pragma once

#include <App/App.hpp>

#include <Menu/BpodMenu.hpp>

class Menu : public App  {
    public:
        Menu() : redraw_(true) {};
        virtual ~Menu() {};

        virtual void begin(BpodMenu &menu) = 0;

        virtual void begin(void);
        virtual void visible();
        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);
        virtual void end(void);

        size_t pos() const { return menu_.pos(); };
        void set_pos(size_t pos) { menu_.move_to(pos); };
        void set(size_t pos, const std::string &name) { menu_.set(pos, name); }

        BpodMenu &menu() { return menu_; };

    private:
        BpodMenu menu_;
        bool redraw_;
};
