#include "UART.h"



HAL_StatusTypeDef UART::send(const char * message)
{

    HAL_StatusTypeDef status = HAL_UART_Transmit( huart1_ , (uint8_t*)message, std::strlen(message) , 100 );
    if(status != HAL_OK)
    {
      HAL_UART_Transmit( huart1_ , (uint8_t*)message, std::strlen(message) , 100 );  
    }
    return status;
}