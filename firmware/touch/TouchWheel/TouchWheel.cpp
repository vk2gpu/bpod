#include "TouchWheel.h"

#define PIN_MOST_TOUCHED    (0)

#define PERCENT_TOUCH_UPPER_BOUND   (30)
#define PERCENT_TOUCH_LOWER_BOUND   (20)

void TouchWheelPin::read()
{
    this->value_ = touchRead(this->pin_);
    // cannot figure out why this is an issue on the ESP32-S2, but this "fixes" it
    if ( first_read_ )
    {
        first_read_ = false;
        return;
    }
    if ( this->value_ >= 0 )
    {
        if ( this->low_ == -1 )
        {
            this->low_ = this->value_;
        }
        if ( this->value_ < this->low_ )
        {
            this->low_ = this->value_;
        }
        if ( this->high_ == -1 )
        {
            this->high_ = this->value_;
        }
        if ( this->value_ > this->high_ )
        {
            this->high_ = this->value_;
        }
        uint32_t range = this->high_ - this->low_;
        if ( range < 1000 )
        {
            this->percent_ = 0;
        }
        else
        {
            uint32_t value = this->value_ - this->low_;
            this->percent_ = (int8_t)((value * 100) / range);
            if ( this->low_is_touch_ )
            {
                this->percent_ = 100 - this->percent_;
            }
        }
    }
}

int8_t TouchWheelBetween::percent()
{
    int16_t min_percent = a_.percent();
    int16_t max_percent = b_.percent();
    int16_t tmp;
    if ( min_percent > max_percent )
    {
        tmp = min_percent;
        min_percent = max_percent;
        max_percent = tmp;
    }
    int16_t diff_percent = max_percent - min_percent;
    if ( diff_percent > 50 )
    {
        return 0;
    }
    int16_t similar_percent = 100 - diff_percent;
    int16_t percent = ((min_percent * similar_percent) + (max_percent * similar_percent)) / 100;
    if ( percent > 100 )
    {
        percent = 100;
    }
    return (int8_t)percent;
}

void TouchWheel::read()
{
    size_t pin = 0;
    size_t pin_count = 6;
    int8_t pin_percent[6] = { 0 };
    int8_t pin_percent_most_touched = 0;
    int16_t angles[6] = {
        0,
        60,
        120,
        180,
        240,
        300
    };
    int16_t angle_most_touched = 0;
    bool wheel_set_angle = false;

    this->deg000_.read();
    this->deg120_.read();
    this->deg240_.read();
    this->ok_.read();

    pin_percent[0] = this->deg000_.percent();
    pin_percent[1] = this->deg060_.percent();
    pin_percent[2] = this->deg120_.percent();
    pin_percent[3] = this->deg180_.percent();
    pin_percent[4] = this->deg240_.percent();
    pin_percent[5] = this->deg300_.percent();

    // find the most touched pin
    pin_percent_most_touched = pin_percent[0];
    angle_most_touched = angles[0];
    for ( pin = 1; pin < pin_count; pin++ )
    {
        if ( pin_percent_most_touched < pin_percent[pin] )
        {
            pin_percent_most_touched = pin_percent[pin];
            angle_most_touched = angles[pin];
        }
    }

    // (tiny) state machine for work out if its a wheel touch, ok touch or no touch
    if ( this->wheel_up() && this->ok_up() )
    {
        if ( this->ok_.percent() > pin_percent_most_touched )
        {
            // check if OK is pressed
            if ( this->ok_.percent() > PERCENT_TOUCH_UPPER_BOUND )
            {
                this->ok_down_ = true;
                this->down_time_ = millis();
            }
        }
        else
        {
            // check if WHEEL is pressed
            if ( pin_percent_most_touched > PERCENT_TOUCH_UPPER_BOUND )
            {
                // finger down on wheel *somewhere*
                wheel_set_angle = true;
                this->wheel_rotated_ = false; // not determined yet
                this->down_time_ = millis();
                this->last_read_ = 0; // has not be read yet
            }
        }
    }
    else if ( this->ok_down() )
    {
        if ( this->ok_.percent() < PERCENT_TOUCH_LOWER_BOUND )
        {
            this->ok_down_ = false;
            this->ok_clicks_++;
        }
    }
    else // this->wheel_down()
    {
        if ( pin_percent_most_touched >= PERCENT_TOUCH_LOWER_BOUND )
        {
            // finger down on wheel *somewhere*
            wheel_set_angle = true;
        }
        else
        {
            unsigned long duration = millis() - this->down_time_;
            if ( (duration < 200) && (!this->wheel_rotated_) )
            {
                switch ( this->angle_ )
                {
                    case 0:
                        this->forward_clicks_++;
                        break;
                    case 60:
                    case 120:
                        this->menu_clicks_++;
                        break;
                    case 180:
                        this->back_clicks_++;
                        break;
                    case 240:
                    case 300:
                        this->play_clicks_++;
                        break;
                }
            }
        }
    }

    // detemine the angle of the wheel press
    if ( wheel_set_angle )
    {
        // don't read wheel too quickly
        if ( (0 == this->last_read_) || (( millis() - this->last_read_ ) > 30) )
        {
            this->last_read_ = millis();
            int16_t old_angle = this->angle_;
            this->angle_ = angle_most_touched;
            if ( old_angle == -1 )
            {
                old_angle = this->angle_;
            }
            if ( old_angle != this->angle_ )
            {
                this->wheel_rotated_ = true;
                if ( ( old_angle >= 300 && this->angle_ <= 120 ) || ( old_angle >= 240 && this->angle_ <= 60 ) )
                {
                    // clockwise, postive direction, over the 0 deg line
                    this->wheel_clicks_ -= ( ( this->angle_ + 360 ) - old_angle ) / 60;
                }
                else if ( ( old_angle <= 120 && this->angle_ >= 300 ) || ( old_angle <= 60 && this->angle_ >= 240 ) )
                {
                    // anti-clockwise, negative direction, over the 0 deg line
                    this->wheel_clicks_ -= ( this->angle_ - ( old_angle + 360 ) ) / 60;
                }
                else
                {
                    // easy
                    this->wheel_clicks_ -= ( this->angle_ - old_angle ) / 60;
                }
            }
        }
    }
    else
    {
        this->angle_ = -1;
    }
}
