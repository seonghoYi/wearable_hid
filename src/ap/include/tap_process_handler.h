#ifndef SRC_AP_INCLUDE_TAP_PROCESS_HANDLER_H_
#define SRC_AP_INCLUDE_TAP_PROCESS_HANDLER_H_


#include "ap_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _USE_AP_TAP_PROCESS_HANDLER


typedef enum
{
  TAP_PROCESS_STATE_INIT = 0x00,
  TAP_PROCESS_STATE_BEGIN,
  TAP_PROCESS_STATE_TAP_DETECTED,
  TAP_PROCESS_STATE_TAP_DOUBLE_TAP,
  TAP_PROCESS_STATE_EXPIRED,
} tapProcessStateTypedef_t;

typedef struct 
{
  tapProcessStateTypedef_t state;
  
  uint8_t tap_state_flag[16];

  uint32_t tapped_time;
  uint32_t double_tap_time;

} tapProcessHandlerTypedef_t;

void tapProcessHandlerCreate(tapProcessHandlerTypedef_t *p_handle);
bool tapDetectorProcess(tapProcessHandlerTypedef_t *p_handle, uint32_t *ret_flag_value, uint16_t input_flag_value);
bool tapDetectorProcessTest(tapProcessHandlerTypedef_t *p_handle, uint32_t *ret_flag_value, bool *input_flag_values, float *intensities, uint8_t sensor_num);
#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_AP_INCLUDE_TAP_PROCESS_HANDLER_H_ */
