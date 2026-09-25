#pragma once
#include "main.h"
#include "gpio.h"


class PwmPin : public Pin
{
    private:
    TIM_HandleTypeDef *tim_;
    uint16_t channel_;

    public:

    PwmPin( TIM_HandleTypeDef *tim , uint16_t channel) : Pin(nullptr , 0) , tim_{tim} , channel_{channel}{}
    void init() override;
    void write(bool state) override;
    void pwmSetDuty(float percentage);
    


};