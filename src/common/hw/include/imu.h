#ifndef SRC_COMMON_HW_INCLUDE_IMU_H_
#define SRC_COMMON_HW_INCLUDE_IMU_H_


#include "hw_def.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _USE_HW_IMU

bool      imuInit(void);
bool      imuBegin(uint32_t update_hz);
uint32_t  imuUpdate();
void      imuGetRPY(float (&rpy)[3]);
void      imuGetQuaternion(float (&quat)[4]);
void      imuGetAcc(float (&acc)[3]);
void      imuGetGyro(float (&gyro)[3]);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_IMU_H_ */