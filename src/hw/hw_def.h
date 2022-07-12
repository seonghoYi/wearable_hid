#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "def.h"
#include "bsp.h"



#define _HW_DEF_RTOS_MEM_SIZE(x)              ((x)/4) //메모리 할당이 4바이트(uint32_t) 씩


#define _HW_DEF_RTOS_THREAD_PRI_MAIN          0
#define _HW_DEF_RTOS_THREAD_PRI_IMU1          1
#define _HW_DEF_RTOS_THREAD_PRI_IMU2          1
#define _HW_DEF_RTOS_THREAD_PRI_IMU3          1
#define _HW_DEF_RTOS_THREAD_PRI_IMU4          1
#define _HW_DEF_RTOS_THREAD_PRI_IMU5          1

#define _HW_DEF_RTOS_THREAD_MEM_MAIN          _HW_DEF_RTOS_MEM_SIZE( 20*1024)
#define _HW_DEF_RTOS_THREAD_MEM_IMU1          _HW_DEF_RTOS_MEM_SIZE( 4*1024)
#define _HW_DEF_RTOS_THREAD_MEM_IMU2          _HW_DEF_RTOS_MEM_SIZE( 4*1024)
#define _HW_DEF_RTOS_THREAD_MEM_IMU3          _HW_DEF_RTOS_MEM_SIZE( 4*1024)
#define _HW_DEF_RTOS_THREAD_MEM_IMU4          _HW_DEF_RTOS_MEM_SIZE( 4*1024)
#define _HW_DEF_RTOS_THREAD_MEM_IMU5          _HW_DEF_RTOS_MEM_SIZE( 4*1024)

#define _USE_HW_RTOS


#define _USE_HW_LED
#define      HW_LED_MAX_CH          1

#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH         4

#define _USE_HW_I2C
#define 		 HW_I2C_MAX_CH 					2
#define _USE_HW_I2C_MUX
#define _USE_HW_TCA9548A
#define      HW_I2C_MUX_MAX_CH      4

//#define _USE_HW_SPI
#define      HW_SPI_MAX_CH          1

#define _USE_HW_UART
#define      HW_UART_MAX_CH         2


#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    24
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    64


#define _USE_HW_MPU6050
#define _USE_HW_IMU
#define      HW_IMU_MAX_CH          5


//#define  _USE_HW_ADXL345
#define 	   HW_ADXL345_MAX_CH      4


#define _USE_HW_HC06

#endif /* SRC_HW_HW_DEF_H_ */
