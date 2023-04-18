#ifndef TOUCH_WHEEL_H
#define TOUCH_WHEEL_H

#include <Arduino.h>

class TouchWheelPin
{
    public:
        TouchWheelPin(int8_t pin, bool low_is_touch) : pin_(pin), first_read_(true), low_is_touch_(low_is_touch),
            low_(-1), high_(-1), value_(-1), percent_(0) { };
        ~TouchWheelPin() {};
        void begin() {};

        void read();
        uint32_t value() { if ( this->value_ == -1 ) { this->read(); } return this->value_; };
        uint32_t low() { if ( this->value_ == -1 ) { this->read(); } return this->low_; };
        uint32_t high() { if ( this->value_ == -1 ) { this->read(); } return this->high_; };
        int8_t percent() { if ( this->value_ == -1 ) { this->read(); } return this->percent_; };

    private:
        int8_t pin_;
        bool first_read_;
        bool low_is_touch_;
        uint32_t low_;
        uint32_t high_;
        uint32_t value_;
        int8_t percent_;
};

class TouchWheel
{
    public:
        TouchWheel(int8_t deg000_pin, int8_t deg120_pin, int8_t deg240_pin, int8_t ok_pin, bool low_is_touch) :
            deg000_(deg000_pin, low_is_touch), deg120_(deg120_pin, low_is_touch), deg240_(deg240_pin, low_is_touch),
            ok_(ok_pin, low_is_touch), angle_(-1), ok_down_(false), wheel_rotated_(false), down_time_(0), last_read_(0),
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
        bool wheel_rotated_;
        unsigned long down_time_;
        unsigned long last_read_;
        int16_t wheel_clicks_;
        int16_t ok_clicks_;
        int16_t forward_clicks_;
        int16_t menu_clicks_;
        int16_t back_clicks_;
        int16_t play_clicks_;
};

#endif // TOUCH_WHEEL_H
