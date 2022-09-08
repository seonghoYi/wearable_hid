#include "accelerometer_handler.h"

#include <math.h>

#ifdef _USE_AP_ACCELEROMETER_HANDLER

typedef struct
{
	uint8_t filter_type;
	float prev_input;
	float prev_output;
} filter_t;


static filter_t filter[4][3];

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


bool accelerometerHandlerInit(void)
{
  bool ret = true;
	ret &= adxl345Begin(_DEF_ADXL345_1);
	ret &= adxl345Begin(_DEF_ADXL345_2);
	ret &= adxl345Begin(_DEF_ADXL345_3);
	ret &= adxl345Begin(_DEF_ADXL345_4);

  return ret;
}





void accelerometerRead(float data[12])
{
  float *index = &data[0];
  float *middle = &data[3];
  float *ring = &data[6];
  float *little = &data[9];

  adxl345GetData(_DEF_ADXL345_1, index);
  adxl345GetData(_DEF_ADXL345_2, middle);
  adxl345GetData(_DEF_ADXL345_3, ring);
  adxl345GetData(_DEF_ADXL345_4, little);

  for (int i = 0; i < 3; i++)
  {
    index[i] =  highPassFilter(&filter[0][i], index[i], 0.0003, 100.0);
    middle[i] =  highPassFilter(&filter[1][i], middle[i], 0.0003, 100.0);
    ring[i] =  highPassFilter(&filter[2][i], ring[i], 0.0003, 100.0);
    little[i] =  highPassFilter(&filter[3][i], little[i], 0.0003, 100.0);
  }
}


#endif