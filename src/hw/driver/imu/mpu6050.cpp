#include "imu/mpu6050.h"
#include "imu/mpu6050_regs.h"
#include "i2c.h"
#ifdef _USE_HW_MPU6050


#define MPU6050_I2C_ADDRESS 0x68 << 1


cMPU6050::cMPU6050()
{
  b_connected = false;
  i2c_addr = MPU6050_I2C_ADDRESS;
}

bool cMPU6050::init()
{
  bool ret = true;
  uint8_t data = 0;

  i2c_ch = _DEF_I2C1;
  ret &= i2cBegin(i2c_ch, 400);

  //mpu6050 reset
  ret &= regWrite(MPU6050_PWR_MGMT_1, MPU6050_RESET);
  delay(100);

  //mpu6050 wakeup
  ret &= regWrite(MPU6050_PWR_MGMT_1, 0x00);

  //mpu6050 acc 2g
  ret &= regWrite(MPU6050_ACCEL_CONFIG, MPU6050_ACCEL_FSR2 << 3); 

  //mpu6050 gyro 2000
  ret &= regWrite(MPU6050_GYRO_CONFIG, MPU6050_GYRO_FSR2000 << 3);

  //mpu6050 config DLPF ~20Hz
  ret &= regRead(MPU6050_CONFIG, &data);
  data |= MPU6050_CONFIG_DLFP4;
  ret &= regWrite(MPU6050_CONFIG, data);

  ret &= regWrite(MPU6050_FIFO_EN, 0x78);
  ret &= regWrite(MPU6050_USER_CTRL, 0x40);
 

  return ret;
}

bool cMPU6050::begin()
{
  bool ret = true;
  ret &= init();
  accInit();
  gyroInit();

  if (ret)
  {
    b_connected = true;
    return b_connected;
  }
  else
  {
    return ret;
  }
}


void cMPU6050::accInit()
{
  for (int axis = 0; axis < 3; axis++)
  {
    accRaw[axis] = 0;
    accData[axis] = 0;
  }
  accStartCali();
}

void cMPU6050::accGetData()
{
  int16_t x;
  int16_t y;
  int16_t z;

  uint8_t raw[6];

  if (b_connected == true)
  {
    regReads(MPU6050_ACCEL_XOUT_H, raw, 6);

    x = (((int16_t)raw[0]) << 8) | raw[1];
    y = (((int16_t)raw[2]) << 8) | raw[3];
    z = (((int16_t)raw[4]) << 8) | raw[5];


    accRaw[0] = x;
    accRaw[1] = y;
    accRaw[2] = z;

    accData[0] = x;
    accData[1] = y;
    accData[2] = z;
  }

  accCalibration();
}

void cMPU6050::gyroInit()
{
  for (int axis = 0; axis < 3; axis++)
  {
    gyroRaw[axis] = 0;
    gyroData[axis] = 0;
  }
  gyroStartCali();
}

void cMPU6050::gyroGetData()
{
  int16_t x;
  int16_t y;
  int16_t z;

  uint8_t raw[6];

  if (b_connected == true)
  {
    regReads(MPU6050_GYRO_XOUT_H, raw, 6);

    x = (((int16_t)raw[0]) << 8) | raw[1];
    y = (((int16_t)raw[2]) << 8) | raw[3];
    z = (((int16_t)raw[4]) << 8) | raw[5];

    gyroRaw[0] = x;
    gyroRaw[1] = y;
    gyroRaw[2] = z;

    gyroData[0] = x;
    gyroData[1] = y;
    gyroData[2] = z;
  }

  gyroCalibration();
}

void cMPU6050::accStartCali()
{
  calibrating_count_acc = MPU_CALI_COUNT;
}

void cMPU6050::gyroStartCali()
{
  calibrating_count_gyro = MPU_CALI_COUNT;
}

bool cMPU6050::accGetCaliDone()
{
  bool ret = false;

  if (calibrating_count_acc == 0)
  {
    ret = true;
  }

  return ret;
}

bool cMPU6050::gyroGetCaliDone()
{
  bool ret = false;

  if (calibrating_count_gyro == 0)
  {
    ret = true;
  }

  return ret;
}

void cMPU6050::accCalibration()
{
  //static int32_t a[3];

	if (calibrating_count_acc>0)
	{
		calibrating_count_acc--;
		for (uint8_t axis = 0; axis < 3; axis++)
		{
			a[axis] += accData[axis];             // Sum up 512 readings
			accZero[axis] = a[axis]>>9;          // Calculate average, only the last itteration where (calibratingA == 0) is relevant
		}
		if (calibrating_count_acc == 0)
		{
			//accZero[YAW] -= ACC_1G;
      accZero[YAW] = 0;
		}
	}

  accData[ROLL]  -=  accZero[ROLL] ;
  accData[PITCH] -=  accZero[PITCH];
  accData[YAW]   -=  accZero[YAW] ;
}

void cMPU6050::gyroCalibration()
{
	//static int16_t previousGyroData[3];
	//static int32_t g[3];

	memset(previousGyroData, 0, 3 * 2);

	if (calibrating_count_gyro>0)
	{
		for (int axis = 0; axis < 3; axis++)
		{
			if (calibrating_count_gyro == MPU_CALI_COUNT)
			{ // Reset g[axis] at start of calibration
				g[axis]=0;
				previousGyroData[axis] = gyroData[axis];
			}
			if (calibrating_count_gyro % 10 == 0)
			{
				//if(abs(gyroADC[axis] - previousGyroADC[axis]) > 8) tilt=1;

				previousGyroData[axis] = gyroData[axis];
			}
			g[axis] += gyroData[axis]; // Sum up 512 readings
			gyroZero[axis]=g[axis]>>9;

			if (calibrating_count_gyro == 1)
			{
				//SET_ALARM_BUZZER(ALRM_FAC_CONFIRM, ALRM_LVL_CONFIRM_ELSE);
			}
		}

		calibrating_count_gyro--;
		return;
	}

  for (int axis = 0; axis < 3; axis++)
  {
    gyroData[axis] -= gyroZero[axis];

    //anti gyro glitch, limit the variation between two consecutive readings
    //gyroADC[axis] = constrain(gyroADC[axis],previousGyroADC[axis]-800,previousGyroADC[axis]+800);
    previousGyroData[axis] = gyroData[axis];
 
  }
}


bool cMPU6050::regWrite(uint16_t reg_addr, uint8_t data)
{
  bool ret = true;

  ret &= i2cMemWrite(i2c_ch, i2c_addr, reg_addr, 1, data);
  return ret;
}

bool cMPU6050::regRead(uint16_t reg_addr, uint8_t *p_data)
{
  bool ret = true;

  ret &= i2cMemRead(i2c_ch, i2c_addr, reg_addr, 1, p_data);
  return ret;
}

bool cMPU6050::regReads(uint16_t reg_addr, uint8_t *p_data, uint16_t size)
{
  bool ret = true;

  ret &= i2cMemReads(i2c_ch, i2c_addr, reg_addr, 1, p_data, size);
  return ret;
}

#endif