#include "pwmpin.h"


void PwmPin::init()
{
    HAL_TIM_PWM_Start( tim_ , channel_);
}


void PwmPin::write(bool state)
{
    pwmSetDuty( state? 100.0f : 0.0f);
}


void PwmPin::pwmSetDuty(float percentage)
{
    if (percentage < 0.0f) percentage = 0.0f;
    if ( percentage > 100.0f ) percentage = 100.0f;

    uint32_t period = __HAL_TIM_GET_AUTORELOAD(tim_);
    uint32_t pulse = (percentage/100.0f) * period;

    __HAL_TIM_SET_COMPARE( tim_ , channel_ , pulse );
}