#include "accelerometer_handler.h"

#include <math.h>

#ifdef _USE_AP_ACCELEROMETER_HANDLER


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
}


#endif