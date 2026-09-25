#include "PID.h"
#include <cmath>

float PID::calculate( float measured , float dt )
{
    if (dt <= 0.0f ) { return PID_output_; }
    if (std::isnan(measured) || std::isinf(measured)) {
        return PID_output_;
    }
    const float MAX_PID = 100.0f;
    float current_error = set_point_ - measured;

    float P = kp_ * current_error;
    
    float D = kd_ * ((current_error - previous_error_) / dt);

    previous_error_ = current_error;
    

    float PID_output_now = P + I + D;

    if (PID_output_now > MAX_PID) { 
        PID_output_ = MAX_PID;
    }
    else if ( PID_output_now < -100.0f ) { 
        PID_output_ = -100.0f;
    }
    else {
        PID_output_ = PID_output_now;
        I += ki_ * current_error * dt;
    }

    return PID_output_;

}