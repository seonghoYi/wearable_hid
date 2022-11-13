#include "ap.h"
#include "accelerometer_handler.h"
#include "tap_detector.h"
#include "tap_process_handler.h"
#include "filter.h"

#ifdef __cplusplus
extern "C" {
#endif

void vApplicationMallocFailedHook( void )
{
	ledOn(_DEF_LED1);
	while(1);
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	ledOn(_DEF_LED1);
	while(1);
}

void vApplicationTickHook( void )
{

}

#ifdef __cplusplus
}
#endif





void threadLed(void *arg);
void tapDetectThread(void *arg);
void keyHandleThread(void *arg);

void apInit()
{
	QueueHandle_t tap_type_queue = xQueueCreate(128, sizeof(uint32_t));

	uartOpen(_DEF_UART1, 38400);

	xTaskCreate(threadLed, "threadLed", 128, NULL, 3, NULL);
	xTaskCreate(tapDetectThread, "tapDetectThread", 2*1024, (void *)tap_type_queue, 1, NULL);
	xTaskCreate(keyHandleThread, "keyHandleThread", 1024, (void *)tap_type_queue, 1, NULL);
}



void apMain()
{
	while(1)	
	{
		delay(1);
	}
}



void threadLed(void *arg)
{
	while(1)
	{
		ledToggle(_DEF_LED1);
		delay(200);
	}
	
}


void tapDetectThread(void *arg)
{
	float data[12];
	float *thumb = &data[0];
	float *index = &data[3];
	float *middle = &data[6];
	float *ring = &data[9];

	float sthumb, sindex, smiddle, sring;


	QueueHandle_t tap_type_queue = (QueueHandle_t)arg;

	uint8_t tap_flag = 0;

	filter_t filter[4][3];
	tapDetectorHandleTypedef_t tap_handle[4];
	tapProcessHandlerTypedef_t tap_process_handle;

	HC06Begin(HC06_BAUD_115200);
	accelerometerHandlerInit();

	tapDetectorInit(&tap_handle[0], 0.9f);
	tapDetectorInit(&tap_handle[1], 1.2f);
	tapDetectorInit(&tap_handle[2], 1.17f);
	tapDetectorInit(&tap_handle[3], 1.1f);

	tapProcessHandlerCreate(&tap_process_handle);

	for (int i = 0; i < 3; i++)
	{
		filterInit(&filter[0][i]);
		filterInit(&filter[1][i]);
		filterInit(&filter[2][i]);
		filterInit(&filter[3][i]);
	}

  float sample_time = 1.f/500.f;

	HC06Printf("\033[2J");
	HC06Printf("\033[1;1H");
	printf("\033[2J");
	printf("\033[1;1H");

	uint32_t prev_time = millis();
	while(1)
	{
    accelerometerRead(data);
    
    for (int i = 0; i < 3; i++)
    {
      thumb[i] = filterHighPassFilter(&filter[0][i], thumb[i], sample_time, 100.f);
      index[i] = filterHighPassFilter(&filter[1][i], index[i], sample_time, 100.f);
      middle[i] = filterHighPassFilter(&filter[2][i], middle[i], sample_time, 100.f);
      ring[i] = filterHighPassFilter(&filter[3][i], ring[i], sample_time, 100.f);
		}

		sthumb = sqrtf(powf(thumb[0], 2) + powf(thumb[1], 2) + powf(thumb[2], 2));
		sindex = sqrtf(powf(index[0], 2) + powf(index[1], 2) + powf(index[2], 2));
		smiddle = sqrtf(powf(middle[0], 2) + powf(middle[1], 2) + powf(middle[2], 2));
		sring = sqrtf(powf(ring[0], 2) + powf(ring[1], 2) + powf(ring[2], 2));


		//printf("%f, %f, %f, %f\n", sthumb+4, sindex+2, smiddle, sring-2);


		float thumb_org = sthumb;
		float index_org = sindex;
		float middle_org = smiddle;
		float ring_org = sring;

		tapDetectorStatusTyepdef_t thumb_status = tapDetectorUpdate(&tap_handle[0], sthumb);
		tapDetectorStatusTyepdef_t index_status = tapDetectorUpdate(&tap_handle[1], sindex);
		tapDetectorStatusTyepdef_t middle_status = tapDetectorUpdate(&tap_handle[2], smiddle);
		tapDetectorStatusTyepdef_t ring_status = tapDetectorUpdate(&tap_handle[3], sring);

		uint16_t flag = 0;
		flag |= thumb_status.is_tapped << 0;
		flag |= index_status.is_tapped << 1;
		flag |= middle_status.is_tapped << 2;
		flag |= ring_status.is_tapped << 3;

		bool flags[4];
		float intensities[4];
		flags[0] = thumb_status.is_tapped;
		flags[1] = index_status.is_tapped;
		flags[2] = middle_status.is_tapped;
		flags[3] = ring_status.is_tapped;
		intensities[0] = thumb_status.intensity_proportion;
		intensities[1] = index_status.intensity_proportion;
		intensities[2] = middle_status.intensity_proportion;
		intensities[3] = ring_status.intensity_proportion;
		
		uint32_t output = 0;

		if (tapDetectorProcess(&tap_process_handle, &output, flag) == true)
		{
			xQueueSend(tap_type_queue, (void *)&output, 0);

			/*
			HC06Printf("\033[2J");
      HC06Printf("\033[10;5H");
			HC06Printf("thumb index middle ring\n");
			HC06Printf("\033[11;5H");
			HC06Printf("  %d     %d     %d     %d\n", (output>>0)&0x03, (output>>2)&0x03, (output>>4)&0x03, (output>>6)&0x03);
			*/
		}

	}
}


