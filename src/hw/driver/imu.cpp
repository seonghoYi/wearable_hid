#include "imu.h"
#include "imu/imu.h"

#include "cli.h"


#ifdef _USE_HW_IMU


static cIMU imu;

static void cliIMU(cli_args_t *args);

bool imuInit(void)
{
  bool ret = true;

#ifdef _USE_HW_CLI
  cliAdd("IMU", cliIMU);
#endif


  return ret;
}

bool imuBegin(uint32_t update_hz)
{
  bool ret = imu.begin(update_hz);
  return ret;
}


uint32_t imuUpdate()
{
  uint32_t ret = imu.update();

  return ret;
}


void imuGetRPY(float (&rpy)[3])
{
  imu.get_rpy(rpy);
}

void imuGetQuaternion(float (&quat)[4])
{
  imu.get_quat(quat);
}

void imuGetAcc(float (&acc)[3])
{
  imu.get_acc(acc);
}

void imuGetGyro(float (&gyro)[3])
{
  imu.get_gyro(gyro);
}



#ifdef _USE_HW_CLI


void cliIMU(cli_args_t *args)
{
  bool ret = true;
  uint32_t update_hz = 0;
  static uint32_t update_ms = 0;

  if (args->argc == 2)
  {
    if (args->isStr(0, "begin") == true)
    {
      update_hz = args->getData(1);
      ret = imuBegin(update_hz);
      if (update_hz == 0)
      {
        ret = false;
      }
      else
      {
        update_ms = 1000 / update_hz;
      }

      if (ret == true)
      {
        cliPrintf("success\r\n");
      }
      else
      {
        cliPrintf("fail\r\n");
      }
    }
    else if (args->isStr(0, "show") == true)
    {
      if (args->isStr(1, "rpy") == true)
      {
        while(cliKeepLoop())
        {
          imu.update();
          cliPrintf("\033[2J");
          cliPrintf("\033[10;5H");
          cliPrintf("roll: %+3.3f pitch: %+3.3f yaw: %+3.3f\r\n", imu.rpy[ROLL], imu.rpy[PITCH], imu.rpy[YAW]);
          delay(update_ms);
        }
      }
      else if (args->isStr(1, "quaternion") == true)
      {
        while(cliKeepLoop())
        {
          imu.update();
          cliPrintf("\033[2J");
          cliPrintf("\033[10;5H");
          cliPrintf("roll: %+3.3f pitch: %+3.3f yaw: %+3.3f\r\n", imu.rpy[ROLL], imu.rpy[PITCH], imu.rpy[YAW]);
          delay(update_ms);
        }
      }
      else if (args->isStr(1, "accel") == true)
      {
        while(cliKeepLoop())
        {
          imu.update();
          cliPrintf("\033[2J");
          cliPrintf("\033[10;5H");
          cliPrintf("x: %+d y: %+d z: %+d\r\n", imu.accData[ROLL], imu.accData[PITCH], imu.accData[YAW]);
          delay(update_ms);
        }
      }
      else if (args->isStr(1, "gyro") == true)
      {
        while(cliKeepLoop())
        {
          imu.update();
          cliPrintf("\033[2J");
          cliPrintf("\033[10;5H");
          cliPrintf("roll: %+d pitch: %+d yaw: %+d\r\n", imu.gyroData[ROLL], imu.gyroData[PITCH], imu.gyroData[YAW]);
          delay(update_ms);
        }
      }
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }


  if (ret == false)
  {
    cliPrintf("imu begin update_Hz\r\n");
    cliPrintf("imu show rpy\r\n");
    cliPrintf("imu show quaternion\r\n");
    cliPrintf("imu show accel\r\n");
    cliPrintf("imu show gyro\r\n");
  }
}


#endif

#endif