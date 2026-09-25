/*
 * cpp_main.cpp
 *
 *  Created on: Sep 13, 2026
 *      Author: engmoh
 */
#include <stdio.h>
#include <cmath>
#include <array>
#include "cpp_main.h"
#include "gpio.h"
#include "pwmpin.h"
#include "motor.h"
#include "mpu.h"
#include "PID.h"
#include "UART.h"

extern "C" void initialise_monitor_handles(void);
extern "C" {
    #include "main.h"
    extern TIM_HandleTypeDef htim1;
    extern TIM_HandleTypeDef htim2;
    extern I2C_HandleTypeDef hi2c1;
    extern UART_HandleTypeDef huart1;
}



MPU sensor(&hi2c1); //pb7 sda , pb6 scl

Motor righ_motor(GPIOB , GPIO_PIN_0 , GPIOB , GPIO_PIN_1 , &htim1 , TIM_CHANNEL_1 ); //PA8
Motor left_motor( GPIOA , GPIO_PIN_1 , GPIOA , GPIO_PIN_2 , &htim2 , TIM_CHANNEL_1); // PA0

UART airport(&huart1); //pa10 rx , pa9tx
PID controller(0.0377f , 2.2f , 1.1f , 0.0f);

void main_app(void) 
{


    sensor.init();
    righ_motor.init();
    left_motor.init();

    Motor::Direction last_direction = Motor::Direction::STOP;
    uint32_t last_time = HAL_GetTick();

    while (1)
    {
        uint32_t now = HAL_GetTick();
        float dt = (float)(now - last_time) / 1000.0f;
        last_time = now;

        sensor.readData();
        sensor.updateAngles(dt);

        float motor_input = controller.calculate(sensor.getRoll() , dt);
//*
        Motor::Direction motor_direction = motor_input >= 0 ? Motor::Direction::CCW : Motor::Direction::CC;
        
    if ( std::abs(motor_input) < 2.5f)
        {
            left_motor.stop();
            righ_motor.stop();
        }
    if ( std::abs(motor_input) < 2.5f || sensor.getRoll() > 60.0f)
        {
            left_motor.stop();
            righ_motor.stop();
            HAL_Delay(10);
        }
        else
        {
            if ( last_direction != motor_direction){
                left_motor.stop();
                righ_motor.stop();
                last_direction = motor_direction;
            }
            else 
            {
                righ_motor.move( motor_direction , std::abs(motor_input) );
                left_motor.move(motor_direction , std::abs(motor_input) );
            }
        }
    }
}

