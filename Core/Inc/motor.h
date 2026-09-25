#ifndef MOTOR_H_CPP
#define MOTOR_H_CPP

#include "pwmpin.h"
#include "gpio.h"
/*
  motor.h Created on: Sep 17, 2026 Author: Eng Mohammad Ossama
 */

class Motor
{
    private:
    Pin pin_out_one_;
    Pin pin_out_two_;
    PwmPin pwm_pin_out_;
    float current_speed_ = 0.0f;
    bool is_initialized_ = false;

    public:
    Motor() = delete;
    Motor(const Motor&) = delete;
    Motor& operator=(const Motor&) = delete;
   /**
    * @brief constructor for a new motor object.
    * @param port_one GPIO Port for direction pin 1 (e.g., GPIOA)
    * @param pin_one_number GPIO Pin number for direction pin 1 (e.g., GPIO_PIN_0)
    * @param port_two GPIO Port for direction pin 2 (e.g., GPIOA)
    * @param pin_two_number GPIO Pin number for direction pin 2 (e.g., GPIO_PIN_1)
    * @param pwm_timer_number Pointer to TIM Handle (e.g., &htim1)
    * @param channel TIM Channel for PWM output (e.g., TIM_CHANNEL_1)
    */
    Motor(GPIO_TypeDef *port_one , uint32_t pin_one_number, 
            GPIO_TypeDef *port_two, uint32_t pin_two_number, 
            TIM_HandleTypeDef* pwm_timer_number , uint32_t channel) : 
            pin_out_one_(port_one , pin_one_number) ,
            pin_out_two_(port_two , pin_two_number) , 
            pwm_pin_out_(pwm_timer_number , channel){}

    enum class Direction{
        CC,
        CCW,
        STOP
    };

    void init();
    /**
     * @warning MUST BE USED FOR EMERGENCY STOP ONLY OR INITALIZATION!
     */
    void stop();
    /**
     * @param step_delay_ms The time between every step Eq: Required Time = Delay_ms * 100 
     * @param direction Motor::Direction::CC for Clockwise / Motor::Direction::CCW for Counter-Clockwise
     */
    void softstart(Direction direction , uint16_t step_delay_ms);
    /**
     * @param step_delay_ms The time between every step Eq: Required Time = Delay_ms * 100 
     */
    void softstop(uint16_t step_delay_ms);
    /**
    * @param direction Motor::Direction::CC for Clockwise / Motor::Direction::CCW for Counter-Clockwise
    */
    void move(Direction direction , float speed_percentage);
};


#endif //MOTOR_H_CPP