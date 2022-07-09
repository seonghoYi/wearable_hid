#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#include "def.h"

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>
#include <timers.h>

#include <pico/binary_info.h>
#include <pico/stdlib.h>
#include <hardware/clocks.h>
#include <hardware/pll.h>

#ifdef __cplusplus
extern "C" {
#endif



bool bspInit();

uint32_t millis();
uint32_t micros();
void delay(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* SRC_BSP_BSP_H_ */
