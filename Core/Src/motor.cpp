#include "motor.h"

void Motor::init()
{
    pin_out_one_.Pin::init();
    pin_out_two_.Pin::init();
    pwm_pin_out_.PwmPin::init();
    is_initialized_ = true;
}

void Motor::softstart(Direction direction , uint16_t step_delay_ms)
{
    if (!is_initialized_){
        Error_Handler();
        return;
    }

    switch (direction)
    {
        case Motor::Direction::CC:
        pin_out_one_.write(true);
        pin_out_two_.write(false);
        break;

        case Motor::Direction::CCW:
        pin_out_one_.write(false);
        pin_out_two_.write(true);
        break;

        default:
        return;
    }

    while ( current_speed_ < 100.0f)
    {
        current_speed_+=1;
        pwm_pin_out_.pwmSetDuty(current_speed_);
        HAL_Delay(step_delay_ms);
    }  
}

void Motor::softstop(uint16_t step_delay_ms)
{
    if (!is_initialized_){
    Error_Handler();
    return;
    }

    while ( current_speed_ > 0.0f)
    {
        current_speed_-=1;
        pwm_pin_out_.pwmSetDuty(current_speed_);
        HAL_Delay(step_delay_ms);
    }
    stop();
}


void Motor::stop()
{
    pin_out_one_.write(false);
    pin_out_two_.write(false);
    pwm_pin_out_.write(false);
    current_speed_ = 0.0f;
}


void Motor::move(Direction direction , float speed_percentage)
{
    if (!is_initialized_){
        Error_Handler();
        return;
    }

    switch (direction)
    {
        case Motor::Direction::CC:
        pin_out_one_.write(true);
        pin_out_two_.write(false);
        break;

        case Motor::Direction::CCW:
        pin_out_one_.write(false);
        pin_out_two_.write(true);
        break;

        default:
        stop();
        return;
    }
    pwm_pin_out_.pwmSetDuty(speed_percentage);
    current_speed_ = speed_percentage;
}
