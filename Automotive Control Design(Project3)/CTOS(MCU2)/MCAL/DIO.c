/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"



#define BUTTON_1_MONITOR_TASK_PERIODICITY 			50      /* Modified1 */
#define BUTTON_2_MONITOR_TASK_PERIODICITY 			50      /* Modified1 */
#define PERIODIC_TRANSIMITTER_TASK_PERIODICITY	100     /* Modified1 */
#define	UART_RECEIVER_TASK_PERIODICITY					20      /* Modified1 */
#define	LOAD_1_SIMULATOR_PERIODICITY						10			/* Modified1 */
#define	LOAD_2_SIMULATOR_PERIODICITY						100     /* Modified1 */


#define LOAD1_FOR_VALUE				33100
#define LOAD2_FOR_VALUE 			79500
/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

			
TaskHandle_t task_1_Handler = NULL;		/* Modified1 */
TaskHandle_t task_2_Handler = NULL;		/* Modified1 */
TaskHandle_t task_3_Handler = NULL;		/* Modified1 */
TaskHandle_t task_4_Handler = NULL;		/* Modified1 */
TaskHandle_t task_5_Handler = NULL;		/* Modified1 */
TaskHandle_t task_6_Handler = NULL;		/* Modified1 */


/* Modified1 */
QueueHandle_t	xQueue;

int Button_1_Rising = 0, Button_1_Falling = 0 ;
int Button_2_Rising = 0, Button_2_Falling = 0 ;
int Periodic_Transmit = 0 ;
extern unsigned char txDataSizeLeftToSend;

/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */

char runTimeStatsBuff[190];

int task_1_in_time =0 , task_1_out_time =0 , task_1_total_time =0 ;
int task_2_in_time =0 , task_2_out_time =0 , task_2_total_time =0 ;
int task_3_in_time =0 , task_3_out_time =0 , task_3_total_time =0 ;
int task_4_in_time =0 , task_4_out_time =0 , task_4_total_time =0 ;
int task_5_in_time =0 , task_5_out_time =0 , task_5_total_time =0 ;
int task_6_in_time =0 , task_6_out_time =0 , task_6_total_time =0 ;
int system_time  ; 
int cpu_load  ;

static void prvSetupHardware( void );
/*-----------------------------------------------------------*/

/* Modified1 */
/* Tasks Decleration */
void Button_1_Monitor (void *pvPararmeters);
void Button_2_Monitor (void *pvPararmeters);
void Periodic_Transmitter (void *pvPararmeters);
void Uart_Receiver (void *pvPararmeters);
void Load_1_Simulation (void *pvPararmeters);
void Load_2_Simulation (void *pvPararmeters);


/* Implement Tick Hook */
//void vApplicationTickHook (void)
//{
//		/* Write your code here */
//		GPIO_write( PORT_0 , PIN2 , PIN_IS_HIGH );
//		GPIO_write( PORT_0 , PIN2 , PIN_IS_LOW );	
//}

/*Implement Idle Hook */
//void vApplicationIdleHook (void)
//{
//	GPIO_write( PORT_0 , PIN3 , PIN_IS_HIGH );
//}

