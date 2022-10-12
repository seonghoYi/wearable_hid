#include "accelerometer_handler.h"

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
  float *thumb = &data[0];
  float *index = &data[3];
  float *middle = &data[6];
  float *ring = &data[9];

  adxl345GetData(_DEF_ADXL345_1, thumb);
  adxl345GetData(_DEF_ADXL345_2, index);
  adxl345GetData(_DEF_ADXL345_3, middle);
  adxl345GetData(_DEF_ADXL345_4, ring);
}

void accelerometerReadRaw(int16_t data[12])
{
  int16_t *thumb = &data[0];
  int16_t *index = &data[3];
  int16_t *middle = &data[6];
  int16_t *ring = &data[9];

  adxl345GetRawData(_DEF_ADXL345_1, thumb);
  adxl345GetRawData(_DEF_ADXL345_2, index);
  adxl345GetRawData(_DEF_ADXL345_3, middle);
  adxl345GetRawData(_DEF_ADXL345_4, ring);
}



#endif