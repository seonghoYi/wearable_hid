#ifndef SRC_COMMON_HW_INCLUDE_HC06_H_
#define SRC_COMMON_HW_INCLUDE_HC06_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"

#ifdef _USE_HW_HC06

#define _DEF_HC06   _DEF_UART2


typedef enum
{
  HC06_BAUD_1200 = 1,
  HC06_BAUD_2400,
  HC06_BAUD_4800,
  HC06_BAUD_9600,
  HC06_BAUD_19200,
  HC06_BAUD_38400,
  HC06_BAUD_57600,
  HC06_BAUD_115200,
  HC06_BAUD_230400,
  HC06_BAUD_460800,
  HC06_BAUD_921600,
  HC06_BAUD_1382400,
} HC06_baud_t;



bool      HC06Init(void);
bool      HC06Begin(HC06_baud_t baud);
uint32_t  HC06Available(void);
uint8_t   HC06Read(void);
uint32_t  HC06Write(uint8_t *p_data, uint32_t length);
uint32_t  HC06Printf(char *fmt, ...);




#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_HC06_H_ */
