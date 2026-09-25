#pragma once
#include "main.h"


class PID
{
    private:
    float set_point_ , kp_ , ki_ , kd_ , I;
    float previous_error_;
    float PID_output_; 


    public:
    PID(float set_point , float kp , float ki , float kd) : 
    set_point_{set_point} , kp_{kp} , ki_{ki} , kd_{kd} , 
	I(0.0f)  ,previous_error_{0.0f} , PID_output_{0.0f}
    {}

    float calculate( float measured , float dt );

    ~PID() = default;
};


