#include "bsp.h"



bool bspInit()
{
  bool ret = true;
  //set_sys_clock_khz(133000, true);
  //set_sys_clock_48mhz();
  set_sys_clock_khz(250000, true);
  stdio_init_all();


  delay(1000);
  return ret;
}



uint32_t millis()
{
	return to_ms_since_boot(get_absolute_time());
}

uint32_t micros()
{
  return to_us_since_boot(get_absolute_time());
}

void delay(uint32_t ms)
{
	sleep_ms(ms);
}