/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
	
	xQueue = xQueueCreate( 5 , sizeof(signed char[20]));
	
	
    /* Create Tasks here */
	 /* Create the task, storing the handle. */
    xTaskPeriodicCreate(
                    Button_1_Monitor,       /* Function that implements the task. */
                    "Button 1",          /* Text name for the task. */
                    configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &task_1_Handler,
										BUTTON_1_MONITOR_TASK_PERIODICITY);      /* Used to pass out the created task's handle. */
										
		/* Create the task, storing the handle. */
		xTaskPeriodicCreate(
                    Button_2_Monitor,       /* Function that implements the task. */
                    "Button 2",          /* Text name for the task. */
                    configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &task_2_Handler,
										BUTTON_2_MONITOR_TASK_PERIODICITY);      /* Used to pass out the created task's handle. */
		
		/* Create the task, storing the handle. */
		xTaskPeriodicCreate(
                    Periodic_Transmitter,       /* Function that implements the task. */
                    "Periodic Transmitter",          /* Text name for the task. */
                    configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &task_3_Handler,
										PERIODIC_TRANSIMITTER_TASK_PERIODICITY);      /* Used to pass out the created task's handle. */
										
		/* Create the task, storing the handle. */
		xTaskPeriodicCreate(
                    Uart_Receiver,       /* Function that implements the task. */
                    "Uart Reciever",          /* Text name for the task. */
                    configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &task_4_Handler,
										PERIODIC_TRANSIMITTER_TASK_PERIODICITY);      /* Used to pass out the created task's handle. */	
										
		/* Create the task, storing the handle. */
		xTaskPeriodicCreate(
                    Load_1_Simulation,       /* Function that implements the task. */
                    "Load 1 Simulation",          /* Text name for the task. */
                    configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &task_5_Handler,
										PERIODIC_TRANSIMITTER_TASK_PERIODICITY);      /* Used to pass out the created task's handle. */	
				
		/* Create the task, storing the handle. */
		xTaskPeriodicCreate(
                    Load_2_Simulation,       /* Function that implements the task. */
                    "Load 2 Simulation",          /* Text name for the task. */
                    configMINIMAL_STACK_SIZE,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,/* Priority at which the task is created. */
                    &task_6_Handler,
										PERIODIC_TRANSIMITTER_TASK_PERIODICITY);      /* Used to pass out the created task's handle. */	
												

	vTaskSetApplicationTaskTag( task_1_Handler, ( void * ) 1 );
	vTaskSetApplicationTaskTag( task_2_Handler, ( void * ) 2 );
	vTaskSetApplicationTaskTag( task_3_Handler, ( void * ) 3 );
	vTaskSetApplicationTaskTag( task_4_Handler, ( void * ) 4 );
	vTaskSetApplicationTaskTag( task_5_Handler, ( void * ) 5 );
	vTaskSetApplicationTaskTag( task_6_Handler, ( void * ) 6 );									


	

										


	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/

/* ***************************** TASK1 ****************************************** */
												/* ******************* */
/* Task to be created. */
void Button_1_Monitor (void *pvPararmeters)			/* Modified1 */
{
	pinState_t prev_buttonState = PIN_IS_LOW ;
	int	xLastWakeTime = xTaskGetTickCount();
	pinState_t curr_button_1_State = PIN_IS_LOW ;      /* Modified1 */
	
	
	for(;;)
	{
		/* Write your code here */
		curr_button_1_State = GPIO_read( PORT_1 , PIN0);

		if(	(prev_buttonState == PIN_IS_LOW) && (curr_button_1_State == PIN_IS_HIGH) )
			{
				Button_1_Rising = 1 ; 		
			}
		else if ( (prev_buttonState == PIN_IS_HIGH) && (curr_button_1_State == PIN_IS_LOW) )
			{
				Button_1_Falling = 1 ;
			}
		prev_buttonState = curr_button_1_State ;
		vTaskGetRunTimeStats( runTimeStatsBuff );
		xSerialPutChar('\n');
		vSerialPutString( runTimeStatsBuff , 190 );
		vTaskDelayUntil( &xLastWakeTime ,50);
		
	}
}
/*-----------------------------------------------------------*/

/* ***************************** TASK2 ****************************************** */
												/* ******************* */
/* Task to be created. */
void Button_2_Monitor (void *pvPararmeters)			/* Modified1 */
{
	pinState_t prev_buttonState = PIN_IS_LOW ;
	int	xLastWakeTime = xTaskGetTickCount();
	pinState_t curr_button_2_State = PIN_IS_LOW ;      /* Modified1 */
	

	
	for(;;)
	{
		/* Write your code here */
		curr_button_2_State = GPIO_read( PORT_1 , PIN1);

		if(	(prev_buttonState == PIN_IS_LOW) && (curr_button_2_State == PIN_IS_HIGH) )
			{
				Button_2_Rising = 1 ; 		
			}
		else if ( (prev_buttonState == PIN_IS_HIGH) && (curr_button_2_State == PIN_IS_LOW) )
			{
				Button_2_Falling = 1 ;
			}
		else
			{
				Button_2_Rising  = 0 ;
				Button_2_Falling = 0 ;
			}
		prev_buttonState = curr_button_2_State ;
		vTaskGetRunTimeStats( runTimeStatsBuff );
		xSerialPutChar('\n');
		vSerialPutString( runTimeStatsBuff , 190 );
		vTaskDelayUntil( &xLastWakeTime ,50);
		
	}
}

