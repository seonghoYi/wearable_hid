#include "gpio.h"
#include "hardware/gpio.h"

#ifdef _USE_HW_GPIO

typedef struct
{
  uint8_t pin;
  uint8_t mode;
  bool on_state;
  bool off_state;
  bool init_value;
} gpio_tbl_t;




gpio_tbl_t gpio_tbl[GPIO_MAX_CH] = {
  {18, _DEF_OUTPUT, true, false, _DEF_LOW},
  {6, _DEF_OUTPUT, true, false, _DEF_LOW}, //i2c mux s0
  {7, _DEF_OUTPUT, true, false, _DEF_LOW}, //i2c mux s1
  {8, _DEF_OUTPUT, true, false, _DEF_LOW}, //i2c mux s2
  {9, _DEF_OUTPUT, false, true, _DEF_LOW}, //i2c mux E
};

bool gpioInit(void)
{
  bool ret = true;

  for(int i = 0; i < GPIO_MAX_CH; i++)
  {
    gpioPinMode(i, gpio_tbl[i].mode);
  }

  return ret;
}

bool gpioPinMode(uint8_t ch, uint8_t mode)
{
  bool ret = true;

  gpio_init(gpio_tbl[ch].pin);

  switch (mode)
  {
  case _DEF_INPUT:
    gpio_set_dir(gpio_tbl[ch].pin, _DEF_INPUT);
    gpio_disable_pulls(gpio_tbl[ch].pin);
    break;
  case _DEF_INPUT_PULLUP:
    gpio_set_dir(gpio_tbl[ch].pin, _DEF_INPUT);
    gpio_pull_up(gpio_tbl[ch].pin);
    break;
  case _DEF_INPUT_PULLDOWN:
    gpio_set_dir(gpio_tbl[ch].pin, _DEF_INPUT);
    gpio_pull_down(gpio_tbl[ch].pin);
    break;
  case _DEF_OUTPUT:
    gpio_set_dir(gpio_tbl[ch].pin, _DEF_OUTPUT);
    gpio_disable_pulls(gpio_tbl[ch].pin);
    break;
  case _DEF_OUTPUT_PULLUP:
    gpio_set_dir(gpio_tbl[ch].pin, _DEF_INPUT);
    gpio_pull_up(gpio_tbl[ch].pin);
    break;
  case _DEF_OUTPUT_PULLDOWN:
    gpio_set_dir(gpio_tbl[ch].pin, _DEF_INPUT);
    gpio_pull_down(gpio_tbl[ch].pin);
    break;
  }




  return ret;
}

void gpioPinWrite(uint8_t ch, bool value)
{
  if (ch >= GPIO_MAX_CH) return;
	
	if(value)
	{
		gpio_put(gpio_tbl[ch].pin, gpio_tbl[ch].on_state);
	}
	else
	{
		gpio_put(gpio_tbl[ch].pin, gpio_tbl[ch].off_state);
	}
}

bool gpioPinRead(uint8_t ch)
{
  bool ret = false;

  if (ch >= GPIO_MAX_CH) return ret;

	if (gpio_get(gpio_tbl[ch].pin) == gpio_tbl[ch].on_state)
	{
		ret = true;
	}
	return ret;
}

void gpioPinToggle(uint8_t ch)
{
  if (ch >= GPIO_MAX_CH) return;
  gpio_xor_mask(1<<gpio_tbl[ch].pin);
}


#endif