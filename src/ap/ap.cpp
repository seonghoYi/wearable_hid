#include "ap.h"
#include "accelerometer_handler.h"

#include <math.h>

#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

//#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"

#include "model_v1_quantized.h"
#include "model_v2_quantized.h"

	
#ifdef __cplusplus
extern "C" {
#endif

void vApplicationMallocFailedHook( void )
{
	printf("malloc failed\n");
	while(1);
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	printf("stack overflowed\n");
	while(1);
}

void vApplicationTickHook( void )
{

}

#ifdef __cplusplus
}
#endif


void threadLed(void *arg)
{
	while(1)
	{
		ledToggle(_DEF_LED1);
		delay(1000);
	}
	
}


void accelerometerThread(void *arg)
{
	int16_t data[12];

	QueueHandle_t queue = (QueueHandle_t)arg;

	HC06Begin(HC06_BAUD_115200);
	accelerometerHandlerInit();
	uint32_t prev_time = millis();

  //float sample_time = 1.f/3200.f;
	while(1)
	{
    accelerometerReadRaw(data);

    //uartPrintf(_DEF_UART2, "ix:%f iy:%f iz:%f, mx:%f my:%f mz:%f, rx:%f ry:%f rz:%f, lx:%f ly:%f lz:%f\n", index[0]+8.0, index[1]+8.0, index[2]+8.0,
    //                                                    middle[0]+4.0, middle[1]+4.0, middle[2]+4.0,
    //                                                    ring[0], ring[1], ring[2],
    //                                                    little[0]-4.0, little[1]-4.0, little[2]-4.0);

    //HC06Printf("i:%f, m:%f, r:%f, l:%f\n", sindex+8.0, smiddle+4.0, sring, slittle-4.0);
    xQueueSend(queue, (void *)data, 0);
		//printf("%d\n", millis()-prev_time);
		prev_time = millis();
    //delay(1);
	}
}


void dataProcessThread(void *arg);

void apInit()
{
	uartOpen(_DEF_UART1, 38400);

	QueueHandle_t raw_data_queue;
	raw_data_queue = xQueueCreate(960, sizeof(int16_t)*3*4);

	if (raw_data_queue == NULL)
	{
		printf("Queue allocation failed.\nSystem halted.\n");
		while(1);
	}

	xTaskCreate(threadLed, "threadLed", 128, NULL, 3, NULL);
	xTaskCreate(accelerometerThread, "accelerometerThread", 1024, (void *)raw_data_queue, 1, NULL);
	xTaskCreate(dataProcessThread, "dataProcessThread", 4*1024, (void *)raw_data_queue, 2, NULL);
}



void apMain()
{

	while(1)	
	{
		delay(1);
	}
}


void dataProcessThread(void *arg)
{
  int16_t data[12];
	int16_t *thumb = &data[0];
	int16_t *index = &data[3];
	int16_t *middle = &data[6];
	int16_t *ring = &data[9];

  int32_t sthumb, sindex, smiddle, sring;

  float buffer[4][150];
  int idx = 0;

	QueueHandle_t queue = (QueueHandle_t)arg;

  uint32_t prev_time = millis();



	printf("running...\n");

	tflite::ErrorReporter *error_reporter = nullptr;
	const tflite::Model *model = nullptr;
	tflite::MicroInterpreter *interpreter = nullptr;
	TfLiteTensor *model_input = nullptr;

	constexpr int kTensorArenaSize = 60 * 1024;
	uint8_t tensor_arena[kTensorArenaSize];


	static tflite::MicroErrorReporter micro_error_reporter;
	error_reporter = &micro_error_reporter;

	model = tflite::GetModel(model_v2_quantized_tflite);

	if (model->version() != TFLITE_SCHEMA_VERSION)
	{
		error_reporter->Report(
			"Model provided is schema version %d not equal "
			"to supported version %d.\n",
			model->version(), TFLITE_SCHEMA_VERSION
		);
		while(1);
	}

	static tflite::MicroMutableOpResolver<8> micro_op_resolver;
	micro_op_resolver.AddConv2D();
	micro_op_resolver.AddMaxPool2D();
	micro_op_resolver.AddFullyConnected();
	micro_op_resolver.AddLogistic();
	micro_op_resolver.AddRelu();
	micro_op_resolver.AddQuantize();
	micro_op_resolver.AddDequantize();
	micro_op_resolver.AddReshape();


  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

	interpreter->AllocateTensors();


	model_input = interpreter->input(0);
	printf("%d, %d, %d, %d, %d\n", model_input->dims->size, model_input->dims->data[0], model_input->dims->data[1], model_input->dims->data[2], model_input->dims->data[3]);
	printf("%d, %d, %d\n", interpreter->output(0)->dims->size, interpreter->output(0)->dims->data[0], interpreter->output(0)->dims->data[1]);
	int input_length = model_input->bytes / sizeof(float);

	
  
  while(1)
  {
		if (xQueueReceive(queue, (void *)&data, portMAX_DELAY) == pdPASS)
		{
      sthumb = sqrtf(powf(thumb[0], 2) + powf(thumb[1], 2) + powf(thumb[2], 2));
			sindex = sqrtf(powf(index[0], 2) + powf(index[1], 2) + powf(index[2], 2));
			smiddle = sqrtf(powf(middle[0], 2) + powf(middle[1], 2) + powf(middle[2], 2));
			sring = sqrtf(powf(ring[0], 2) + powf(ring[1], 2) + powf(ring[2], 2));
      
      buffer[0][idx] = sthumb / 1773.62f;
      buffer[1][idx] = sindex / 1773.62f;
      buffer[2][idx] = smiddle / 1773.62f;
      buffer[3][idx++] = sring / 1773.62f;
      
      if (idx >= 150)
      {
        idx = 0;
        //for (int i = 0; i < 400; i+=4)
        //{
          //HC06Printf("i:%f, m:%f, r:%f, l:%f\n", buffer[i]+8.0, buffer[i+1]+4.0, buffer[i+2], buffer[i+3]-4.0);
          //HC06Printf("@%f,%f,%f,%f\n", buffer[i], buffer[i+1], buffer[i+2], buffer[i+3]);
        //}
				int input_idx = 0;

				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 150; j++)
					{
						model_input->data.f[input_idx++] = buffer[i][j];
					}
				}
				input_idx = 0;
				prev_time=millis();
				TfLiteStatus invoke_status = interpreter->Invoke();
				
				printf("inference time: %d\n", millis()-prev_time);

				for (int i = 0 ; i < 30; i++)
					printf("%.1f, ", interpreter->output(0)->data.f[i]);
				printf("\n");
      }
      
      //HC06Printf("@%d,%d,%d,%d\n", sthumb, sindex, smiddle, sring);
			//printf("@%d,%d,%d,%d\n", sthumb, sindex, smiddle, sring);
      //printf("i:%f, m:%f, r:%f, l:%f\n", sindex+8.0, smiddle+4.0, sring, slittle-4.0);
			//printf("%d\n", millis()-prev_time);
			prev_time=millis();
			
		}
  }
}