/*-----------------------------------------------------------*/

/* ***************************** TASK3 ****************************************** */
												/* ******************* */
/* Task to be created. */
void Periodic_Transmitter (void *pvPararmeters)			/* Modified1 */
{
	int	xLastWakeTime = xTaskGetTickCount();
	

	
	for(;;)
	{
		/* Write your code here */
		Periodic_Transmit = 1 ;		
	  vTaskGetRunTimeStats( runTimeStatsBuff );
		xSerialPutChar('\n');
		vSerialPutString( runTimeStatsBuff , 190 );
		vTaskDelayUntil( &xLastWakeTime ,100);
		
	}
}

/*-----------------------------------------------------------*/

/* ***************************** TASK4 ****************************************** */
												/* ******************* */
/* Task to be created. */
void Uart_Receiver (void *pvPararmeters)			/* Modified1 */
{
	int	xLastWakeTime = xTaskGetTickCount();
	

	for(;;)
	{
		/* Write your code here */
		if ( Button_1_Rising == 1 )
		{
			vSerialPutString("Rising Edge From Button 1",26);
			while (txDataSizeLeftToSend !=0);
			//xSerialPutChar('\n');
			Button_1_Rising = 0 ; 
		}
		else if ( Button_1_Falling == 1 )
		{
			vSerialPutString("Falling Edge From Button 1",27);
			//while (txDataSizeLeftToSend !=0);
			//xSerialPutChar('\n');
			Button_1_Falling = 0 ; 
		}
		else if ( Button_2_Rising == 1 )
		{
			vSerialPutString("Rising Edge From Button 2",26);
			//while (txDataSizeLeftToSend !=0);
			//xSerialPutChar('\n');
			Button_2_Rising = 0 ; 
		}
		else if ( Button_2_Falling == 1 )
		{
			vSerialPutString("Falling Edge From Button 2",27);
			//while (txDataSizeLeftToSend !=0);
			//xSerialPutChar('\n');
			Button_2_Falling = 0 ; 
		}
		else if( Periodic_Transmit == 1 )
		{
			vSerialPutString("Periodic Transmit",20);
			//while (txDataSizeLeftToSend !=0);
			//xSerialPutChar('\n');
			Periodic_Transmit = 0;
		}
		vTaskGetRunTimeStats( runTimeStatsBuff );
		xSerialPutChar('\n');
		vSerialPutString( runTimeStatsBuff , 190 );
		vTaskDelayUntil( &xLastWakeTime ,20);
		
	}
}
/*-----------------------------------------------------------*/

/* ***************************** TASK5 ****************************************** */
												/* ******************* */
void Load_1_Simulation ( void *pvPararmeters )
{
	int	xLastWakeTime = xTaskGetTickCount();
	
	
	for(;;)
	{
		int  i =0 ;
		for( i=0 ; i<LOAD1_FOR_VALUE  ;i++ )
		{
			i = i ;
		}
			vTaskGetRunTimeStats( runTimeStatsBuff );
		 xSerialPutChar('\n');
		 vSerialPutString( runTimeStatsBuff , 190 );
		 vTaskDelayUntil( &xLastWakeTime ,10);
	}
}


/*-----------------------------------------------------------*/

/* ***************************** TASK6 ****************************************** */
												/* ******************* */
void Load_2_Simulation ( void *pvPararmeters )
{
	int	xLastWakeTime = xTaskGetTickCount();
	

	
	for(;;)
	{
		int i =0 ;
		for( i=0 ; i<LOAD2_FOR_VALUE  ;i++ )
		{
			i = i ;	
		}
		vTaskGetRunTimeStats( runTimeStatsBuff );
		xSerialPutChar('\n');
		vSerialPutString( runTimeStatsBuff , 190 );
		vTaskDelayUntil( &xLastWakeTime ,100);
		
	}
}