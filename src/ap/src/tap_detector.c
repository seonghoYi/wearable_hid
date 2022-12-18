#include "tap_detector.h"


#ifdef _USE_AP_TAP_DETECTOR


static uint8_t handler_num = 0;


void tapDetectorInit(tapDetectorHandleTypedef_t *p_tap_handle, float threshold)
{
  p_tap_handle->ch = handler_num++;
  p_tap_handle->step = false;
  p_tap_handle->state = TAP_DETECTOR_STATE_DATA_ACQ;
  p_tap_handle->thresh_high_tick = 0;
  p_tap_handle->thresh_low_tick = 0;
  p_tap_handle->threshold = threshold;

  p_tap_handle->acc_sum = 0;
  p_tap_handle->sum_count = 0;

}


tapDetectorStatusTyepdef_t tapDetectorUpdate(tapDetectorHandleTypedef_t *p_tap_handle, float acc_data)
{
  tapDetectorStatusTyepdef_t ret = {false, 0.f};

	if (p_tap_handle->state == TAP_DETECTOR_STATE_DATA_ACQ)
  {
		if (p_tap_handle->step == false)
		{
			p_tap_handle->thresh_high_tick = 0;
			p_tap_handle->thresh_low_tick = 0;
			p_tap_handle->tap_count = 0;
			p_tap_handle->step = true;

      p_tap_handle->acc_sum = 0;
      p_tap_handle->sum_count = 0;
			p_tap_handle->state = TAP_DETECTOR_STATE_DATA_ACQ;
		}
		else
		{
			p_tap_handle->acc = acc_data;
			p_tap_handle->state = TAP_DETECTOR_STATE_OVERRUN_CHK;
		}
  }
  else if (p_tap_handle->state == TAP_DETECTOR_STATE_OVERRUN_CHK)
  {
    if (p_tap_handle->thresh_high_tick >= 50)
    {
      p_tap_handle->step = false;
      p_tap_handle->state = TAP_DETECTOR_STATE_DATA_ACQ;
    }
    p_tap_handle->state = TAP_DETECTOR_STATE_THRES_CHK;
  }
  else if (p_tap_handle->state == TAP_DETECTOR_STATE_THRES_CHK)
  {
    if (p_tap_handle->acc / p_tap_handle->threshold > 0.8)
    {
      p_tap_handle->acc_sum += p_tap_handle->acc;
      p_tap_handle->sum_count++;
    }

    if (p_tap_handle->acc > p_tap_handle->threshold)
    {
      p_tap_handle->thresh_low_tick = 0;
      p_tap_handle->thresh_high_tick += 1;
      p_tap_handle->state = TAP_DETECTOR_STATE_DATA_ACQ;
    }
    else
    {
      if (p_tap_handle->thresh_high_tick+p_tap_handle->thresh_low_tick > 0)
      {
        p_tap_handle->thresh_low_tick += 1;
        p_tap_handle->state = TAP_DETECTOR_STATE_TAP_DETECT;
      }
      else
      {
        p_tap_handle->state = TAP_DETECTOR_STATE_DATA_ACQ;
      }
    }
  }
  else if (p_tap_handle->state == TAP_DETECTOR_STATE_TAP_DETECT)
  {
    if (p_tap_handle->thresh_low_tick > 5)
    {
      if (p_tap_handle->thresh_high_tick > 0 && p_tap_handle->thresh_high_tick < 20)
      {
        p_tap_handle->tap_count = 1;
        ret.is_tapped = true;
      }
      p_tap_handle->step = false;
      p_tap_handle->state = TAP_DETECTOR_STATE_DATA_ACQ;
    }
    else
    {
      p_tap_handle->state = TAP_DETECTOR_STATE_DATA_ACQ;
    }
  }

  if (p_tap_handle->sum_count == 0)
  {
    ret.intensity_proportion = 0.f;  
  }
  else
  {
    ret.intensity_proportion = (p_tap_handle->acc_sum/p_tap_handle->sum_count)/p_tap_handle->threshold;
  }

  return ret;
}




#endif