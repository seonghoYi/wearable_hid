#include "hw.h"




bool hwInit()
{
	bool ret = true;
	ret &= bspInit();
	ret &= gpioInit();
	ret &= ledInit();
	ret &= uartInit();
	ret &= cliInit();
	ret &= i2cInit();
	ret &= imuInit();
	ret &= adxl345Init();
	return ret;
}
