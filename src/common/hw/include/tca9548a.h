#ifndef SRC_COMMON_HW_INCLUDE_TCA9548A_H_
#define SRC_COMMON_HW_INCLUDE_TCA9548A_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"

#ifdef _USE_HW_TCA9548A
#define			I2C_MUX_MAX_CH			HW_I2C_MUX_MAX_CH




bool tca9548Init(uint32_t freq_khz);
bool tca9548IsDeviceReady(uint8_t ch, uint8_t dev_addr);
uint32_t tca9548Write(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint16_t size);
uint32_t tca9548Read(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint16_t size);
uint32_t tca9548MemWrite(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t data);
uint32_t tca9548MemWrites(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size);
uint32_t tca9548MemRead(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data);
uint32_t tca9548MemReads(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size);




#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_TCA9548A_H_ */
