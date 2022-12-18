#include "tap_process_handler.h"



#ifdef _USE_AP_TAP_PROCESS_HANDLER


void tapProcessHandlerCreate(tapProcessHandlerTypedef_t *p_handle)
{
  p_handle->state = TAP_PROCESS_STATE_BEGIN;
  for(int i =0 ; i <16; i++)
  {
    p_handle->tap_state_flag[i] = 0;
  }
}

bool tapDetectorProcess(tapProcessHandlerTypedef_t *p_handle, uint32_t *ret_flag_value, uint16_t input_flag_value)
{
  bool ret = false;
  bool is_double_tapped = false;
  //printf("%d\n", p_handle->state);
  //printf("%d\n", input_flag_value);
  //printf("%d\n", tap_obj.tap_state_flag);

  if (p_handle->state == TAP_PROCESS_STATE_BEGIN)
  {
    if (input_flag_value != 0)
    {
      p_handle->tapped_time = millis();
      for (int i = 0; i < 16; i++)
      {
        p_handle->tap_state_flag[i] = (uint8_t)(input_flag_value>>i)&0x01;
      }
      p_handle->state = TAP_PROCESS_STATE_TAP_DETECTED;
    }
  }
  else if (p_handle->state == TAP_PROCESS_STATE_TAP_DETECTED)
  {
    if (millis()-p_handle->tapped_time >= 300)
    {
      //printf("1 expired\n");
      p_handle->state = TAP_PROCESS_STATE_EXPIRED;
    }
    else
    {
      for (int i = 0; i < 16; i++)
      {
        p_handle->tap_state_flag[i] += (uint8_t)(input_flag_value>>i)&0x01;
      }
      

      is_double_tapped = false;
      for (int i = 0; i < 16; i++)
      {
        if (p_handle->tap_state_flag[i] == 2)
        {
          is_double_tapped = true;
        }
      }

      if (is_double_tapped == true)
      {
        p_handle->double_tap_time = millis();
        p_handle->state = TAP_PROCESS_STATE_TAP_DOUBLE_TAP;
      }
    }
  }
  else if (p_handle->state==TAP_PROCESS_STATE_TAP_DOUBLE_TAP)
  {
    if (millis()-p_handle->double_tap_time >= 200)
    {
      //printf("2 expired\n");
      for (int i = 0; i < 16; i++)
      {
        if (p_handle->tap_state_flag[i] == 1)
        {
          p_handle->tap_state_flag[i] += 1;
        }
      }


      p_handle->state = TAP_PROCESS_STATE_EXPIRED;
    }
    else
    {
      for (int i = 0; i < 16; i++)
      {
        if (p_handle->tap_state_flag[i] < 2)
        {
          p_handle->tap_state_flag[i] += (uint8_t)(input_flag_value>>i)&0x01;
        }
      }

    }
  }
  else if (p_handle->state==TAP_PROCESS_STATE_EXPIRED)
  {
    ret = true;
    
    for (int i = 0; i < 16; i++)
    {
      *ret_flag_value |= p_handle->tap_state_flag[i] << i*2;
    }
    
    p_handle->state = TAP_PROCESS_STATE_BEGIN;
  }

  return ret;
}

bool tapDetectorProcessTest(tapProcessHandlerTypedef_t *p_handle, uint32_t *ret_flag_value, bool *input_flag_values, float *intensities, uint8_t sensor_num)
{
  bool ret = false;
  bool is_double_tapped = false;

  if (p_handle->state == TAP_PROCESS_STATE_BEGIN)
  {
    bool input_flag = false;
    for (int i = 0; i < sensor_num; i++)
    {
      input_flag |= input_flag_values[i];
    }
    if (input_flag == true)
    {
      p_handle->tapped_time = millis();
      for (int i = 0; i < sensor_num; i++)
      {
        p_handle->tap_state_flag[i] = (uint8_t)input_flag_values[i];
      }
      p_handle->state = TAP_PROCESS_STATE_TAP_DETECTED;
    }
  }
  else if (p_handle->state == TAP_PROCESS_STATE_TAP_DETECTED)
  {
    if (millis()-p_handle->tapped_time >= 200)
    {
      p_handle->state = TAP_PROCESS_STATE_EXPIRED;
    }
    else
    {
      for (int i = 0; i < sensor_num; i++)
      {
        if (input_flag_values[i] == true)
        {
          p_handle->tap_state_flag[i] += 1;
        }
        else if (p_handle->tap_state_flag[i] == 1 && intensities[i] > 0.85 && input_flag_values[i] == false)
        {
          p_handle->tap_state_flag[i] += 1;
        }
      }
      

      is_double_tapped = false;
      for (int i = 0; i < sensor_num; i++)
      {
        if (p_handle->tap_state_flag[i] == 2)
        {
          is_double_tapped = true;
        }
      }

      if (is_double_tapped == true)
      {
        p_handle->double_tap_time = millis();
        p_handle->state = TAP_PROCESS_STATE_TAP_DOUBLE_TAP;
      }
    }
  }
  else if (p_handle->state==TAP_PROCESS_STATE_TAP_DOUBLE_TAP)
  {
    if (millis()-p_handle->double_tap_time >= 20)
    {
      for (int i = 0; i < sensor_num; i++)
      {
        if (p_handle->tap_state_flag[i] == 1)
        {
          p_handle->tap_state_flag[i] += 1;
        }
      }


      p_handle->state = TAP_PROCESS_STATE_EXPIRED;
    }
    else
    {
      for (int i = 0; i < sensor_num; i++)
      {
        if (p_handle->tap_state_flag[i] < 2)
        {
          p_handle->tap_state_flag[i] += (uint8_t)input_flag_values[i];
        }
      }

    }
  }
  else if (p_handle->state==TAP_PROCESS_STATE_EXPIRED)
  {
    ret = true;
    
    for (int i = 0; i < sensor_num; i++)
    {
      *ret_flag_value |= p_handle->tap_state_flag[i] << i*2;
    }
    
    p_handle->state = TAP_PROCESS_STATE_BEGIN;
  }

  return ret;
}
#endif