/*
 * main_user.c
 *
 *  Created on: Aug 8, 2022
 *      Author: Andre Hendricks
 */

#include <stdio.h>

//STM32 generated header files
#include "main.h"

//User generated header files
#include "User/main_user.h"
#include "User/util.h"

//Required FreeRTOS header files
#include "FreeRTOS.h"
#include "task.h"

char main_string[50];
char idle_string[50];
uint32_t main_counter = 0;
static int task1_id = 0;
static int task2_id = 1;
uint32_t idle_cycle_count = 0;


void vApplicationIdleHook( void )
{
	idle_cycle_count++;
}

static void TaskPrintIdleCycleCount(void *params){
	do{
	// the format specifier forces 8 charaters for the unsigned integer.
		sprintf(idle_string,"Idle Cycle Count: 0x%08lx\r\n",idle_cycle_count);
		print_str(idle_string);
		idle_cycle_count = 0;
		vTaskDelay(1000/ portTICK_RATE_MS);
	}while(1);
}

static void main_task(void *param){

	while(1){
		print_str("Main task loop executing\r\n");
		sprintf(main_string,"Main task iteration: 0x%08lx\r\n",main_counter++);
		print_str(main_string);
		vTaskDelay(1000/portTICK_RATE_MS);
	}
}
static void PrintStringTask(void *params){
	int *type = (int *)params;
	switch (*type){
		case 0:
			do{
				print_str("This task is created with ID 0\r\n");
				vTaskDelay(1000/portTICK_RATE_MS);
			}while(1);
		case 1:
			do{
				print_str("This task is created with ID 1\r\n");
				vTaskDelay(1000/portTICK_RATE_MS);
			}while(1);
	}
}

void main_user(){
	util_init();

	xTaskCreate(main_task,"Main Task", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(PrintStringTask, "Task 1", configMINIMAL_STACK_SIZE + 100, &task1_id, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(PrintStringTask, "Task 2", configMINIMAL_STACK_SIZE + 100, &task2_id, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(TaskPrintIdleCycleCount, "Idle Cycle Count", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 1, NULL);

	vTaskStartScheduler();

	while(1);

}
