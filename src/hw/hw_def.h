#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "def.h"
#include "bsp.h"


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
