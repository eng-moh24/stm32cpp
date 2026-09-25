#ifndef MPU_H_CPP
#define MPU_H_CPP

#define MPU6050_ADDR (0x68 << 1) // Shifted for HAL 7-bit addressing
#define PWR_MGMT_1_REG 0x6B
#define ACCEL_XOUT_H_REG 0x3B
#include "main.h"

struct MPU_DATA
{
    float accel_x , accel_y , accel_z;
    float gyro_roll , gyro_pitch , gyro_yaw;
};
/*
// محور x  على شريحة MPU6050 = PITCH (يمين/شمال)
// محور y على شريحة MPU6050 = ROLL (قدام/ورا)
*/

class MPU
{
    private:
    I2C_HandleTypeDef *hi2c1_;
    MPU_DATA reading_;
    float pitch_angle_;
    float roll_angle_;
    bool initialized_ = false;

    float computeScaled( const float & raw , float scale ,  float shift = 0.0f);

    float complementaryFilter ( float angle , float gyro , float accel , float dt , float alpha = 0.98f);

    public:
    /**
     * @param hi2c1 Pointer to the I2C handle for communication with the MPU6050.
     * @note The default constructor is deleted to enforce the requirement of providing an I2C handle during object creation. This ensures that the MPU class is always initialized with a valid I2C handle, preventing potential runtime errors related to uninitialized I2C communication.
     */
    MPU( I2C_HandleTypeDef * hi2c1 ) : hi2c1_{hi2c1} , pitch_angle_{0.0f} , roll_angle_{0.0f} {}
    MPU() = delete;
    MPU(const MPU&) = delete;
    MPU& operator=(const MPU&) = delete;

    float computePitch( const MPU_DATA &data);
    float computeRoll( const MPU_DATA &data );

    void init();
    bool readData();
    void updateAngles(float dt);
    float getPitch() const { return pitch_angle_; }
    float getRoll()  const { return roll_angle_; }

    MPU_DATA getAccurateData();
};

#endif