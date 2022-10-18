#include "ap.h"
#include "accelerometer_handler.h"
#include "filter.h"
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
		delay(200);
	}
	
}


void accelerometerThread(void *arg)
{
	float data[12];
	float *thumb = &data[0];
	float *index = &data[3];
	float *middle = &data[6];
	float *ring = &data[9];


	filter_t filter[4][3];

	QueueHandle_t queue = (QueueHandle_t)arg;

	HC06Begin(HC06_BAUD_115200);
	accelerometerHandlerInit();

	for (int i = 0; i < 3; i++)
	{
		filterInit(&filter[0][i]);
		filterInit(&filter[1][i]);
		filterInit(&filter[2][i]);
		filterInit(&filter[3][i]);
	}
	uint32_t prev_time = millis();

  float sample_time = 1.f/3200.f;
	while(1)
	{
		
    accelerometerRead(data);
    
    for (int i = 0; i < 3; i++)
    {
      thumb[i] = filterHighPassFilter(&filter[0][i], thumb[i], sample_time, 200.0);
      index[i] = filterHighPassFilter(&filter[1][i], index[i], sample_time, 200.0);
      middle[i] = filterHighPassFilter(&filter[2][i], middle[i], sample_time, 200.0);
      ring[i] = filterHighPassFilter(&filter[3][i], ring[i], sample_time, 200.0);
		}
    //uartPrintf(_DEF_UART2, "ix:%f iy:%f iz:%f, mx:%f my:%f mz:%f, rx:%f ry:%f rz:%f, lx:%f ly:%f lz:%f\n", index[0]+8.0, index[1]+8.0, index[2]+8.0,
    //                                                    middle[0]+4.0, middle[1]+4.0, middle[2]+4.0,
    //                                                    ring[0], ring[1], ring[2],
    //                                                    little[0]-4.0, little[1]-4.0, little[2]-4.0);

    //HC06Printf("i:%f, m:%f, r:%f, l:%f\n", sindex+8.0, smiddle+4.0, sring, slittle-4.0);
    xQueueSend(queue, (void *)data, 0);
		prev_time = millis();
    delay(5);
	}
}


void dataProcessThread(void *arg);

void apInit()
{
	uartOpen(_DEF_UART1, 38400);

	QueueHandle_t raw_data_queue;
	raw_data_queue = xQueueCreate(960, sizeof(float)*4*3);

	if (raw_data_queue == NULL)
	{
		printf("Queue allocation failed.\nSystem halted.\n");
		while(1);
	}

	xTaskCreate(threadLed, "threadLed", 128, NULL, 3, NULL);
	xTaskCreate(accelerometerThread, "accelerometerThread", 1024, (void *)raw_data_queue, 1, NULL);
	xTaskCreate(dataProcessThread, "dataProcessThread", 1024, (void *)raw_data_queue, 2, NULL);
}



void apMain()
{

	while(1)	
	{
		delay(1);
	}
}


void dataProcessThread(void *arg)
{
  float data[12];
	float *thumb = &data[0];
	float *index = &data[3];
	float *middle = &data[6];
	float *ring = &data[9];

  float sthumb, sindex, smiddle, sring;

	QueueHandle_t queue = (QueueHandle_t)arg;

  uint32_t prev_time = millis();
  
  while(1)
  {
		if (xQueueReceive(queue, (void *)&data, portMAX_DELAY) == pdPASS)
		{
      sthumb = sqrtf(powf(thumb[0], 2) + powf(thumb[1], 2) + powf(thumb[2], 2));
			sindex = sqrtf(powf(index[0], 2) + powf(index[1], 2) + powf(index[2], 2));
			smiddle = sqrtf(powf(middle[0], 2) + powf(middle[1], 2) + powf(middle[2], 2));
			sring = sqrtf(powf(ring[0], 2) + powf(ring[1], 2) + powf(ring[2], 2));

      //HC06Printf("@%f,%f,%f,%f\n", sthumb, sindex, smiddle, sring);

			HC06Printf("%f,%f,%f,%f\n", sthumb+4.0, sindex+2.0, smiddle-2.0, sring-4.0);

			prev_time=millis();
			
		}
  }
}
