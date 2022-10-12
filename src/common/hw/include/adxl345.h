#ifndef SRC_COMMON_HW_INCLUDE_ADXL345_H_
#define SRC_COMMON_HW_INCLUDE_ADXL345_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"

#ifdef _USE_HW_ADXL345
#define			ADXL345_MAX_CH			HW_ADXL345_MAX_CH

#define ADXL345_1     _DEF_ADXL345_1
#define ADXL345_2     _DEF_ADXL345_2
#define ADXL345_3     _DEF_ADXL345_3
#define ADXL345_4     _DEF_ADXL345_4

#define ADXL345_REG_DEVID             0x00U
#define ADXL345_REG_THRESH_TAP        0x1DU
#define ADXL345_REG_OFSX              0x1EU
#define ADXL345_REG_OFSY              0x1FU
#define ADXL345_REG_OFSZ              0x20U
#define ADXL345_REG_DUR               0x21U
#define ADXL345_REG_LATENT            0x22U
#define ADXL345_REG_WINDOW            0x23U
#define ADXL345_REG_THRESH_ACT        0x24U
#define ADXL345_REG_THRESH_INACT      0x25U
#define ADXL345_REG_TIME_INACT        0x26U
#define ADXL345_REG_ACT_INACT_CTL     0x27U
#define ADXL345_REG_THRESH_FF         0x28U
#define ADXL345_REG_TIME_FF           0x29U
#define ADXL345_REG_TAP_AXES          0x2AU
#define ADXL345_REG_ACT_TAP_STATUS    0x2BU
#define ADXL345_REG_BW_RATE           0x2CU
#define ADXL345_REG_POWER_CTL         0x2DU
#define ADXL345_REG_INT_ENABLE        0x2EU
#define ADXL345_REG_INT_MAP           0x2FU
#define ADXL345_REG_INT_SOURCE        0x30U
#define ADXL345_REG_DATA_FORMAT       0x31U
#define ADXL345_REG_DATAX0            0x32U
#define ADXL345_REG_DATAX1            0x33U
#define ADXL345_REG_DATAY0            0x34U
#define ADXL345_REG_DATAY1            0x35U
#define ADXL345_REG_DATAZ0            0x36U
#define ADXL345_REG_DATAZ1            0x37U
#define ADXL345_REG_FIFO_CTL          0x38U
#define ADXL345_REG_FIFO_STATUS       0x39U



bool adxl345Init(void);
bool adxl345Begin(uint8_t ch);
bool adxl345GetData(uint8_t ch, float data[3]);
bool adxl345GetRawData(uint8_t ch, int16_t data[3]);


#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_ADXL345_H_ */
