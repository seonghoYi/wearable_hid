#include "filter.h"
#include <math.h>

#ifdef _USE_AP_FILTER

void filterInit(filter_t *filter)
{
  filter->prev_input = 0.;
  filter->prev_output = 0.;
}

float filterLowPassFilter(filter_t *filter, float input, float sampling_time, float cutOffFreq)
{
	float output;
	float wc = 2*M_PI*cutOffFreq; // 차단 각주파수
	float tau = 1/wc;


	output = (tau*filter->prev_output + sampling_time*input)/(tau+sampling_time);
	filter->prev_output = output; //나중을 위한 출력값 저장
	return output;
}


float filterHighPassFilter(filter_t *filter, float input, float sampling_time, float cutOffFreq)
{
	float output;
	float wc = 2*M_PI*cutOffFreq; // 차단 각주파수
	float tau = 1/wc;


	output = (tau*filter->prev_output + tau*(input-filter->prev_input))/(tau+sampling_time);
	filter->prev_output = output;
	filter->prev_input = input;
	return output;
}

#endif