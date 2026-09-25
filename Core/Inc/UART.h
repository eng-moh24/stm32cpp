#pragma once
#include "main.h"
#include "cstring"


class UART
{
    private:
    UART_HandleTypeDef* huart1_;
    
    public:
    UART(UART_HandleTypeDef* huart) : huart1_{huart}{}
    UART() = delete;
    UART(const UART&) = delete;
    UART& operator=(const UART&) = delete;

    HAL_StatusTypeDef send(const char* message);

};