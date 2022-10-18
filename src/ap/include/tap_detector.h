#ifndef SRC_AP_INCLUDE_TAP_DETECTOR_H_
#define SRC_AP_INCLUDE_TAP_DETECTOR_H_


#include "ap_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _USE_AP_TAP_DETECTOR


typedef enum
{
  TAP_DETECTOR_STATE_DATA_ACQ = 0x00u,
  TAP_DETECTOR_STATE_OVERRUN_CHK,
  TAP_DETECTOR_STATE_CALC_PI,
  TAP_DETECTOR_STATE_THRES_CHK,
  TAP_DETECTOR_STATE_TAP_DETECT,
  TAP_DETECTOR_STATE_SINGLE_TAP,
  TAP_DETECTOR_STATE_DOUBLE_TAP,
  TAP_DETECTOR_STATE_TRIPLE_TAP,
} tapDetectorStateTypedef_t;


typedef struct 
{
  bool step;
  uint8_t ch;
  tapDetectorStateTypedef_t state;
  
  float acc;
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

} tapDetectorObjTypedef_t;



void tapDetectorInit(tapDetectorObjTypedef_t *p_tap_obj, float threshold);
bool tapDetectorUpdate(tapDetectorObjTypedef_t *p_tap_obj, float acc_data);



#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_AP_INCLUDE_TAP_DETECTOR_H_ */
