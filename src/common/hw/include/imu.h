#ifndef SRC_COMMON_HW_INCLUDE_IMU_H_
#define SRC_COMMON_HW_INCLUDE_IMU_H_


#include "hw_def.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _USE_HW_IMU
#define IMU_MAX_CH HW_IMU_MAX_CH

bool      imuInit(void);
bool      imuBegin(uint8_t ch, uint32_t update_hz);
uint32_t  imuUpdate(uint8_t ch);
void      imuGetRPY(uint8_t ch, float (&rpy)[3]);
void      imuGetQuaternion(uint8_t ch, float (&quat)[4]);
void      imuGetAcc(uint8_t ch, float (&acc)[3]);
void      imuGetGyro(uint8_t ch, float (&gyro)[3]);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_IMU_H_ */