#include "tap_detector.h"


#ifdef _USE_AP_TAP_DETECTOR


void tapDetectorInit(tapDetectorObjTypedef_t *p_tap_obj, float threshold)
{
  p_tap_obj->step = false;
  p_tap_obj->state = TAP_DETECTOR_STATE_DATA_ACQ;
  p_tap_obj->thresh_high_tick = 0;
  p_tap_obj->thresh_low_tick = 0;
  p_tap_obj->threshold = threshold;

}


bool tapDetectorUpdate(tapDetectorObjTypedef_t *p_tap_obj, float acc_data)
{
  bool ret = false;
	//printf("state: %d\n", p_tap_obj->state);
	if (p_tap_obj->state == TAP_DETECTOR_STATE_DATA_ACQ)
  {
		if (p_tap_obj->step == false)
		{
			p_tap_obj->thresh_high_tick = 0;
			p_tap_obj->thresh_low_tick = 0;
			p_tap_obj->tap_count = 0;
			p_tap_obj->step = true;
			p_tap_obj->state = TAP_DETECTOR_STATE_DATA_ACQ;
		}
		else
		{
			p_tap_obj->acc = acc_data;
			p_tap_obj->state = TAP_DETECTOR_STATE_OVERRUN_CHK;
		}
  }
  else if (p_tap_obj->state == TAP_DETECTOR_STATE_OVERRUN_CHK)
  {
    if (p_tap_obj->thresh_high_tick >= 250)
    {
      p_tap_obj->thresh_high_tick = 0;
    }
    p_tap_obj->state = TAP_DETECTOR_STATE_THRES_CHK;
  }
  else if (p_tap_obj->state == TAP_DETECTOR_STATE_THRES_CHK)
  {
    if (p_tap_obj->acc > p_tap_obj->threshold)
    {
      p_tap_obj->thresh_low_tick = 0;
      p_tap_obj->thresh_high_tick += 1;
      p_tap_obj->state = TAP_DETECTOR_STATE_DATA_ACQ;
    }
    else
    {
      if (p_tap_obj->thresh_high_tick+p_tap_obj->thresh_low_tick > 0)
      {
        //printf("%d, %d\n", p_tap_obj->thresh_high_tick, p_tap_obj->thresh_low_tick);
				printf("L: %d, H: %d\n", p_tap_obj->thresh_low_tick, p_tap_obj->thresh_high_tick);
        p_tap_obj->thresh_low_tick += 1;
        p_tap_obj->state = TAP_DETECTOR_STATE_TAP_DETECT;
      }
      else
      {
        p_tap_obj->state = TAP_DETECTOR_STATE_DATA_ACQ;
      }
    }
  }
  else if (p_tap_obj->state == TAP_DETECTOR_STATE_TAP_DETECT)
  {
    if (p_tap_obj->thresh_low_tick > 5)
    {
      if (p_tap_obj->thresh_high_tick > 0 && p_tap_obj->thresh_high_tick < 3)
      {
        //printf("tap\n");
        p_tap_obj->tap_count = 1;
        ret = true;
      }
      p_tap_obj->step = false;
      p_tap_obj->state = TAP_DETECTOR_STATE_DATA_ACQ;
    }
    else
    {
      p_tap_obj->state = TAP_DETECTOR_STATE_DATA_ACQ;
    }
  }
  return ret;
}







#endif