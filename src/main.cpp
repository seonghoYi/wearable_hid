#include "main.h"



static void threadMain(void *arg);

int main()
{
	hwInit();

	apInit();

	
	if (xTaskCreate(threadMain, "threadMain",_HW_DEF_RTOS_THREAD_MEM_MAIN, NULL, _HW_DEF_RTOS_THREAD_PRI_MAIN, NULL) != pdPASS)
	{
		while(1);
	}


	vTaskStartScheduler();

	return 0;
}



static void threadMain(void *arg)
{
	apMain();
}