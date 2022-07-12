#include "imu_ap.h"




#ifdef _USE_AP_IMU_AP

//SemaphoreHandle_t mutex = NULL;


void threadImu(void *arg)
{
  //mutex = xSemaphoreCreateMutex();
}


void threadImu1(void *arg)
{
  uint32_t prev_time = millis();
  if (imuBegin(_DEF_MPU6050_1, 100) != true)
  {
    while(1);
  }
  printf("imu1 begin %dms\n", millis()-prev_time);
  //prev_time = micros();
  prev_time = millis();
  while(1)
  {
    imuUpdate(_DEF_MPU6050_1);
    //printf("imu1: %dus\n", micros()-prev_time);
    //prev_time = micros();
    if (millis()-prev_time >= 1000)
    {
      float rpy[3];
      imuGetRPY(_DEF_MPU6050_1, rpy);
      printf("IMU1: %.3f, %.3f, %.3f\n", rpy[0], rpy[1], rpy[2]);
      prev_time = millis();
    }
  }
}

void threadImu2(void *arg)
{
  uint32_t prev_time = millis();
  if (imuBegin(_DEF_MPU6050_2, 100) != true)
  {
    while(1);
  }
  printf("imu2 begin %dms\n", millis()-prev_time);
  //prev_time = micros();
  prev_time = millis();
  while(1)
  {
    imuUpdate(_DEF_MPU6050_2);
    //printf("imu2: %dus\n", micros()-prev_time);
    //prev_time = micros();
    if (millis()-prev_time >= 1000)
    {
      float rpy[3];
      imuGetRPY(_DEF_MPU6050_2, rpy);
      printf("IMU2: %.3f, %.3f, %.3f\n", rpy[0], rpy[1], rpy[2]);
      prev_time = millis();
    }
    
  }
}

void threadImu3(void *arg)
{
  uint32_t prev_time = millis();
  if (imuBegin(_DEF_MPU6050_3, 100) != true)
  {
    while(1);
  }
  printf("imu3 begin %dms\n", millis()-prev_time);
  //prev_time = micros();
  prev_time = millis();
  while(1)
  {
    imuUpdate(_DEF_MPU6050_3);
    //printf("imu3: %dus\n", micros()-prev_time);
    //prev_time = micros();
    if (millis()-prev_time >= 1000)
    {
      float rpy[3];
      imuGetRPY(_DEF_MPU6050_3, rpy);
      printf("IMU3: %.3f, %.3f, %.3f\n", rpy[0], rpy[1], rpy[2]);
      prev_time = millis();
    }
  }
}

void threadImu4(void *arg)
{
  uint32_t prev_time = millis();
  if (imuBegin(_DEF_MPU6050_4, 100) != true)
  {
    while(1);
  }
  printf("imu4 begin %dms\n", millis()-prev_time);
  prev_time = millis();
  while(1)
  {
    imuUpdate(_DEF_MPU6050_4);
    if (millis()-prev_time >= 1000)
    {
      float rpy[3];
      imuGetRPY(_DEF_MPU6050_4, rpy);
      printf("IMU4: %.3f, %.3f, %.3f\n", rpy[0], rpy[1], rpy[2]);
      prev_time = millis();
    }
  }
}

void threadImu5(void *arg)
{
  uint32_t prev_time = millis();
  if (imuBegin(_DEF_MPU6050_5, 100) != true)
  {
    while(1);
  }
  printf("imu5 begin %dms\n", millis()-prev_time);
  prev_time = millis();
  while(1)
  {
    imuUpdate(_DEF_MPU6050_5);
    if (millis()-prev_time >= 1000)
    {
      float rpy[3];
      imuGetRPY(_DEF_MPU6050_5, rpy);
      printf("IMU5: %.3f, %.3f, %.3f\n", rpy[0], rpy[1], rpy[2]);
      prev_time = millis();
    }
  }
}



#endif