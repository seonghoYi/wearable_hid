#ifndef SRC_AP_INCLUDE__ACCELEROMETER_HANDLER_H_
#define SRC_AP_INCLUDE__ACCELEROMETER_HANDLER_H_


#include "ap_def.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _USE_AP_ACCELEROMETER_HANDLER

bool accelerometerHandlerInit(void);
void accelerometerRead(float data[12]);
void accelerometerReadRaw(int16_t data[12]);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_AP_INCLUDE__ACCELEROMETER_HANDLER_H_ */