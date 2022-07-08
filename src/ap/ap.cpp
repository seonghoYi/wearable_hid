#include "ap.h"
#include "imu/imu.h"

/*
cIMU imu1(_DEF_MPU6050_1);
cIMU imu2(_DEF_MPU6050_2);
cIMU imu3(_DEF_MPU6050_3);
cIMU imu4(_DEF_MPU6050_4);
cIMU imu5(_DEF_MPU6050_5);
*/


void ledThread(void *args)
{
	while(1)
	{
		ledToggle(_DEF_LED1);
		vTaskDelay(100);
	}
}


void apInit()
{
	//uartOpen(_DEF_UART1, 38400);
	//cliOpen(_DEF_UART1, 38400);
	//i2cBegin(_DEF_I2C2, 400);
	//imuBegin(100);
	//HC06Begin(HC06_BAUD_115200);

	
	//HC06Printf("Starting IMU calibration.. Takes a second.\n");
	//imuBegin(_DEF_MPU6050_1, 100);
	//imuBegin(_DEF_MPU6050_2, 100);
	//imuBegin(_DEF_MPU6050_3, 100);
	//imuBegin(_DEF_MPU6050_4, 100);
	//imuBegin(_DEF_MPU6050_5, 100);
	//imu_tbl[_DEF_MPU6050_1].begin(100);
	//imu_tbl[_DEF_MPU6050_2].begin(100);
	//imu_tbl[_DEF_MPU6050_3].begin(100);
	//imu_tbl[_DEF_MPU6050_4].begin(100);
	//imu_tbl[_DEF_MPU6050_5].begin(100);
	/*
	imu1.begin(200);
	ledOn(_DEF_LED1);
	delay(10);
	ledOff(_DEF_LED1);
	imu2.begin(200);
	ledOn(_DEF_LED1);
	delay(10);
	ledOff(_DEF_LED1);
	imu3.begin(200);
	ledOn(_DEF_LED1);
	delay(10);
	ledOff(_DEF_LED1);
	imu4.begin(200);
	ledOn(_DEF_LED1);
	delay(10);
	ledOff(_DEF_LED1);
	imu5.begin(200);
	ledOn(_DEF_LED1);
	*/

	//HC06Printf("Calibration Done!!\n");

	xTaskCreate(ledThread, "ledThread", 256, NULL, 1, NULL);
}


void apMain()
{
	/*
	uint32_t pre_time = millis();
	uint32_t loop_us = micros();

	float quat[4], euler[3];
	int16_t *acc, *gyro;
	char buff[100];
	*/
	vTaskStartScheduler();
	while(1)	
	{
		//imuUpdate(_DEF_MPU6050_1);
		//imuUpdate(_DEF_MPU6050_2);
		//imuUpdate(_DEF_MPU6050_3);
		//imuUpdate(_DEF_MPU6050_4);
		//imuUpdate(_DEF_MPU6050_5);

		//imu_tbl[_DEF_MPU6050_1].update();
		//imu_tbl[_DEF_MPU6050_2].update();
		//imu_tbl[_DEF_MPU6050_3].update();
		//imu_tbl[_DEF_MPU6050_4].update();
		//imu_tbl[_DEF_MPU6050_5].update();
		/*
		imu1.update();
		imu2.update();
		imu3.update();
		imu4.update();
		imu5.update();

		if(millis() - pre_time >= 50)
		{
			printf("@");
			printf("%d %d %d %d %d %d %d,", 
									(int16_t)(imu2.ax*1000), (int16_t)(imu2.ay*1000), (int16_t)(imu2.az*1000), 
									(int16_t)(imu2.quat[0]*1000), (int16_t)(imu2.quat[1]*1000), (int16_t)(imu2.quat[2]*1000), (int16_t)(imu2.quat[3]*1000));
			
			printf("%d %d %d %d %d %d %d,", 
									(int16_t)(imu3.ax*1000), (int16_t)(imu3.ay*1000), (int16_t)(imu3.az*1000), 
									(int16_t)(imu3.quat[0]*1000), (int16_t)(imu3.quat[1]*1000), (int16_t)(imu3.quat[2]*1000), (int16_t)(imu3.quat[3]*1000));

			printf("%d %d %d %d %d %d %d,", 
									(int16_t)(imu4.ax*1000), (int16_t)(imu4.ay*1000), (int16_t)(imu4.az*1000), 
									(int16_t)(imu4.quat[0]*1000), (int16_t)(imu4.quat[1]*1000), (int16_t)(imu4.quat[2]*1000), (int16_t)(imu4.quat[3]*1000));

			printf("%d %d %d %d %d %d %d,", 
									(int16_t)(imu5.ax*1000), (int16_t)(imu5.ay*1000), (int16_t)(imu5.az*1000), 
									(int16_t)(imu5.quat[0]*1000), (int16_t)(imu5.quat[1]*1000), (int16_t)(imu5.quat[2]*1000), (int16_t)(imu5.quat[3]*1000));
									

			printf("%d %d %d %d %d %d %d\n", 
									(int16_t)(imu1.ax*1000), (int16_t)(imu1.ay*1000), (int16_t)(imu1.az*1000), 
									(int16_t)(imu1.quat[0]*1000), (int16_t)(imu1.quat[1]*1000), (int16_t)(imu1.quat[2]*1000), (int16_t)(imu1.quat[3]*1000));
			
			//printf("%d %d %d\n", gyro[0], gyro[1], gyro[2]);
			
			//printf("%d %d %d\n", gyro[0], gyro[1], gyro[2]);

			//HC06Printf("%d\n", millis());
			pre_time = millis();
		}
		*/
		//printf("1: %3.3f %3.3f %3.3f\n", imu1.rpy[0], imu1.rpy[1], imu1.rpy[2]);
		//imuGetRPY(_DEF_MPU6050_1, euler);
		//imu_tbl[_DEF_MPU6050_1].get_rpy(euler);
		/*
		printf("\033[2J");
		printf("\033[10;0H");
		imu1.get_rpy(euler);
		printf("0: %3.3f %3.3f %3.3f\n", euler[0], euler[1], euler[2]);
		imu2.get_rpy(euler);
		printf("1: %3.3f %3.3f %3.3f\n", euler[0], euler[1], euler[2]);
		imu3.get_rpy(euler);
		printf("2: %3.3f %3.3f %3.3f\n", euler[0], euler[1], euler[2]);
		imu4.get_rpy(euler);
		printf("3: %3.3f %3.3f %3.3f\n", euler[0], euler[1], euler[2]);
		imu5.get_rpy(euler);
		printf("4: %3.3f %3.3f %3.3f\n", euler[0], euler[1], euler[2]);
		*/
		/*
		if (millis() - pre_time >= 500)
		{
			ledToggle(_DEF_LED1);
			pre_time = millis();
		}
		
		imuUpdate();
		rpy = imuGetRPY();
		printf("%d, %d, %d\n", (int)rpy[0], (int)rpy[1], (int)rpy[2]);
		*/
		/*
		if(uartAvailable(_DEF_UART2))
		{
			uint8_t rx = uartRead(_DEF_UART2);
			//uartWrite(_DEF_UART2, &rx, 1);
			cliPrintf("%c\n", rx);
		}
		*/
		//cliMain();
	}
}