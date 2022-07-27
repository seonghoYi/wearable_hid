#ifndef SRC_HW_HW_H_
#define SRC_HW_HW_H_

#include "hw_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio.h"
#include "led.h"
#include "uart.h"
#include "i2c.h"
#include "spi.h"
#include "cli.h"

#include "imu.h"
#include "adxl345.h"
#include "hc06.h"

bool hwInit();

#ifdef __cplusplus
}
#endif

#endif /* SRC_HW_HW_H_ */
