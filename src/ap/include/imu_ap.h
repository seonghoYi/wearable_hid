#ifndef SRC_AP_INCLUDE_IMU_AP_H_
#define SRC_AP_INCLUDE_IMU_AP_H_


#include "ap_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _USE_AP_IMU_AP

void threadImu(void *arg);
void threadImu1(void *arg);
void threadImu2(void *arg);
void threadImu3(void *arg);
void threadImu4(void *arg);
void threadImu5(void *arg);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_AP_INCLUDE_IMU_AP_H_ */
