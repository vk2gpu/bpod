#include "TouchWheel.h"

#define PIN_MOST_TOUCHED    (0)

void TouchWheelPin::read()
{
    this->value_ = touchRead(this->pin_);
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
        int16_t range = this->high_ - this->low_;
        if ( range < 30 )
        {
            this->percent_ = 0;
        }
        else
        {
            int16_t value = this->value_ - this->low_;
            this->percent_ = (int8_t)((value * 100) / range);
            if ( this->low_is_touch_ )
            {
                this->percent_ = 100 - this->percent_;
            }
        }
    }
}

void TouchWheel::read()
{
    size_t pin = 0;
    size_t pin_count = 3;
    TouchWheelPin *pins[3] = { &this->deg000_, &this->deg120_, &this->deg240_ };
    TouchWheelPin *tmp_pin = nullptr;
    int16_t angles[3] = { 0, 120, 240 };
    int16_t tmp_angle = 0;
    bool swapped = true;
    bool wheel_set_angle = false;

    this->deg000_.read();
    this->deg120_.read();
    this->deg240_.read();
    this->ok_.read();

    // order the pins from most touched to least touched
    while ( swapped )
    {
        swapped = false;
        for ( pin = 0; pin < (pin_count - 1); pin++ )
        {
            if ( pins[pin]->percent() < pins[pin + 1]->percent() )
            {
                tmp_pin = pins[pin];
                tmp_angle = angles[pin];
                pins[pin] = pins[pin + 1];
                angles[pin] = angles[pin + 1];
                pins[pin + 1] = tmp_pin;
                angles[pin + 1] = tmp_angle;
                swapped = true;
            }
        }
    }

    // (tiny) state machine for work out if its a wheel touch, ok touch or no touch
    if ( this->wheel_up() && this->ok_up() )
    {
        if ( this->ok_.percent() > pins[PIN_MOST_TOUCHED]->percent() )
        {
            // check if OK is pressed
            if ( this->ok_.percent() > 50 )
            {
                this->ok_down_ = true;
                this->down_time_ = millis();
            }
        }
        else
        {
            // check if WHEEL is pressed
            if ( pins[PIN_MOST_TOUCHED]->percent() > 50 )
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
        if ( this->ok_.percent() < 50 )
        {
            this->ok_down_ = false;
            this->ok_clicks_++;
        }
    }
    else // this->wheel_down()
    {
        if ( pins[PIN_MOST_TOUCHED]->percent() > 50 )
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
            if ( ( pins[0]->percent() - pins[1]->percent() ) < 30 )
            {
                if ( ( angles[0] == 0 ) && ( angles[1] == 240 ) )
                {
                    angles[0] = 360;
                }
                if ( ( angles[1] == 0 ) && ( angles[0] == 240 ) )
                {
                    angles[1] = 360;
                }
                if ( angles[0] < angles[1] )
                {
                    this->angle_ = angles[0] + 60;
                }
                else
                {
                    this->angle_ = angles[0] - 60;
                }
            }
            else
            {
                this->angle_ = angles[0];
            }
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
