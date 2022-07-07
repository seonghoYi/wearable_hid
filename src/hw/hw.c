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
	ret &= spiInit();
	ret &= imuInit();
	ret &= HC06Init();
	return ret;
}
