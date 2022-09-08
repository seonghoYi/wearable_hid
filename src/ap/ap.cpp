#include "ap.h"
#include "accelerometer_handler.h"

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

void vApplicationMallocFailedHook( void )
{
	ledOn(_DEF_LED1);
	while(1);
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	ledOn(_DEF_LED1);
	while(1);
}

void vApplicationTickHook( void )
{

}

#ifdef __cplusplus
}
#endif


void threadLed(void *arg)
{
	while(1)
	{
		ledToggle(_DEF_LED1);
		delay(1000);
	}
	
}


void accelerometerThread(void *arg)
{
	float data[12];
	float *index = &data[0];
	float *middle = &data[3];
	float *ring = &data[6];
	float *little = &data[9];

	float sindex, smiddle, sring, slittle;

	HC06Begin(HC06_BAUD_115200);
	accelerometerHandlerInit();

	float loop_hz = 500.0;
	uint32_t loop_time = (1/loop_hz)*1000;
	uint32_t prev_time = millis();
	while(1)
	{
		if (millis()-prev_time >= loop_time)
		{
			accelerometerRead(data);
			sindex = sqrtf(powf(index[0], 2) + powf(index[1], 2) + powf(index[2], 2));
			smiddle = sqrtf(powf(middle[0], 2) + powf(middle[1], 2) + powf(middle[2], 2));
			sring = sqrtf(powf(ring[0], 2) + powf(ring[1], 2) + powf(ring[2], 2));
			slittle = sqrtf(powf(little[0], 2) + powf(little[1], 2) + powf(little[2], 2));
			//uartPrintf(_DEF_UART2, "ix:%f iy:%f iz:%f, mx:%f my:%f mz:%f, rx:%f ry:%f rz:%f, lx:%f ly:%f lz:%f\n", index[0]+8.0, index[1]+8.0, index[2]+8.0,
      //                                                    middle[0]+4.0, middle[1]+4.0, middle[2]+4.0,
      //                                                    ring[0], ring[1], ring[2],
      //                                                    little[0]-4.0, little[1]-4.0, little[2]-4.0);

			uartPrintf(_DEF_UART2, "i:%f, m:%f, r:%f, l:%f\n", sindex+8.0, smiddle+4.0, sring, slittle-4.0);

			prev_time = millis();
		}
	}
}


void apInit()
{
	uartOpen(_DEF_UART1, 38400);

	

	xTaskCreate(threadLed, "threadLed", 128, NULL, 1, NULL);
	xTaskCreate(accelerometerThread, "accelerometerThread", 1024, NULL, 1, NULL);
}


void apMain()
{


	while(1)	
	{

		delay(1);
	}
}