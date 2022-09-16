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
	float data[12];
	float *index = &data[0];
	float *middle = &data[3];
	float *ring = &data[6];
	float *little = &data[9];

	float sindex, smiddle, sring, slittle;

	filter_t filter[4][3];

	QueueHandle_t queue = (QueueHandle_t)arg;

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
			
			for (int i = 0; i < 3; i++)
			{
				index[i] = highPassFilter(&filter[0][i], index[i], 0.0003, 200.0);
				middle[i] = highPassFilter(&filter[1][i], middle[i], 0.0003, 200.0);
				ring[i] = highPassFilter(&filter[2][i], ring[i], 0.0003, 200.0);
				little[i] = highPassFilter(&filter[3][i], little[i], 0.0003, 200.0);
			}
			
			/*
			sindex = sqrtf(powf(index[0], 2) + powf(index[1], 2) + powf(index[2], 2));
			smiddle = sqrtf(powf(middle[0], 2) + powf(middle[1], 2) + powf(middle[2], 2));
			sring = sqrtf(powf(ring[0], 2) + powf(ring[1], 2) + powf(ring[2], 2));
			slittle = sqrtf(powf(little[0], 2) + powf(little[1], 2) + powf(little[2], 2));
			*/
			//uartPrintf(_DEF_UART2, "ix:%f iy:%f iz:%f, mx:%f my:%f mz:%f, rx:%f ry:%f rz:%f, lx:%f ly:%f lz:%f\n", index[0]+8.0, index[1]+8.0, index[2]+8.0,
      //                                                    middle[0]+4.0, middle[1]+4.0, middle[2]+4.0,
      //                                                    ring[0], ring[1], ring[2],
      //                                                    little[0]-4.0, little[1]-4.0, little[2]-4.0);

			//uartPrintf(_DEF_UART2, "i:%f, m:%f, r:%f, l:%f\n", sindex+8.0, smiddle+4.0, sring, slittle-4.0);
			xQueueSend(queue, (void *)data, 0);
			prev_time = millis();
		}
	}
}


void dataProcessThread(void *arg);

void apInit()
{
	uartOpen(_DEF_UART1, 38400);

	QueueHandle_t raw_data_queue;
	raw_data_queue = xQueueCreate(128, sizeof(float)*3*4);

	if (raw_data_queue == NULL)
	{
		printf("Queue allocation failed.\nSystem halted.\n");
		while(1);
	}

	xTaskCreate(threadLed, "threadLed", 128, NULL, 1, NULL);
	xTaskCreate(accelerometerThread, "accelerometerThread", 1024, (void *)raw_data_queue, 1, NULL);
	xTaskCreate(dataProcessThread, "dataProcessThread", 1024, (void *)raw_data_queue, 1, NULL);
}



void apMain()
{

	while(1)	
	{
		delay(1);
	}
}


typedef enum
{
  TAP_DETECT_STATE_DATA_ACQ = 0x00u,
  TAP_DETECT_STATE_OVERRUN_CHK,
  TAP_DETECT_STATE_CALC_PI,
  TAP_DETECT_STATE_THRES_CHK,
  TAP_DETECT_STATE_TAP_DETECT,
  TAP_DETECT_STATE_SINGLE_TAP,
  TAP_DETECT_STATE_DOUBLE_TAP,
  TAP_DETECT_STATE_TRIPLE_TAP,


} tapDetectStateTypedef_t;



typedef struct 
{
  bool step;
  uint8_t ch;
  uint8_t state;
  
  float acc[3];
  float prev_acc[3];
  float diff_acc[3];
  float performance_index;
  float tapped_pi;

  float threshold;

  uint32_t tap_count;

  uint32_t begin_tick;
  uint32_t event_start_tick;
  uint32_t thresh_high_tick;
  uint32_t thresh_low_tick;  
} tapDetectObjTypedef_t;

void tapDetectUpdate(tapDetectObjTypedef_t *p_tapObject, float acc_data[3]);



