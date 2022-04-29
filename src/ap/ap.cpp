#include "ap.h"

void apInit()
{
	uartOpen(_DEF_UART1, 38400);
	//cliOpen(_DEF_UART1, 38400);

	imuBegin(50);
}


void apMain()
{
	uint32_t pre_time = millis();
	uint32_t loop_us = 0;

	float *rpy;

	while(1)	
	{
		/*
		if (millis() - pre_time >= 500)
		{
			ledToggle(_DEF_LED1);
			pre_time = millis();
		}
		*/
		imuUpdate();
		rpy = imuGetRPY();
		printf("%d, %d, %d\n", (int)rpy[0], (int)rpy[1], (int)rpy[2]);

		//cliMain();
	}
}