#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "def.h"
#include "bsp.h"


#define _USE_HW_LED
#define      HW_LED_MAX_CH          1

//#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH         1

#define _USE_HW_I2C
#define 		 HW_I2C_MAX_CH 					1


#define _USE_HW_UART
#define      HW_UART_MAX_CH         1


#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    24
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    64


//#define _USE_HW_SSD1306
#define 		 HW_SSD1306_WIDTH				128
#define			 HW_SSD1306_HEIGHT			64


//#define _USE_HW_ILI9481
#define			 HW_ILI9481_WIDTH				480
#define 		 HW_ILI9481_HEIGHT			320
#define			 HW_ILI9481_GPIO_MAX_CH	13


//#define _USE_HW_LCD
#define 		 HW_LCD_CH_MAX					1


#define _USE_HW_MPU6050

#define _USE_HW_IMU

#endif /* SRC_HW_HW_DEF_H_ */
