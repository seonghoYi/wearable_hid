#include "imu/imu.h"




cIMU::cIMU()
{
  for (int i = 0; i< 3; i++)
  {
    rpy[i] = 0.0f;
  }

  b_connected = false;
}

bool cIMU::begin(uint32_t hz)
{

  update_hz = hz;
  update_us = 1000000 / hz;

  accRes = 2.0/32768.0;     //2g
  gyrRes = 2000.0/32768.0;  // 2000 dps



  b_connected = sensor.begin();


  if (b_connected == true)
  {
    filter.begin(update_hz);

    while(!sensor.gyroGetCaliDone())
    {
      update();
    }
  }

  

  return b_connected;
}


uint32_t cIMU::update()
{
  uint16_t ret_time = 0;

	static uint32_t tTime;


	if( (micros()-tTime) >= update_us )
	{
		ret_time = micros()-tTime;
    tTime = micros();

		computeIMU();
    
		gyroData[0] = sensor.gyroData[0];
		gyroData[1] = sensor.gyroData[1];
		gyroData[2] = sensor.gyroData[2];

    gyroRaw[0]  = sensor.gyroRaw[0];
    gyroRaw[1]  = sensor.gyroRaw[1];
    gyroRaw[2]  = sensor.gyroRaw[2];

    accData[0]  = sensor.accData[0];
    accData[1]  = sensor.accData[1];
    accData[2]  = sensor.accData[2];

    accRaw[0]   = sensor.accRaw[0];
    accRaw[1]   = sensor.accRaw[1];
    accRaw[2]   = sensor.accRaw[2];
	}

	return ret_time;
}


#define FILTER_NUM    3

void cIMU::computeIMU()
{
  static uint32_t prev_process_time = micros();
  static uint32_t cur_process_time = 0;
  static uint32_t process_time = 0;
  uint32_t i;
  static int32_t gyroADC[3][FILTER_NUM] = {0,};
  int32_t gyroAdcSum;

  uint32_t axis;

	sensor.accGetData();
	sensor.gyroGetData();

  for (axis = 0; axis < 3; axis++)
  {
    gyroADC[axis][0] = sensor.gyroData[axis];


    gyroAdcSum = 0;
    for (i=0; i<FILTER_NUM; i++)
    {
      gyroAdcSum += gyroADC[axis][i];
    }
    sensor.gyroData[axis] = gyroAdcSum/FILTER_NUM;
    for (i=FILTER_NUM-1; i>0; i--)
    {
      gyroADC[axis][i] = gyroADC[axis][i-1];
    }

    if (abs(sensor.gyroData[axis]) <= 3)
    {
      sensor.gyroData[axis] = 0;
    }
  }


  for( i=0; i<3; i++ )
  {
    accRaw[i]   = sensor.accRaw[i];
    accData[i]  = sensor.accData[i];
    gyroRaw[i]  = sensor.gyroRaw[i];
    gyroData[i] = sensor.gyroData[i];
  }

  ax = (float)sensor.accData[0]*accRes;
  ay = (float)sensor.accData[1]*accRes;
  az = (float)sensor.accData[2]*accRes;

  gx = (float)sensor.gyroData[0]*gyrRes;
  gy = (float)sensor.gyroData[1]*gyrRes;
  gz = (float)sensor.gyroData[2]*gyrRes;


  cur_process_time  = micros();
  process_time      = cur_process_time-prev_process_time;
  prev_process_time = cur_process_time;

  if (sensor.gyroGetCaliDone() == true && sensor.accGetCaliDone() == true)
  {
    filter.invSampleFreq = (float)process_time/1000000.0f;
    filter.updateIMU(gx, gy, gz, ax, ay, az);
  }


  rpy[0] = filter.getRoll();
  rpy[1] = filter.getPitch();
  rpy[2] = filter.getYaw()-180.;

  quat[0] = filter.q0;
  quat[1] = filter.q1;
  quat[2] = filter.q2;
  quat[3] = filter.q3;

  angle[0] = (int16_t)(rpy[0] * 10.);
  angle[1] = (int16_t)(rpy[1] * 10.);
  angle[2] = (int16_t)(rpy[1] * 1.);
}