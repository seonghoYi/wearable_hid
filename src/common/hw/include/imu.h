#ifndef SRC_COMMON_HW_INCLUDE_IMU_H_
#define SRC_COMMON_HW_INCLUDE_IMU_H_


#include "hw_def.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _USE_HW_IMU


bool imuInit(void);
bool imuBegin(uint32_t update_hz);
uint32_t imuUpdate();
float *imuGetRPY(void);
float *imuGetQuaternion(void);
int16_t *imuGetAcc(void);
int16_t *imuGetGyro(void);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_IMU_H_ */