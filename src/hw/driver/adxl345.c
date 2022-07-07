#include "adxl345.h"
#include "spi.h"
#include "gpio.h"

#include "cli.h"

#ifdef _USE_HW_ADXL345


#ifdef _USE_HW_CLI
static void cliADXL345(cli_args_t *args);
#endif



void csEnable(uint8_t ch)
{
  gpioPinWrite(ch, true);
}

void csDisable(uint8_t ch)
{
  gpioPinWrite(ch, false);
}


void adxl345WriteReg(uint8_t ch, uint8_t reg, uint8_t data)
{
  uint8_t tx_buf[2], rx_buf[2];
  tx_buf[0] = reg | 0x00 | 0x40; //write, multibyte
  tx_buf[1] = data;

  csEnable(ch);
  //spiTransfer(_DEF_SPI1, tx_buf, rx_buf, 2);

  spiTransfer(_DEF_SPI1, tx_buf, rx_buf, 2, 100);
  csDisable(ch);

}

uint8_t adxl345ReadReg(uint8_t ch, uint8_t reg)
{
  uint8_t tx_buf[2], rx_buf[2];

  tx_buf[0] = reg | 0x80 | 0x40; //read, multibyte
  tx_buf[1] = 0x00;

  csEnable(ch);
  //spiTransfer(_DEF_SPI1, tx_buf, rx_buf, 2);
  spiTransfer(_DEF_SPI1, tx_buf, rx_buf, 2, 100);
  csDisable(ch);

  return rx_buf[1];
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

  spiBegin(_DEF_SPI1);//, 2*1000); //2MHz

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