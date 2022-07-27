#include "adxl345.h"
#include "i2c.h"
#include "gpio.h"

#include "cli.h"

#ifdef _USE_HW_ADXL345

#define ADXL345_ADDRESS   0x53 << 1

#ifdef _USE_HW_CLI
static void cliADXL345(cli_args_t *args);
#endif



bool adxl345WriteReg(uint8_t ch, uint8_t reg, uint8_t data)
{
  bool ret = false;

  gpioPinWrite(_DEF_GPIO5, _DEF_HIGH); //Enable

  gpioPinWrite(_DEF_GPIO2, ch & 0x01); //s0
  gpioPinWrite(_DEF_GPIO3, ch & 0x02); //s1
  gpioPinWrite(_DEF_GPIO4, ch & 0x04); //s2


  if (i2cMemWrite(_DEF_I2C2, ADXL345_ADDRESS, reg, 1, data) > 0)
  {
    reg = true;
  }
  gpioPinWrite(_DEF_GPIO5, _DEF_LOW);

  return ret;
}

uint8_t adxl345ReadReg(uint8_t ch, uint8_t reg)
{
  uint8_t ret = 0;

  gpioPinWrite(_DEF_GPIO5, _DEF_HIGH); //Enable

  gpioPinWrite(_DEF_GPIO2, ch & 0x01); //s0
  gpioPinWrite(_DEF_GPIO3, ch & 0x02); //s1
  gpioPinWrite(_DEF_GPIO4, ch & 0x04); //s2

  
  i2cMemRead(_DEF_I2C2, ADXL345_ADDRESS, reg, 1, &ret);
  gpioPinWrite(_DEF_GPIO5, _DEF_LOW);

  return ret;
}




bool adxl345Init(void)
{
  bool ret = true;


#ifdef _USE_HW_CLI
	cliAdd("adxl", cliADXL345);
#endif

  return ret;
}

bool adxl345Begin(uint8_t ch)
{
  bool ret = true;

  i2cBegin(_DEF_I2C2, 400);

  delay(1000);

  //while (adxl345ReadReg(ch, ADXL345_REG_DEVID) != 0xE5);

  if (adxl345ReadReg(ch, ADXL345_REG_DEVID) == 0xE5)
  {
    ret &= true;
  }
  else
  {
    ret &= false;
  }



  return ret;
}


#ifdef _USE_HW_CLI
void cliADXL345(cli_args_t *args)
{
  bool ret = true;
  bool sensor_ret;

  uint8_t print_ch;
  uint8_t ch;
  uint8_t write_data;
  uint8_t reg_addr;

  if (args->argc == 2)
  {
    print_ch = (uint8_t) args->getData(1);

    print_ch = constrain(print_ch, 1, ADXL345_MAX_CH);
    ch = print_ch - 1;

    if(args->isStr(0, "begin") == true)
    {
      sensor_ret = adxl345Begin(ch);
      if (sensor_ret == true)
      {
        cliPrintf("adxl CH%d Begin OK\n", print_ch);
      }
      else
      {
        cliPrintf("adxl CH%d Begin Fail\n", print_ch);
      }
    }
  }
  else if (args->argc == 3)
  {
    print_ch = (uint8_t) args->getData(1);
    print_ch = constrain(print_ch, 1, ADXL345_MAX_CH);

    reg_addr = (uint8_t) args->getData(2);
    ch       = print_ch - 1;

    if(args->isStr(0, "read") == true)
    {
      cliPrintf("adxl CH%d 0x%02X : 0x%02X\n", print_ch, reg_addr, adxl345ReadReg(ch, reg_addr));
    }
    else
    {
      ret = false;
    }
  }
  else if (args->argc == 4)
  {
    print_ch = (uint8_t) args->getData(1);
    print_ch = constrain(print_ch, 1, ADXL345_MAX_CH);

    reg_addr = (uint8_t) args->getData(2);
    write_data = (uint8_t) args->getData(3);
    ch       = print_ch - 1;

    if (args->isStr(0, "write") == true)
    {
      adxl345WriteReg(ch, reg_addr, write_data);
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }

  if (ret == false)
  {
    cliPrintf( "adxl begin channel[%d~%d]\n", ADXL345_1+1, ADXL345_4+1);
    cliPrintf( "adxl read channel[%d~%d] reg\n", ADXL345_1+1, ADXL345_4+1);
    cliPrintf( "adxl write channel[%d~%d] reg data\n", ADXL345_1+1, ADXL345_4+1);
  }
}

#endif


#endif