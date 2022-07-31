#include "i2c.h"
#include "cli.h"
#include "hardware/i2c.h"

#ifdef _USE_HW_I2C


#ifdef _USE_HW_CLI
static void cliI2C(cli_args_t *args);
#endif


typedef struct
{
	bool is_init;
	i2c_inst_t *p_hi2c;
} i2c_tbl_t;


i2c_tbl_t i2c_tbl[I2C_MAX_CH];



bool i2cInit()
{
	for(int i = 0; i < I2C_MAX_CH; i++)
	{
		i2c_tbl[i].is_init = false;
		i2c_tbl[i].p_hi2c 	 = NULL;
	}


#ifdef _USE_HW_CLI
	cliAdd("i2c", cliI2C);
#endif

	return true;
}

bool i2cBegin(uint8_t ch, uint32_t freq_khz)
{
	bool ret = false;


  if (i2c_tbl[ch].is_init == true)
  {
    return true;
  }

	switch(ch)
	{
		case _DEF_I2C1:
			i2c_tbl[_DEF_I2C1].p_hi2c = i2c1;
			i2c_init(i2c_tbl[_DEF_I2C1].p_hi2c, freq_khz * 1000);
			gpio_set_function(2, GPIO_FUNC_I2C);
			gpio_set_function(3, GPIO_FUNC_I2C);

			gpio_pull_up(2);
			gpio_pull_up(3);

			//bi_decl(bi_2pins_with_func(2, 3, GPIO_FUNC_I2C));
    	//bi_decl(bi_program_description("OLED I2C example for the Raspberry Pi Pico"));
			ret = true;
		break;
    
    case _DEF_I2C2:
      i2c_tbl[_DEF_I2C2].p_hi2c = i2c0;
      i2c_init(i2c_tbl[_DEF_I2C2].p_hi2c, freq_khz * 1000);
			gpio_set_function(0, GPIO_FUNC_I2C);
			gpio_set_function(1, GPIO_FUNC_I2C);

			gpio_pull_up(0);
			gpio_pull_up(1);
      
      ret = true;
    break;
    
		default:
		break;
	}

	return ret;
}

bool i2cIsDeviceReady(uint8_t ch, uint8_t dev_addr)
{
  i2c_inst_t *p_handle = i2c_tbl[ch].p_hi2c;
	uint8_t rxdata;

	if(dev_addr == 0x78)
	{
		return false;
	}

	if(i2c_read_timeout_us(p_handle, dev_addr, &rxdata, 1, false, 100*1000) > 0)
	{
		return true;
	}
	else
	{
  	return false;
	}
}

uint32_t i2cWrite(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint16_t size)
{
	uint32_t ret = 0;

	i2c_inst_t *p_handle = i2c_tbl[ch].p_hi2c;

	if (i2c_write_timeout_us(p_handle, dev_addr >> 1, p_data, size, false, 100*1000) > 0)
	{
		ret = size;
	}
	return ret;
}

uint32_t i2cRead(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint16_t size)
{
	uint32_t ret = 0;

	i2c_inst_t *p_handle = i2c_tbl[ch].p_hi2c;

	if(i2c_read_timeout_us(p_handle, dev_addr >> 1, p_data, size, false, 100*1000) > 0)
	{
		ret = size;
	}

	return ret;
}

uint32_t i2cMemWrites(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size)
{
	uint32_t ret = 0;

	i2c_inst_t *p_handle = i2c_tbl[ch].p_hi2c;

	uint8_t *temp = malloc(size + mem_addr_size);
	
	if (mem_addr_size > 1)
	{
		for(int i = 0; i < mem_addr_size; i++)
		{
			temp[i] = (mem_addr >> 8 * i) & 0xFF;
		}
	}
	else
	{
		temp[0] = mem_addr;
	}

	for (int i = mem_addr_size; i < size + mem_addr_size; i++)
	{
		temp[i] = p_data[i-mem_addr_size];
	}

	if (i2c_write_timeout_us(p_handle, dev_addr >> 1, temp, size + mem_addr_size, false, 100*1000) > 0)
	{
		ret = size;
	}

	free(temp);

	return ret;
}