void dataProcessThread(void *arg)
{
  float data[12];
	float *index = &data[0];
	float *middle = &data[3];
	float *ring = &data[6];
	float *little = &data[9];


  tapDetectObjTypedef_t tapObject1;
  tapDetectObjTypedef_t tapObject2;
  tapDetectObjTypedef_t tapObject3;
  tapDetectObjTypedef_t tapObject4;

	QueueHandle_t queue = (QueueHandle_t)arg;

  tapObject1.state = TAP_DETECT_STATE_DATA_ACQ;
  tapObject2.state = TAP_DETECT_STATE_DATA_ACQ;
  tapObject3.state = TAP_DETECT_STATE_DATA_ACQ;
  tapObject4.state = TAP_DETECT_STATE_DATA_ACQ;

  uint32_t prev_time = millis();
  
  while(1)
  {
		if (xQueueReceive(queue, (void *)&data, 100) == pdPASS)
		{
			tapDetectUpdate(&tapObject1, index);
			tapDetectUpdate(&tapObject2, middle);
			tapDetectUpdate(&tapObject3, ring);
			tapDetectUpdate(&tapObject4, little);

      
			if (tapObject1.tap_count == 1)
			{
				printf("index: %f\n", tapObject1.tapped_pi);
				//while(1);
			}
			if (tapObject2.tap_count == 1)
			{
				printf("middle: %f\n", tapObject2.tapped_pi);
				//while(1);
			}
			if (tapObject3.tap_count == 1)
			{
				printf("ring: %f\n", tapObject3.tapped_pi);
				//while(1);
			}
			if (tapObject4.tap_count == 1)
			{
				printf("little: %f\n", tapObject4.tapped_pi);
				//while(1);
			}
      
			HC06Printf("%f %f %f %f\n", tapObject1.performance_index, tapObject2.performance_index, tapObject3.performance_index, tapObject4.performance_index);      
			//printf("%d\n", millis()-prev_time);
			prev_time=millis();
			
			//uartPrintf(_DEF_UART1, "@%d, %f %f %f, %f %f %f, %f %f %f, %f %f %f\n", millis(), data.index[0], data.index[1], data.index[2],
			//                                                  data.middle[0], data.middle[1], data.middle[2],
			//                                                  data.ring[0], data.ring[1], data.ring[2],
			//                                                  data.baby[0], data.baby[2], data.baby[2]);

		}
  }
}




void tapDetectUpdate(tapDetectObjTypedef_t *p_tapObject, float acc_data[3])
{

  if (p_tapObject->state == TAP_DETECT_STATE_DATA_ACQ)
  {
    if (p_tapObject->step == false)
    {
      p_tapObject->prev_acc[0] = acc_data[0];
      p_tapObject->prev_acc[1] = acc_data[1];
      p_tapObject->prev_acc[2] = acc_data[2];


      /*
       * Initialize variables
       */
      p_tapObject->thresh_low_tick = 0;
      p_tapObject->thresh_high_tick = 0;
      p_tapObject->tap_count = 0;

      p_tapObject->step = true;
      p_tapObject->state = TAP_DETECT_STATE_DATA_ACQ;
    }
    else
    {
      p_tapObject->acc[0] = acc_data[0];
      p_tapObject->acc[1] = acc_data[1];
      p_tapObject->acc[2] = acc_data[2];
      p_tapObject->state = TAP_DETECT_STATE_OVERRUN_CHK;
    }
    
  }
  else if (p_tapObject->state == TAP_DETECT_STATE_OVERRUN_CHK)
  {
    if (p_tapObject->thresh_high_tick >= 250)
    {
      p_tapObject->thresh_high_tick = 0;
    }
    p_tapObject->state = TAP_DETECT_STATE_CALC_PI;
  }
  else if (p_tapObject->state == TAP_DETECT_STATE_CALC_PI)
  {
      p_tapObject->diff_acc[0] = p_tapObject->acc[0] - p_tapObject->prev_acc[0];
      p_tapObject->diff_acc[1] = p_tapObject->acc[1] - p_tapObject->prev_acc[1];
      p_tapObject->diff_acc[2] = p_tapObject->acc[2] - p_tapObject->prev_acc[2];

      p_tapObject->performance_index = fabsf(p_tapObject->diff_acc[0]) + fabsf(p_tapObject->diff_acc[1]) + fabsf(p_tapObject->diff_acc[2]);
      //printf("%f\n", p_tapObject->performance_index);

      p_tapObject->state = TAP_DETECT_STATE_THRES_CHK;
  }
  else if (p_tapObject->state == TAP_DETECT_STATE_THRES_CHK)
  {
    if (p_tapObject->performance_index < 5.0f && p_tapObject->performance_index > 2.7f)
    {
      p_tapObject->tapped_pi =  p_tapObject->performance_index;
      p_tapObject->thresh_low_tick = 0;
      p_tapObject->thresh_high_tick += 1;
      p_tapObject->state = TAP_DETECT_STATE_DATA_ACQ;
    }
    else
    {
      if (p_tapObject->thresh_high_tick+p_tapObject->thresh_low_tick > 0)
      {
        //printf("%d, %d\n", p_tapObject->thresh_high_tick, p_tapObject->thresh_low_tick);
        p_tapObject->thresh_low_tick += 1;
        p_tapObject->state = TAP_DETECT_STATE_TAP_DETECT;
      }
      else
      {
        p_tapObject->state = TAP_DETECT_STATE_DATA_ACQ;
      }
    }
  }
  else if (p_tapObject->state == TAP_DETECT_STATE_TAP_DETECT)
  {
    if (p_tapObject->thresh_low_tick > 5)
    {
      if (p_tapObject->thresh_high_tick <= 20 && p_tapObject->thresh_high_tick >= 1)
      {
        //printf("tap\n");
        p_tapObject->tap_count = 1;
      }
      p_tapObject->step = false;
      p_tapObject->state = TAP_DETECT_STATE_DATA_ACQ;
      
      //while(1);
    }
    else
    {
      p_tapObject->state = TAP_DETECT_STATE_DATA_ACQ;
    }
  }




}