#ifndef SRC_COMMON_HW_INCLUDE_IMU_IMU_H_
#define SRC_COMMON_HW_INCLUDE_IMU_IMU_H_


#include "hw_def.h"

#ifdef _USE_HW_IMU

#include "imu/mpu6050.h"
#include "imu/MadgwickAHRS.h"



class cIMU
{
  public:
    cMPU6050 sensor;


    bool b_connected;

    float accRes;
    float gyrRes;


    float ax, ay, az;
    float gx, gy, gz;


    int16_t angle[3];
    float rpy[3];
    float quat[4];

    int16_t accRaw[3];
    int16_t accData[3];
    int16_t gyroRaw[3];
    int16_t gyroData[3];

  public:
    cIMU();

    bool begin(uint32_t hz);
    uint32_t update(void);

  private:
    Madgwick filter;
    uint32_t update_hz;
    uint32_t update_us;

    void computeIMU(void);

};

#endif



#endif /* SRC_COMMON_HW_INCLUDE_IMU_IMU_H_ */
