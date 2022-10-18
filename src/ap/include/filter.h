#ifndef SRC_AP_INCLUDE__FILTER_H_
#define SRC_AP_INCLUDE__FILTER_H_

#include "ap_def.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _USE_AP_FILTER


typedef struct
{
	float prev_input;
	float prev_output;
} filter_t;


void filterInit(filter_t *filter);
float filterLowPassFilter(filter_t *filter, float input, float sampling_time, float cutOffFreq);
float filterHighPassFilter(filter_t *filter, float input, float sampling_time, float cutOffFreq);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_AP_INCLUDE__FILTER_H_ */