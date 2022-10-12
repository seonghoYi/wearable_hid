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


typedef struct
{
	uint8_t filter_type;
	float prev_input;
	float prev_output;
} filter_t;

float lowPassFilter(filter_t *filter, float input, float sampling_time, float cutOffFreq)
{
	float output;
	float wc = 2*M_PI*cutOffFreq; // 차단 각주파수
	float tau = 1/wc;


	output = (tau*filter->prev_output + sampling_time*input)/(tau+sampling_time);
	filter->prev_output = output; //나중을 위한 출력값 저장
	return output;
}


float highPassFilter(filter_t *filter, float input, float sampling_time, float cutOffFreq)
{
	float output;
	float wc = 2*M_PI*cutOffFreq; // 차단 각주파수
	float tau = 1/wc;


	output = (tau*filter->prev_output + tau*(input-filter->prev_input))/(tau+sampling_time);
	filter->prev_output = output;
	filter->prev_input = input;
	return output;
}



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
	int16_t data[12];

	//filter_t filter[4][3];

	QueueHandle_t queue = (QueueHandle_t)arg;

	HC06Begin(HC06_BAUD_115200);
	accelerometerHandlerInit();
	uint32_t prev_time = millis();

  //float sample_time = 1.f/3200.f;
	while(1)
	{
    accelerometerReadRaw(data);
    /*
    for (int i = 0; i < 3; i++)
    {
      index[i] = highPassFilter(&filter[0][i], index[i], sample_time, 200.0);
      middle[i] = highPassFilter(&filter[1][i], middle[i], sample_time, 200.0);
      ring[i] = highPassFilter(&filter[2][i], ring[i], sample_time, 200.0);
      little[i] = highPassFilter(&filter[3][i], little[i], sample_time, 200.0);
    }
    */
    //uartPrintf(_DEF_UART2, "ix:%f iy:%f iz:%f, mx:%f my:%f mz:%f, rx:%f ry:%f rz:%f, lx:%f ly:%f lz:%f\n", index[0]+8.0, index[1]+8.0, index[2]+8.0,
    //                                                    middle[0]+4.0, middle[1]+4.0, middle[2]+4.0,
    //                                                    ring[0], ring[1], ring[2],
    //                                                    little[0]-4.0, little[1]-4.0, little[2]-4.0);

    //HC06Printf("i:%f, m:%f, r:%f, l:%f\n", sindex+8.0, smiddle+4.0, sring, slittle-4.0);
    xQueueSend(queue, (void *)data, 0);
		printf("%d\n", millis()-prev_time);
		prev_time = millis();
    //delay(1);
	}
}


void dataProcessThread(void *arg);

void apInit()
{
	uartOpen(_DEF_UART1, 38400);

	QueueHandle_t raw_data_queue;
	raw_data_queue = xQueueCreate(960, sizeof(int16_t)*3*4);

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
  int16_t data[12];
	int16_t *thumb = &data[0];
	int16_t *index = &data[3];
	int16_t *middle = &data[6];
	int16_t *ring = &data[9];

  int32_t sthumb, sindex, smiddle, sring;

  float buffer[4*100];
  int idx = 0;

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
      /*
      buffer[idx++] = sindex;
      buffer[idx++] = smiddle;
      buffer[idx++] = sring;
      buffer[idx++] = slittle;
      */
      /*
      if (idx > 400)
      {
        idx = 0;
        for (int i = 0; i < 400; i+=4)
        {
          //HC06Printf("i:%f, m:%f, r:%f, l:%f\n", buffer[i]+8.0, buffer[i+1]+4.0, buffer[i+2], buffer[i+3]-4.0);
          HC06Printf("@%f,%f,%f,%f\n", buffer[i], buffer[i+1], buffer[i+2], buffer[i+3]);
        }
      }
      */
      HC06Printf("@%d,%d,%d,%d\n", sthumb, sindex, smiddle, sring);
			//printf("@%d,%d,%d,%d\n", sthumb, sindex, smiddle, sring);
      //printf("i:%f, m:%f, r:%f, l:%f\n", sindex+8.0, smiddle+4.0, sring, slittle-4.0);
			//printf("%d\n", millis()-prev_time);
			prev_time=millis();
			
		}
  }
}
