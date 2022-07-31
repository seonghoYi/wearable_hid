#include "tap_detect_ap.h"



#ifdef _USE_AP_TAPDETECT_AP

QueueHandle_t data_queue = NULL;


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


void dataProcessThread(void *arg)
{
  fingerAccTypedef_t data;
  fingerAccTypedef_t *p_data;
  while(1)
  {
    if (data_queue != NULL)
    {
      if (xQueueReceive(data_queue, (void *)&data, 100) == pdPASS)
      {
        p_data = &data;
        printf("1: %f, %f, %f\n", p_data->index[0], p_data->index[1], p_data->index[2]);
        printf("2: %f, %f, %f\n", p_data->middle[0], p_data->middle[1], p_data->middle[2]);
        printf("3: %f, %f, %f\n", p_data->ring[0], p_data->ring[1], p_data->ring[2]);
        printf("4: %f, %f, %f\n", p_data->baby[0], p_data->baby[1], p_data->baby[2]);
      }
    }
    
  }
}





#endif