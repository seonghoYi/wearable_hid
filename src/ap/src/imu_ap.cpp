#include "imu_ap.h"




#ifdef _USE_AP_IMU_AP


#define MAX_QUEUE_SIZE    128
#define MAX_Q_ITEM_SIZE   sizeof(float) * 7

QueueHandle_t imu1_q = NULL;
QueueHandle_t imu2_q = NULL;
QueueHandle_t imu3_q = NULL;
QueueHandle_t imu4_q = NULL;
QueueHandle_t imu5_q = NULL;



void threadImu(void *arg)
{
  float data[7];
  while(1)
  {
    if (imu1_q != NULL)
    {
      xQueueReceive(imu1_q, (void *)data, 100);
      printf("imu1: %f, %f, %f  %f, %f, %f, %f\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
    }
    if (imu2_q != NULL)
    {
      xQueueReceive(imu2_q, (void *)data, 100);
      printf("imu2: %f, %f, %f  %f, %f, %f, %f\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
    }
    if (imu3_q != NULL)
    {
      xQueueReceive(imu3_q, (void *)data, 100);
      printf("imu3: %f, %f, %f  %f, %f, %f, %f\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
    }
    if (imu4_q != NULL)
    {
      xQueueReceive(imu4_q, (void *)data, 100);
      printf("imu4: %f, %f, %f  %f, %f, %f, %f\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
    }
    if (imu5_q != NULL)
    {
      xQueueReceive(imu5_q, (void *)data, 100);
      printf("imu5: %f, %f, %f  %f, %f, %f, %f\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
    }

  }





}


void threadImu1(void *arg)
{
  uint32_t prev_time = millis();
  while (imuBegin(_DEF_MPU6050_1, 50) != true);
  printf("imu1 begin %dms\n", millis()-prev_time);
  imu1_q = xQueueCreate(MAX_QUEUE_SIZE, MAX_Q_ITEM_SIZE);
  //prev_time = micros();
  prev_time = millis();
  while(1)
  {
    imuUpdate(_DEF_MPU6050_1);
    //printf("imu1: %dus\n", micros()-prev_time);
    //prev_time = micros();
    float acc[3], quat[4], data[7];
    imuGetAcc(_DEF_MPU6050_1, acc);
    imuGetQuaternion(_DEF_MPU6050_1, quat);

    for (int i = 0; i < 3; i++)
    {
      data[i] = acc[i];
    }

    for (int i = 4; i < 7; i++)
    {
      data[i] = quat[i-4];
    }

    if (millis()-prev_time>=20)
    {
      xQueueSend(imu1_q, (const void *)data, 10);
    }

    
    /*
    if (millis()-prev_time >= 1000)
    {

      printf("IMU1: %.3f, %.3f, %.3f\n", rpy[0], rpy[1], rpy[2]);
      prev_time = millis();
    }
    */
  }
}

void threadImu2(void *arg)
{
  uint32_t prev_time = millis();
  while (imuBegin(_DEF_MPU6050_2, 50) != true);
  printf("imu2 begin %dms\n", millis()-prev_time);
  imu2_q = xQueueCreate(MAX_QUEUE_SIZE, MAX_Q_ITEM_SIZE);
  //prev_time = micros();
  prev_time = millis();
  while(1)
  {
    imuUpdate(_DEF_MPU6050_2);
    //printf("imu2: %dus\n", micros()-prev_time);
    //prev_time = micros();
    float acc[3], quat[4], data[7];
    imuGetAcc(_DEF_MPU6050_2, acc);
    imuGetQuaternion(_DEF_MPU6050_2, quat);

    for (int i = 0; i < 3; i++)
    {
      data[i] = acc[i];
    }

    for (int i = 4; i < 7; i++)
    {
      data[i] = quat[i-4];
    }

    if (millis()-prev_time>=20)
    {
      xQueueSend(imu2_q, (const void *)data, 10);
    }
    
  }
}

void threadImu3(void *arg)
{
  uint32_t prev_time = millis();
  while (imuBegin(_DEF_MPU6050_3, 50) != true);
  printf("imu3 begin %dms\n", millis()-prev_time);
  //prev_time = micros();
  prev_time = millis();
  while(1)
  {
    imuUpdate(_DEF_MPU6050_3);
    //printf("imu3: %dus\n", micros()-prev_time);
    //prev_time = micros();
    float acc[3], quat[4], data[7];
    imuGetAcc(_DEF_MPU6050_3, acc);
    imuGetQuaternion(_DEF_MPU6050_3, quat);

    for (int i = 0; i < 3; i++)
    {
      data[i] = acc[i];
    }

    for (int i = 4; i < 7; i++)
    {
      data[i] = quat[i-4];
    }

    if (millis()-prev_time>=20)
    {
      xQueueSend(imu3_q, (const void *)data, 10);
    }
  }
}

void threadImu4(void *arg)
{
  uint32_t prev_time = millis();
  while (imuBegin(_DEF_MPU6050_4, 50) != true);
  printf("imu4 begin %dms\n", millis()-prev_time);
  prev_time = millis();
  while(1)
  {
    imuUpdate(_DEF_MPU6050_4);
    float acc[3], quat[4], data[7];
    imuGetAcc(_DEF_MPU6050_4, acc);
    imuGetQuaternion(_DEF_MPU6050_4, quat);

    for (int i = 0; i < 3; i++)
    {
      data[i] = acc[i];
    }

    for (int i = 4; i < 7; i++)
    {
      data[i] = quat[i-4];
    }

    if (millis()-prev_time>=20)
    {
      xQueueSend(imu4_q, (const void *)data, 10);
    }
  }
}

void threadImu5(void *arg)
{
  uint32_t prev_time = millis();
  if (imuBegin(_DEF_MPU6050_5, 50) != true)
  {
    while(1);
  }
  printf("imu5 begin %dms\n", millis()-prev_time);
  prev_time = millis();
  while(1)
  {
    imuUpdate(_DEF_MPU6050_5);
    float acc[3], quat[4], data[7];
    imuGetAcc(_DEF_MPU6050_5, acc);
    imuGetQuaternion(_DEF_MPU6050_5, quat);

    for (int i = 0; i < 3; i++)
    {
      data[i] = acc[i];
    }

    for (int i = 4; i < 7; i++)
    {
      data[i] = quat[i-4];
    }

    if (millis()-prev_time>=20)
    {
      xQueueSend(imu5_q, (const void *)data, 10);
    }
  }
}



#endif