uint32_t i2cMemWrite(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t data)
{
	uint32_t ret = 0;
	ret = i2cMemWrites(ch, dev_addr, mem_addr, mem_addr_size, &data, 1);
  return ret;
}

uint32_t i2cMemReads(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size)
{
	uint32_t ret = 0;

	i2c_inst_t *p_handle = i2c_tbl[ch].p_hi2c;

	if(i2c_write_timeout_us(p_handle, dev_addr >> 1, (uint8_t *)&mem_addr, mem_addr_size, true, 100*1000) > 0)
	{
		if(i2c_read_timeout_us(p_handle, dev_addr >> 1, p_data, size, false, 100*1000) > 0)
		{
			ret = size;
		}
	}

	return ret;
}

uint32_t i2cMemRead(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data)
{
  uint32_t ret = 0;
  ret = i2cMemReads(ch, dev_addr, mem_addr, mem_addr_size, p_data, 1);
  return ret;
}





#ifdef _USE_HW_CLI
void cliI2C(cli_args_t *args)
{
  bool ret = true;
  bool i2c_ret;

  uint8_t print_ch;
  uint8_t ch;
  uint16_t dev_addr;
  uint16_t reg_addr;
  uint16_t length;

  uint32_t i;
  uint8_t i2c_data[128];
  uint32_t pre_time;



	//i2c scan 


  if (args->argc == 2)
  {
    print_ch = (uint16_t) args->getData(1);

    print_ch = constrain(print_ch, 1, I2C_MAX_CH);
    print_ch -= 1;

    if(args->isStr(0, "scan") == true)
    {
      for (i=0x00; i<= 0x7F; i++)
      {
        if (i2cIsDeviceReady(print_ch, i) == true)
        {
          cliPrintf("I2C CH%d Addr 0x%X : OK\r\n", print_ch+1, i);
        }
      }
    }
    else if(args->isStr(0, "begin") == true)
    {
      i2c_ret = i2cBegin(print_ch, 400);
      if (i2c_ret == true)
      {
        cliPrintf("I2C CH%d Begin OK\r\n", print_ch + 1);
      }
      else
      {
        cliPrintf("I2C CH%d Begin Fail\r\n", print_ch + 1);
      }
    }
  }
  else if (args->argc == 5)
  {
    print_ch = (uint16_t) args->getData(1);
    print_ch = constrain(print_ch, 1, I2C_MAX_CH);

    dev_addr = (uint16_t) args->getData(2);
    reg_addr = (uint16_t) args->getData(3);
    length   = (uint16_t) args->getData(4);
    ch       = print_ch - 1;

    if(args->isStr(0, "read") == true)
    {
      for (i=0; i<length; i++)
      {
        i2c_ret = i2cMemReads(ch, dev_addr, reg_addr+i, 1, i2c_data, 1);

        if (i2c_ret == true)
        {
          cliPrintf("%d I2C - 0x%02X : 0x%02X\r\n", print_ch, reg_addr+i, i2c_data[0]);
        }
        else
        {
          cliPrintf("%d I2C - Fail \r\n", print_ch);
          break;
        }
      }
    }
    else if(args->isStr(0, "write") == true)
    {
      pre_time = millis();
      i2c_ret = i2cMemWrites(ch, dev_addr, reg_addr, 1, (uint8_t *)&length, 1);

      if (i2c_ret == true)
      {
        cliPrintf("%d I2C - 0x%02X : 0x%02X, %d ms\r\n", print_ch, reg_addr, length, millis()-pre_time);
      }
      else
      {
        cliPrintf("%d I2C - Fail \r\n", print_ch);
      }
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
    cliPrintf( "i2c begin channel[1~%d]\r\n", I2C_MAX_CH);
    cliPrintf( "i2c scan channel[1~%d]\r\n", I2C_MAX_CH);
    cliPrintf( "i2c read channel dev_addr reg_addr length\r\n");
    cliPrintf( "i2c write channel dev_addr reg_addr data\r\n");
  }
}

#endif


#endif