static char single_tap_table[16] = {
	'\0', 'o', 'i', 'r', 'e', 'n', 't', 's', 'a', 'u', 'd', 'p', 'l', 'c', 'm', 'h'
};

static char double_tap_table[16] = {
	'\0', '\b', '\0', 'b', 'k', 'q', 'x', 'z', '\n', 'v', 'j', 'f', 'y', 'g', 'w', ' '
};

static void motorTimerCallback(TimerHandle_t pxTimer)
{
	gpioPinWrite(_DEF_GPIO1, false);
}

void keyHandleThread(void *arg)
{
	char key_code;
	
	bool is_tapped[4];
	uint32_t tap_data;
	uint8_t tap_code;
	
	QueueHandle_t tap_type_queue = (QueueHandle_t)arg;
	bool is_double_tap = false;


	TimerHandle_t motor_timer = xTimerCreate("motorTimer", pdMS_TO_TICKS(200), pdFALSE, NULL, motorTimerCallback);


	while(1)
	{
		if (xQueueReceive(tap_type_queue, (void *)&tap_data, portMAX_DELAY) == pdPASS)
		{
			for (int i=0; i<8; i+=2)
			{
				is_tapped[i/2] = (bool)((tap_data>>i)&0x03);
				if (((tap_data>>i)&0x03) / 2 == 1)
				{
					is_double_tap = true;
				}
			}

			tap_code = (is_tapped[0]<<3) | (is_tapped[1]<<2) | (is_tapped[2]<<1) | (is_tapped[3]<<0);

			printf("\n  %d     %d     %d     %d\n", (tap_data>>0)&0x03, (tap_data>>2)&0x03, (tap_data>>4)&0x03, (tap_data>>6)&0x03);
			printf("%04b\n", tap_code);

			if (is_double_tap == false)
			{
				key_code = single_tap_table[tap_code];
			}
			else
			{
				key_code = double_tap_table[tap_code];
			}


			if (key_code != '\0')
			{
				gpioPinWrite(_DEF_GPIO1, true);
				xTimerStart(motor_timer, 0);
				HC06Write((uint8_t *)&key_code, 1);
			}

			if (key_code == '\b')
			{
				gpioPinWrite(_DEF_GPIO1, true);
				xTimerStart(motor_timer, 0);
				HC06Printf("\x1B[1P");
			}
		}

		is_double_tap = false;
	}
}