#ifndef SRC_COMMON_HW_INCLUDE_IMU_MPU6050_H_
#define SRC_COMMON_HW_INCLUDE_IMU_MPU6050_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"

#ifdef _USE_HW_MPU6050

#define MPU_CALI_COUNT 512 //512

class cMPU6050
{
  public:
    bool b_connected;
    
    
    int16_t accRaw[3];
    int16_t accZero[3];
    int16_t accData[3];
    int16_t gyroRaw[3];
    int16_t gyroZero[3];
    int16_t gyroData[3];




  public:
    cMPU6050();

    bool begin(void);
    void gyroGetData(void);
    void accGetData(void);
    bool accGetCaliDone(void);
    bool gyroGetCaliDone(void);
  private:
    
    uint8_t i2c_ch;
    uint16_t i2c_addr;

    int16_t calibrating_count_acc;
    int16_t calibrating_count_gyro;

    int32_t a[3];

    int16_t previousGyroData[3];
    int32_t g[3];

  private:
    bool init(void);
    void accInit(void);
    void gyroInit(void);
    void accStartCali(void);
    void gyroStartCali(void);
    void accCalibration(void);
    void gyroCalibration(void);

    bool regWrite(uint16_t reg_addr, uint8_t data);
    bool regRead(uint16_t reg_addr, uint8_t *p_data);
    bool regReads(uint16_t reg_addr, uint8_t *p_data, uint16_t size);
};



#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_MPU6050_H_ */