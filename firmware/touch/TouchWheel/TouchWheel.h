#ifndef TOUCH_WHEEL_H
#define TOUCH_WHEEL_H

#include <Arduino.h>

class TouchWheelPin
{
    public:
        TouchWheelPin(int8_t pin, bool low_is_touch) : pin_(pin), low_is_touch_(low_is_touch),
            low_(-1), high_(-1), value_(-1), percent_(0) { };
        ~TouchWheelPin() {};
        void begin() { this->read(); };

        void read();
        int8_t value() { return this->value_; };
        int8_t low() { return this->low_; };
        int8_t high() { return this->high_; };
        int8_t percent() { return this->percent_; };

    private:
        int8_t pin_;
        bool low_is_touch_;
        int8_t low_;
        int8_t high_;
        int8_t value_;
        int8_t percent_;
};

class TouchWheel
{
    public:
        TouchWheel(int8_t deg000_pin, int8_t deg120_pin, int8_t deg240_pin, int8_t ok_pin, bool low_is_touch) :
            deg000_(deg000_pin, low_is_touch), deg120_(deg120_pin, low_is_touch), deg240_(deg240_pin, low_is_touch),
            ok_(ok_pin, low_is_touch), angle_(-1), ok_down_(false), down_time_(0),
            wheel_clicks_(0), ok_clicks_(0), forward_clicks_(0), menu_clicks_(0), back_clicks_(0), play_clicks_(0) {};
        ~TouchWheel() {};
        void begin() { this->deg000_.begin(); this->deg120_.begin(); this->deg240_.begin(); this->ok_.begin(); };

        void read();
        int16_t angle() { return this->angle_; };
        bool wheel_down() { return this->angle() >= 0; };
        bool wheel_up() { return !this->wheel_down(); };
        bool ok_down() { return this->ok_down_; };
        bool ok_up() { return !this->ok_down(); };

        int16_t wheel_pop_clicks() { int16_t clicks = this->wheel_clicks_; this->wheel_clicks_ = 0; return clicks; };
        int16_t ok_pop_clicks() { int16_t clicks = this->ok_clicks_; this->ok_clicks_ = 0; return clicks; };
        int16_t forward_pop_clicks() { int16_t clicks = this->forward_clicks_; this->forward_clicks_ = 0; return clicks; };
        int16_t menu_pop_clicks() { int16_t clicks = this->menu_clicks_; this->menu_clicks_ = 0; return clicks; };
        int16_t back_pop_clicks() { int16_t clicks = this->back_clicks_; this->back_clicks_ = 0; return clicks; };
        int16_t play_pop_clicks() { int16_t clicks = this->play_clicks_; this->play_clicks_ = 0; return clicks; };

    private:
        TouchWheelPin deg000_;
        TouchWheelPin deg120_;
        TouchWheelPin deg240_;
        TouchWheelPin ok_;
        int16_t angle_;
        bool ok_down_;
        unsigned long down_time_;
        int16_t wheel_clicks_;
        int16_t ok_clicks_;
        int16_t forward_clicks_;
        int16_t menu_clicks_;
        int16_t back_clicks_;
        int16_t play_clicks_;
};

#endif // TOUCH_WHEEL_H
