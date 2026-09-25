#include "mpu.h"
#include <stdio.h>
#include <cmath>

float MPU::computeScaled( const float & raw , float scale ,  float shift){
    return (raw/scale) + shift;
}


float MPU::computeRoll( const MPU_DATA &data) {
    return atan2f(data.accel_y, sqrtf(data.accel_x * data.accel_x + data.accel_z * data.accel_z)) * 180.0f / M_PI;
}


float MPU::computePitch( const MPU_DATA &data) {
    return atan2f(-data.accel_x, sqrtf(data.accel_y * data.accel_y + data.accel_z * data.accel_z)) * 180.0f / M_PI;
}


float MPU::complementaryFilter(float angle , float gyro , float accel , float dt , float alpha) {
    return (alpha * (angle + (gyro * dt))) + ((1.0f - alpha) * accel);
}


void MPU::init()
{
    uint8_t pData = 0x00;
    if (HAL_I2C_Mem_Write( hi2c1_ , MPU6050_ADDR , PWR_MGMT_1_REG , I2C_MEMADD_SIZE_8BIT , &pData , 1 , 100 ) == HAL_OK)
    {
        initialized_ = true;
        return;
    }
    printf("Initialization failed , check the connection. \n");
}


bool MPU::readData()
{
    uint8_t buffer[14];
    float sum_accel_x = 0.0f , sum_accel_y = 0.0f , sum_accel_z = 0.0f, sum_gyro_x = 0.0f , sum_gyro_y = 0.0f , sum_gyro_z = 0.0f;
    const int reading_samples = 5;
    int reading_count = 0;

    auto clampGyro = [](float value, float max_reasonable = 32000.0f) -> float {
        return (fabsf(value) > max_reasonable) ? 0.0f : value;
    };

    for ( size_t i = 0 ; i < reading_samples ; i++)
    {

        if ((HAL_I2C_Mem_Read( hi2c1_ , MPU6050_ADDR , ACCEL_XOUT_H_REG , I2C_MEMADD_SIZE_8BIT , buffer , 14 , 100 )) != HAL_OK)
            {
                HAL_I2C_DeInit(hi2c1_);
                HAL_I2C_Init(hi2c1_);
                return false;
            }

        sum_accel_x += (int16_t)(buffer[0] << 8 | buffer[1]);
        sum_accel_y += (int16_t)(buffer[2] << 8 | buffer[3]);
        sum_accel_z += (int16_t)(buffer[4] << 8 | buffer[5]);

        sum_gyro_x += clampGyro((int16_t)(buffer[8]  << 8 | buffer[9]));
        sum_gyro_y += clampGyro((int16_t)(buffer[10] << 8 | buffer[11]));
        sum_gyro_z += clampGyro((int16_t)(buffer[12] << 8 | buffer[13]));

        reading_count++;
    }

    if (reading_count == 0)
    {
        printf("Failed to read data from MPU6050.\n");
        return false;
    }

    reading_.accel_x = sum_accel_x / reading_samples;
    reading_.accel_y = sum_accel_y / reading_samples;
    reading_.accel_z = sum_accel_z / reading_samples;

    reading_.gyro_roll = (sum_gyro_x / reading_samples);  
    reading_.gyro_pitch = (sum_gyro_y / reading_samples);
    reading_.gyro_yaw = (sum_gyro_z / reading_samples);

    return true;
}


MPU_DATA MPU::getAccurateData()
{
    if (!initialized_)
    {
        printf("MPU not initialized. Call init() first.\n");
        Error_Handler();
    }

    MPU_DATA scaled_reading;

    scaled_reading.accel_x = computeScaled( reading_.accel_x , 16384.0f);
    scaled_reading.accel_y = computeScaled( reading_.accel_y , 16384.0f);
    scaled_reading.accel_z = computeScaled( reading_.accel_z , 16384.0f);
    
    scaled_reading.gyro_roll = computeScaled( reading_.gyro_roll , 131.0f );
    scaled_reading.gyro_pitch = computeScaled( reading_.gyro_pitch , 131.0f );
    scaled_reading.gyro_yaw = computeScaled ( reading_.gyro_yaw , 131.0f);

    return scaled_reading;
}


void MPU::updateAngles(float dt)
{
    MPU_DATA data = getAccurateData();
    float pitch_accel = computePitch(data);
    float roll_accel = computeRoll(data);

    if (dt <= 0.0 || dt >= 0.1){
        dt = 0.01;
    }

    float acceleration = sqrt( (data.accel_x * data.accel_x + data.accel_z * data.accel_z) );
    if ( acceleration > 1.8 ) {
        pitch_angle_ = complementaryFilter(pitch_angle_, data.gyro_roll,  pitch_accel, dt , 1.0f);
        roll_angle_  = complementaryFilter(roll_angle_,  data.gyro_pitch, roll_accel,  dt , 1.0f);
    }
    else {
        pitch_angle_ = complementaryFilter(pitch_angle_, data.gyro_roll,  pitch_accel, dt );
        roll_angle_  = complementaryFilter(roll_angle_,  data.gyro_pitch, roll_accel,  dt );
    }
    
}

