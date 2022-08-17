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

	adxl345Begin(_DEF_ADXL345_1);
	adxl345Begin(_DEF_ADXL345_2);
	adxl345Begin(_DEF_ADXL345_3);
	adxl345Begin(_DEF_ADXL345_4);

  fingerAccTypedef_t acc_data;
  while(1)
  {
    adxl345GetData(_DEF_ADXL345_1, acc_data.index);
	  adxl345GetData(_DEF_ADXL345_2, acc_data.middle);
		adxl345GetData(_DEF_ADXL345_3, acc_data.ring);
	  adxl345GetData(_DEF_ADXL345_4, acc_data.baby);
    if (data_queue != NULL)
    {
      xQueueSend(data_queue, (void *)&acc_data, 0);
    }
    delay(loop_time);
  }
}


void tapDetectUpdate(tapDetectObjTypedef_t *p_tapObject, float acc_data[3]);

void dataProcessThread(void *arg)
{
  fingerAccTypedef_t data;
  fingerAccTypedef_t *p_data;


  tapDetectObjTypedef_t tapObject;
  tapObject.state = TAP_DETECT_STATE_DATA_ACQ;

  printf("data process begin\n");
  
  while(1)
  {
    if (data_queue != NULL)
    {
      if (xQueueReceive(data_queue, (void *)&data, 100) == pdPASS)
      {
        tapDetectUpdate(&tapObject, data.index);
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
      printf("%f\n", p_tapObject->performance_index);

      p_tapObject->state = TAP_DETECT_STATE_THRES_CHK;
  }
  else if (p_tapObject->state == TAP_DETECT_STATE_THRES_CHK)
  {
    if (p_tapObject->performance_index < 3.6f && p_tapObject->performance_index > 2.0f)
    {
      p_tapObject->thresh_low_tick = 0;
      p_tapObject->thresh_high_tick += 1;
      p_tapObject->state = TAP_DETECT_STATE_DATA_ACQ;
    }
    else
    {
      if (p_tapObject->thresh_high_tick+p_tapObject->thresh_low_tick > 0)
      {
        printf("%d, %d\n", p_tapObject->thresh_high_tick, p_tapObject->thresh_low_tick);
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
    if (p_tapObject->thresh_low_tick > 40)
    {
      while(1);
    }
    else
    {
      p_tapObject->state = TAP_DETECT_STATE_DATA_ACQ;
    }
  }




}



#endif