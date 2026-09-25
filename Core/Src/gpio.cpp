#include "gpio.h"

void Pin::init()
{
    if (port_ == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (port_ == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (port_ == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin_;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-Pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port_, &GPIO_InitStruct);
}


void Pin::write(bool state) 
{
    HAL_GPIO_WritePin(port_, pin_, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}