#include "tap_detect_ap.h"

#include <math.h>

#ifdef _USE_AP_TAPDETECT_AP


#define TAP_PI_THRESH
#define TAP_THRESH_HIGH_TICK_THRESH
#define TAP_THRESH_LOW_TICK_THRESH

QueueHandle_t data_queue = NULL;

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


typedef struct
{
  float index[3];
  float middle[3];
  float ring[3];
  float baby[3];
} fingerAccTypedef_t;


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

void dataProduceThread(void *arg);
void dataProcessThread(void *arg);

void tapDetectInit(void)
{
  data_queue = xQueueCreate(128, sizeof(fingerAccTypedef_t));

  if (data_queue != NULL)
  {
    printf("queue allocation success\n");
  }
  else
  {
    printf("queue allocation failed\n");
  }

  xTaskCreate(dataProduceThread, "dataProduceThread", 4*1024, NULL, 1, NULL);
  xTaskCreate(dataProcessThread, "dataProcessThread", 4*512, NULL, 2, NULL);
}



void dataProduceThread(void *arg)
{
  const uint32_t loop_hz = 100;
  uint32_t loop_time = (1/loop_hz) * 1000;
  float dt = (float)loop_time / 1000;


	adxl345Begin(_DEF_ADXL345_1);
	adxl345Begin(_DEF_ADXL345_2);
	adxl345Begin(_DEF_ADXL345_3);
	adxl345Begin(_DEF_ADXL345_4);

  fingerAccTypedef_t acc_data;

  filter_t filter[4][3];

  float sample_time = 1.0f/3200.0f;
  uint32_t prev_time = micros();
  while(1)
  {
    adxl345GetData(_DEF_ADXL345_1, acc_data.index);
	  adxl345GetData(_DEF_ADXL345_2, acc_data.middle);
		adxl345GetData(_DEF_ADXL345_3, acc_data.ring);
	  adxl345GetData(_DEF_ADXL345_4, acc_data.baby);
    printf("%d\n", micros()-prev_time);
    prev_time = micros();
    //if (data_queue != NULL)
    //{
      
      for (int i = 0; i < 3; i++)
      {
        acc_data.index[i] =  highPassFilter(&filter[0][i], acc_data.index[i], sample_time, 100.0);
        acc_data.middle[i] =  highPassFilter(&filter[1][i], acc_data.middle[i], sample_time, 100.0);
        acc_data.ring[i] =  highPassFilter(&filter[2][i], acc_data.ring[i], sample_time, 100.0);
        acc_data.baby[i] =  highPassFilter(&filter[3][i], acc_data.baby[i], sample_time, 100.0);
      }
      
      xQueueSend(data_queue, (void *)&acc_data, 0);
      //uartPrintf(_DEF_UART1, "@%f %f %f, %f %f %f, %f %f %f, %f %f %f\n", acc_data.index[0], acc_data.index[1], acc_data.index[2],
      //                                                    acc_data.middle[0], acc_data.middle[1], acc_data.middle[2],
      //                                                    acc_data.ring[0], acc_data.ring[1], acc_data.ring[2],
      //                                                    acc_data.baby[0], acc_data.baby[2], acc_data.baby[2]);
      //uartPrintf(_DEF_UART1, "%d", millis()-prev_time-loop_time);
      //printf("ix:%f iy:%f iz:%f, lx:%f ly:%f lz:%f\n", acc_data.index[0], acc_data.index[1], acc_data.index[2], acc_data.baby[0], acc_data.baby[2], acc_data.baby[2]);
    //}
    //delay(loop_time);
    //printf("%d\n", millis()-prev_time);
    //prev_time = millis();
  }
}


void tapDetectUpdate(tapDetectObjTypedef_t *p_tapObject, float acc_data[3]);

void dataProcessThread(void *arg)
{
  fingerAccTypedef_t data;
  tapDetectObjTypedef_t tapObject1;
  tapDetectObjTypedef_t tapObject2;
  tapDetectObjTypedef_t tapObject3;
  tapDetectObjTypedef_t tapObject4;

  tapObject1.state = TAP_DETECT_STATE_DATA_ACQ;
  tapObject2.state = TAP_DETECT_STATE_DATA_ACQ;
  tapObject3.state = TAP_DETECT_STATE_DATA_ACQ;
  tapObject4.state = TAP_DETECT_STATE_DATA_ACQ;

  printf("data process begin\n");

  uint32_t prev_time = millis();
  
  while(1)
  {
    if (data_queue != NULL)
    {
      if (xQueueReceive(data_queue, (void *)&data, 100) == pdPASS)
      {
        /*
        tapDetectUpdate(&tapObject1, data.index);
        tapDetectUpdate(&tapObject2, data.middle);
        tapDetectUpdate(&tapObject3, data.ring);
        tapDetectUpdate(&tapObject4, data.baby);


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
          printf("baby: %f\n", tapObject4.tapped_pi);
          //while(1);
        }
        */
       
        uartPrintf(_DEF_UART2, "ix:%f iy:%f iz:%f, mx:%f my:%f mz:%f, rx:%f ry:%f rz:%f, lx:%f ly:%f lz:%f\n", data.index[0]+8.0, data.index[1]+8.0, data.index[2]+8.0,
                                                          data.middle[0]+4.0, data.middle[1]+4.0, data.middle[2]+4.0,
                                                          data.ring[0], data.ring[1], data.ring[2],
                                                          data.baby[0]-4.0, data.baby[2]-4.0, data.baby[2]-4.0);                                                   
        //printf("%d\n", millis()-prev_time);
        prev_time=millis();
        
        //uartPrintf(_DEF_UART1, "@%d, %f %f %f, %f %f %f, %f %f %f, %f %f %f\n", millis(), data.index[0], data.index[1], data.index[2],
        //                                                  data.middle[0], data.middle[1], data.middle[2],
        //                                                  data.ring[0], data.ring[1], data.ring[2],
        //                                                  data.baby[0], data.baby[2], data.baby[2]);

        //delay(10);                                                      
        
        //printf("%f %f %f %f\n", tapObject1.performance_index, tapObject2.performance_index, tapObject3.performance_index, tapObject4.performance_index);

      }
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
    if (p_tapObject->performance_index < 4.0f && p_tapObject->performance_index > 2.7f)
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
      if (p_tapObject->thresh_high_tick <= 5 && p_tapObject->thresh_high_tick >= 2)
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



#endif