#include "ap.h"
#include "accelerometer_handler.h"
#include "tap_detector.h"
#include "filter.h"

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

	float sthumb, sindex, smiddle, sring;

	filter_t filter[4][3];
	tapDetectorObjTypedef_t tap_obj[4];

	HC06Begin(HC06_BAUD_115200);
	accelerometerHandlerInit();

	tapDetectorInit(&tap_obj[0], 1.3f);
	tapDetectorInit(&tap_obj[1], 1.5f);
	tapDetectorInit(&tap_obj[2], 1.7f);
	tapDetectorInit(&tap_obj[3], 1.7f);

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

		sthumb = sqrtf(powf(thumb[0], 2) + powf(thumb[1], 2) + powf(thumb[2], 2));
		sindex = sqrtf(powf(index[0], 2) + powf(index[1], 2) + powf(index[2], 2));
		smiddle = sqrtf(powf(middle[0], 2) + powf(middle[1], 2) + powf(middle[2], 2));
		sring = sqrtf(powf(ring[0], 2) + powf(ring[1], 2) + powf(ring[2], 2));

		HC06Printf("%f,%f,%f,%f\n", sthumb+4.0, sindex+2.0, smiddle-2.0, sring-4.0);

		bool ttap = tapDetectorUpdate(&tap_obj[0], sthumb);
		bool itap = tapDetectorUpdate(&tap_obj[1], sindex);
		bool mtap = tapDetectorUpdate(&tap_obj[2], smiddle);
		bool rtap = tapDetectorUpdate(&tap_obj[3], sring);

		if (ttap | itap | mtap | rtap)
		{
			printf("tap: %d, %d, %d, %d\n", ttap, itap, mtap, rtap);
		}
		prev_time = millis();
    //delay(5);
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
}



void apMain()
{

	while(1)	
	{
		delay(1);
	}
}

