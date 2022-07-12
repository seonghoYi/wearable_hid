#include "tca9548a.h"
#include "i2c.h"


#ifdef _USE_HW_TCA9548A


#ifdef _USE_HW_RTOS
SemaphoreHandle_t mutex = NULL;
#endif


#define TCA9548A_I2C_ADDRESS  0x70 << 1

typedef struct
{
  bool is_init;
  uint8_t i2c_ch;
  volatile uint8_t mux_ch;
} tca9548_t;


tca9548_t tca9548_handle;



bool tca9548Init(uint32_t freq_khz)
{
  bool ret = true;

  if(tca9548_handle.is_init)
  {
    return ret;
  }

  tca9548_handle.i2c_ch = _DEF_I2C2;
  tca9548_handle.mux_ch = 0x00;

  ret &= i2cBegin(tca9548_handle.i2c_ch, freq_khz);

#ifdef _USE_HW_RTOS
  mutex = xSemaphoreCreateMutex();
  if (mutex == NULL)
  {
    printf("mutex fail\n");
  }
#endif

  if(ret)
  {
    tca9548_handle.is_init = true;
  }

  return ret;
}

bool tca9548IsDeviceReady(uint8_t ch, uint8_t dev_addr)
{
  bool ret = false;
  uint8_t mux_ch = 1 << ch;

#ifdef _USE_HW_RTOS
  while (xSemaphoreTake(mutex, 10) != pdTRUE);
#endif

  if (tca9548_handle.mux_ch != mux_ch)
  {
    if (i2cWrite(tca9548_handle.i2c_ch, TCA9548A_I2C_ADDRESS, &mux_ch, 1) == 0)
    {
      return ret;
    }
    tca9548_handle.mux_ch = mux_ch;
  }
  
  ret = i2cIsDeviceReady(tca9548_handle.i2c_ch, dev_addr);
#ifdef _USE_HW_RTOS
  xSemaphoreGive(mutex);
#endif
  return ret;
}

uint32_t tca9548Write(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint16_t size)
{
  uint32_t ret = 0;
  uint8_t mux_ch = 1 << ch;

#ifdef _USE_HW_RTOS
  while (xSemaphoreTake(mutex, 10) != pdTRUE);
#endif

  if (tca9548_handle.mux_ch != mux_ch)
  {
    if (i2cWrite(tca9548_handle.i2c_ch, TCA9548A_I2C_ADDRESS, &mux_ch, 1) == 0)
    {
      return ret;
    }
    tca9548_handle.mux_ch = mux_ch;
  }

  ret = i2cWrite(tca9548_handle.i2c_ch, dev_addr, p_data, size);
#ifdef _USE_HW_RTOS
  xSemaphoreGive(mutex);
#endif
  return ret;
}

uint32_t tca9548Read(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint16_t size)
{
  uint32_t ret = 0;
  uint8_t mux_ch = 1 << ch;

#ifdef _USE_HW_RTOS
  while (xSemaphoreTake(mutex, 10) != pdTRUE);
#endif

  if (tca9548_handle.mux_ch != mux_ch)
  {
    if (i2cWrite(tca9548_handle.i2c_ch, TCA9548A_I2C_ADDRESS, &mux_ch, 1) == 0)
    {
      return ret;
    }
    tca9548_handle.mux_ch = mux_ch;
  }

  ret = i2cRead(tca9548_handle.i2c_ch, dev_addr, p_data, size);
#ifdef _USE_HW_RTOS
  xSemaphoreGive(mutex);
#endif
  return ret;
}

uint32_t tca9548MemWrite(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t data)
{
  uint32_t ret = 0;
  uint8_t mux_ch = 1 << ch;

#ifdef _USE_HW_RTOS
  while (xSemaphoreTake(mutex, 10) != pdTRUE);
#endif

  if (tca9548_handle.mux_ch != mux_ch)
  {
    if (i2cWrite(tca9548_handle.i2c_ch, TCA9548A_I2C_ADDRESS, &mux_ch, 1) == 0)
    {
      return ret;
    }
    tca9548_handle.mux_ch = mux_ch;
  }

  ret = i2cMemWrite(tca9548_handle.i2c_ch, dev_addr, mem_addr, mem_addr_size, data);
#ifdef _USE_HW_RTOS
  xSemaphoreGive(mutex);
#endif
  return ret;
}

uint32_t tca9548MemWrites(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size)
{
  uint32_t ret = 0;
  uint8_t mux_ch = 1 << ch;

#ifdef _USE_HW_RTOS
  while (xSemaphoreTake(mutex, 10) != pdTRUE);
#endif

  if (tca9548_handle.mux_ch != mux_ch)
  {
    if (i2cWrite(tca9548_handle.i2c_ch, TCA9548A_I2C_ADDRESS, &mux_ch, 1) == 0)
    {
      return ret;
    }
    tca9548_handle.mux_ch = mux_ch;
  } 

  ret = i2cMemWrites(tca9548_handle.i2c_ch, dev_addr, mem_addr, mem_addr_size, p_data, size);
#ifdef _USE_HW_RTOS
  xSemaphoreGive(mutex);
#endif
  return ret;
}

uint32_t tca9548MemRead(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data)
{
  uint32_t ret = 0;
  uint8_t mux_ch = 1 << ch;

#ifdef _USE_HW_RTOS
  while (xSemaphoreTake(mutex, 10) != pdTRUE);
#endif

  if (tca9548_handle.mux_ch != mux_ch)
  {
    if (i2cWrite(tca9548_handle.i2c_ch, TCA9548A_I2C_ADDRESS, &mux_ch, 1) == 0)
    {
      return ret;
    }
    tca9548_handle.mux_ch = mux_ch;
  }

  ret = i2cMemRead(tca9548_handle.i2c_ch, dev_addr, mem_addr, mem_addr_size, p_data);
#ifdef _USE_HW_RTOS
  xSemaphoreGive(mutex);
#endif
  return ret;
}

uint32_t tca9548MemReads(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size)
{
  uint32_t ret = 0;
  uint8_t mux_ch = 1 << ch;

#ifdef _USE_HW_RTOS
  while (xSemaphoreTake(mutex, 10) != pdTRUE);
#endif

  if (tca9548_handle.mux_ch != mux_ch)
  {
    if (i2cWrite(tca9548_handle.i2c_ch, TCA9548A_I2C_ADDRESS, &mux_ch, 1) == 0)
    {
      return ret;
    }
    tca9548_handle.mux_ch = mux_ch;
  } 

  ret = i2cMemReads(tca9548_handle.i2c_ch, dev_addr, mem_addr, mem_addr_size, p_data, size);
#ifdef _USE_HW_RTOS
  xSemaphoreGive(mutex);
#endif
  return ret;
}


#endif