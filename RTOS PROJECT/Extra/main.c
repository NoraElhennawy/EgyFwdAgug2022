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
#include <string.h>
#include <math.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )
//GPIO///
#define B1_CNFG_PORT  PORT_0  //port config for BUTTON1
#define B1_CNFG_PIN   PIN1		//PIN # for BUTTON1
#define B2_CNFG_PORT  PORT_0  //port config BUTTON2
#define B2_CNFG_PIN   PIN2		//PIN # BUTTON2

//RTOS ///

/* Define a variable of type struct AMMessage.  The examples below demonstrate
how to pass the whole variable through the queue, and as the structure is
moderately large, also how to pass a reference to the variable through a queue. */
struct AMessage
{
   char ucMessageID;
   char ucData[ 20 ];
} xMessage;

//Mesg ID for messages between Tasks
#define MSG_FRM_TRANS_TASK  0x01
#define MSG_FRM_B1_R_EDGE   0x02
#define MSG_FRM_B1_F_EDGE   0x03
#define MSG_FRM_B2_R_EDGE   0x04
#define MSG_FRM_B2_F_EDGE   0x05
#define MSG_Q_SIZE           50     //Max number of msg that intertask msg queue can hold 

/* Queue used to send and receive pointers to messages between tasks */
QueueHandle_t xPointerQueue = NULL;

TaskHandle_t Task1_Task_Handler = NULL;
TaskHandle_t UART_Task_Handler = NULL;
TaskHandle_t BUTTON1_Task_Handler =NULL;
TaskHandle_t BUTTON2_Task_Handler =NULL;
TaskHandle_t LD1_SIM_Task_Handler =NULL;
TaskHandle_t LD2_SIM_Task_Handler =NULL;
//TASK CONFIG
#define LD1_TASK_PERIOD    10
#define LD2_TASK_PERIOD    100
#define TRANS_TASK_PERIOD  100
#define B1_TASK_PERIOD     50
#define B2_TASK_PERIOD     50 
#define UART_TASK_PERIOD   20
//Task Statistics
unsigned int LD1_InTime=0,LD1_OutTime=0,LD1_TotalTime=0;
unsigned int LD2_InTime=0,LD2_OutTime=0,LD2_TotalTime=0;
unsigned int B1_InTime=0,B1_OutTime=0,B1_TotalTime=0;
unsigned int B2_InTime=0,B2_OutTime=0,B2_TotalTime=0;
unsigned int Trans_InTime=0,Trans_OutTime=0,Trans_TotalTime=0;
unsigned int UART_InTime=0,UART_OutTime=0,UART_TotalTime=0;
unsigned int TotalTime=0;
float CPU_Usage=0;
 //extra  declare task state outside task to avoid limited task stack
char TaskState[270]; //calculate size from debugging function and watch Buffer size created inside fnc
  
/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
void CalcTimeDemand(void);
/*-----------------------------------------------------------*/
void vCreateQueues( void )
{

   /* Create the queue used to send pointers to struct AMessage structures. */
   xPointerQueue = xQueueCreate(
                         /* The number of items the queue can hold. */
                         MSG_Q_SIZE,
                         /* Size of each item is big enough to hold only a
                         pointer. */
                         sizeof( &xMessage ) );

   if( xPointerQueue == NULL ) 
   {
      /* One or more queues were not created successfully as there was not enough
      heap memory available.  Handle the error here.  Queues can also be created
      statically. */
   }
}

void Task1_Task( void * pvParameters )
{
   int i;
   struct AMessage *pxPointerToxMessage;
   struct AMessage Task1_Msg;
   TickType_t LastWakeTime;
	 const TickType_t xFrequency=TRANS_TASK_PERIOD;
	//Init LastWake var
	 LastWakeTime =xTaskGetTickCount();
 	
		Task1_Msg.ucMessageID =MSG_FRM_TRANS_TASK;
	 //extra  print Task state  in transmitter task	
	 // strcpy(Task1_Msg.ucData,"Transmitter Task\n");
	 // pxPointerToxMessage=&Task1_Msg;
	 
	/* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Task code goes here. */
				//extra  print Task state  in transmitter task
        vTaskList(TaskState);	
				for(i=0;i<270;i=i+20)//extra cut buffer into msg and send on queue
				{
					strncpy(Task1_Msg.ucData,TaskState+i,20);
					pxPointerToxMessage=&Task1_Msg;
				 /* Send the address of xMessage to the queue created to hold 10    pointers. */
				  xQueueSend( /* The handle of the queue. */
										 xPointerQueue,
										 /* The address of the variable that holds the address of xMessage.
										 sizeof( &xMessage ) bytes are copied from here into the queue. As the
										 variable holds the address of xMessage it is the address of xMessage
										 that is copied into the queue. */
										 ( void * ) &pxPointerToxMessage,
										 ( TickType_t ) 0 );					
				vTaskDelayUntil(&LastWakeTime,xFrequency);	
				}
			//	GPIO_write(PORT_0,PIN2,PIN_IS_HIGH);
			/*Idle Task 0ff*/
			//	GPIO_write(PORT_0,PIN3,PIN_IS_LOW);			
			
    }
	}

void UART_Task( void * pvParameters )
{
  long i=0;  
	struct AMessage *pxRxedPointer;
	TickType_t LastWakeTime;
	const TickType_t xFrequency=UART_TASK_PERIOD;
	//Init LastWake var
	LastWakeTime =xTaskGetTickCount();

	/* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
      /* Task code goes here. */
				 if( xPointerQueue != NULL )
				 {
						/* Receive a message from the created queue to hold pointers.  Block for 10
						ticks if a message is not immediately available.  The value is read into a
						pointer variable, and as the value received is the address of the xMessage
						variable, after this call pxRxedPointer will point to xMessage. */
						if( xQueueReceive( xPointerQueue,
															 &( pxRxedPointer ),
															 ( TickType_t ) 0 ) == pdPASS )
						{
							 /* *pxRxedPointer now points to xMessage. */
							switch(pxRxedPointer->ucMessageID)
							{
								case MSG_FRM_TRANS_TASK://extra
									vSerialPutString((const signed char*)pxRxedPointer->ucData,20);
								break;
								case MSG_FRM_B1_R_EDGE: //Not clear in Rubric wht to do if B1 rising edge detect for know send on uart
								case MSG_FRM_B1_F_EDGE: //Not clear in Rubric wht to do if B1 falling edge detect for know send on uart									
								case MSG_FRM_B2_R_EDGE: //Not clear in Rubric wht to do if B2 rising edge detect for know send on uart
								case MSG_FRM_B2_F_EDGE: //Not clear in Rubric wht to do if B2 falling edge detect for know send on uart									

									vSerialPutString((const signed char*)pxRxedPointer->ucData,strlen(pxRxedPointer->ucData));
								break;
								
								default:
									break;
							}
							i++;
						}
				 }
	//			GPIO_write(PORT_0,PIN7,PIN_IS_LOW);
				//vTaskDelay( 20);	
				vTaskDelayUntil(&LastWakeTime,xFrequency);
		//		GPIO_write(PORT_0,PIN7,PIN_IS_HIGH);
			/*Idle Task 0ff*/
	//			GPIO_write(PORT_0,PIN4,PIN_IS_LOW);			
		
    }
	}
void BUTTON1_Task( void * pvParameters )
{
   //int i;
   struct AMessage *pxPointerToB1Message;
   struct AMessage B1_Msg;
   pinState_t u8Button1State;
	 TickType_t LastWakeTime;
	 const TickType_t xFrequency=B1_TASK_PERIOD;
	//Init LastWake var
	 LastWakeTime =xTaskGetTickCount();
	 
	//update button state at task start
	 u8Button1State = GPIO_read(B1_CNFG_PORT,B1_CNFG_PIN); 
	
	/* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Task code goes here. */
				//Monitor Button 1 Level
				if(u8Button1State!=(GPIO_read(B1_CNFG_PORT,B1_CNFG_PIN)))//if level changed
				{	
					//check Transition Type
					if(u8Button1State==PIN_IS_LOW) //rising edge
					{
						B1_Msg.ucMessageID = MSG_FRM_B1_R_EDGE;
						strcpy(B1_Msg.ucData,"B1 Rising Edge\n");						
					}
					else //falling edge
					{
						B1_Msg.ucMessageID = MSG_FRM_B1_F_EDGE;
						strcpy(B1_Msg.ucData,"B1 Falling Edge\n");
					}
					pxPointerToB1Message=&B1_Msg;
					u8Button1State = GPIO_read(B1_CNFG_PORT,B1_CNFG_PIN);//update button state	
				 /* Send the address of xMessage to the queue created to hold 10    pointers. */
				 xQueueSend( /* The handle of the queue. */
										 xPointerQueue,
										 /* The address of the variable that holds the address of xMessage.
										 sizeof( &xMessage ) bytes are copied from here into the queue. As the
										 variable holds the address of xMessage it is the address of xMessage
										 that is copied into the queue. */
										 ( void * ) &pxPointerToB1Message,
										 ( TickType_t ) 0 );
				}										 
				//vTaskDelay( 50);	
				vTaskDelayUntil(&LastWakeTime,xFrequency);
			//	GPIO_write(PORT_0,PIN2,PIN_IS_HIGH);
			/*Idle Task 0ff*/
			//	GPIO_write(PORT_0,PIN3,PIN_IS_LOW);			
			
    }
	}
void BUTTON2_Task( void * pvParameters )
{
   //int i;
   struct AMessage *pxPointerToB2Message;
   struct AMessage B2_Msg;
   pinState_t u8Button2State;
	 TickType_t LastWakeTime;
	 const TickType_t xFrequency=B2_TASK_PERIOD;
	//Init LastWake var
	 LastWakeTime =xTaskGetTickCount();
	
	//update button state at task start
		u8Button2State = GPIO_read(B2_CNFG_PORT,B2_CNFG_PIN); 
	
	/* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Task code goes here. */
				//Monitor Button 2 Level
				if(u8Button2State!=(GPIO_read(B2_CNFG_PORT,B2_CNFG_PIN)))//if level changed
				{	
					//check Transition Type
					if(u8Button2State==PIN_IS_LOW) //rising edge
					{
						B2_Msg.ucMessageID = MSG_FRM_B2_R_EDGE;
						strcpy(B2_Msg.ucData,"B2 Rising Edge\n");						
					}
					else //falling edge
					{
						B2_Msg.ucMessageID = MSG_FRM_B2_F_EDGE;
						strcpy(B2_Msg.ucData,"B2 Falling Edge\n");
					}
					pxPointerToB2Message=&B2_Msg;
					u8Button2State = GPIO_read(B2_CNFG_PORT,B2_CNFG_PIN);//update button state	
				 /* Send the address of xMessage to the queue created to hold 10    pointers. */
				 xQueueSend( /* The handle of the queue. */
										 xPointerQueue,
										 /* The address of the variable that holds the address of xMessage.
										 sizeof( &xMessage ) bytes are copied from here into the queue. As the
										 variable holds the address of xMessage it is the address of xMessage
										 that is copied into the queue. */
										 ( void * ) &pxPointerToB2Message,
										 ( TickType_t ) 0 );
				}										 
				//vTaskDelay( 50);	
					vTaskDelayUntil(&LastWakeTime,xFrequency);
			//	GPIO_write(PORT_0,PIN2,PIN_IS_HIGH);
			/*Idle Task 0ff*/
			//	GPIO_write(PORT_0,PIN3,PIN_IS_LOW);			
			
    }
	}
void LD1_SIM_Task( void * pvParameters )
{
    long i;
	  TickType_t LastWakeTime;
		const TickType_t xFrequency=LD1_TASK_PERIOD;
	//Init LastWake var
	LastWakeTime =xTaskGetTickCount();

//	vTaskSetApplicationTaskTag( NULL,(void*)LD1_TASK_ID );
	/* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Task code goes here. */
				for(i=0;i<33000;i++)//34000 =5 ms  67000=10ms
				{
					i=i;
				}
				
				vTaskDelayUntil(&LastWakeTime,xFrequency);				
    }
	}

void LD2_SIM_Task( void * pvParameters )
{
   long i;
	  TickType_t LastWakeTime;
		const TickType_t xFrequency=LD2_TASK_PERIOD;
	 
	//Init LastWake var
	LastWakeTime =xTaskGetTickCount();

	//vTaskSetApplicationTaskTag( NULL,(void*)LD2_TASK_ID );
	/* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.*/ 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
		
    for( ;; )
    {
     
				/* Task code goes here. */
				for(i=0;i<80000;i++)//34000 =5 ms  67000=10ms  //80000=12ms 
				{
					i=i;
				}
				
				vTaskDelayUntil(&LastWakeTime,xFrequency);			
    }
	}
/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */

int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
 // CalcTimeDemand();
  /* Create Queues first here */
	vCreateQueues();
  /* Create the task, storing the handle. */
	 xTaskCreatePeriodic(
                    LD1_SIM_Task,       /* Function that implements the task. */
                    "LOAD1 SIM. TASK",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    2,/* Priority at which the task is created. */
                    &LD1_SIM_Task_Handler ,LD1_TASK_PERIOD );      /* Used to pass out the created task's handle. */

	 xTaskCreatePeriodic(
                    LD2_SIM_Task,       /* Function that implements the task. */
                    "LOAD2 SIM. TASK",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    2,/* Priority at which the task is created. */
                    &LD2_SIM_Task_Handler,LD2_TASK_PERIOD );      /* Used to pass out the created task's handle. */

  	
  xTaskCreatePeriodic(
                    Task1_Task,       /* Function that implements the task. */
                    "Periodic_Trans",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    2,/* Priority at which the task is created. */
                    &Task1_Task_Handler,TRANS_TASK_PERIOD );      /* Used to pass out the created task's handle. */

	xTaskCreatePeriodic(
                    UART_Task,       /* Function that implements the task. */
                    "UART_Task",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    2,/* Priority at which the task is created. */
                    &UART_Task_Handler,UART_TASK_PERIOD );      /* Used to pass out the created task's handle. */
	xTaskCreatePeriodic(
                    BUTTON1_Task,       /* Function that implements the task. */
                    "Button1 Task",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    2,/* Priority at which the task is created. */
                    &BUTTON1_Task_Handler,B1_TASK_PERIOD );      /* Used to pass out the created task's handle. */

	xTaskCreatePeriodic(
                    BUTTON2_Task,       /* Function that implements the task. */
                    "Button2 Task",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    2,/* Priority at which the task is created. */
                    &BUTTON2_Task_Handler ,B2_TASK_PERIOD);      /* Used to pass out the created task's handle. */
								
	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */

//set task tag here to ensure intial task work is plotted
vTaskSetApplicationTaskTag( LD1_SIM_Task_Handler,(void*)LD1_TASK_ID );
vTaskSetApplicationTaskTag( LD2_SIM_Task_Handler,(void*)LD2_TASK_ID );
vTaskSetApplicationTaskTag(BUTTON1_Task_Handler,(void*)B1_TASK_ID);
vTaskSetApplicationTaskTag(BUTTON2_Task_Handler,(void*)B2_TASK_ID);										
vTaskSetApplicationTaskTag(Task1_Task_Handler,(void *)TRANS_TASK_ID);
vTaskSetApplicationTaskTag( UART_Task_Handler,(void*)UART_TASK_ID );
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
	T1PR = 1000;//(main_freq/1000)
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
void vApplicationTickHook(void)
{
	GPIO_write(PORT_0,PIN0,PIN_IS_LOW);
	GPIO_write(PORT_0,PIN0,PIN_IS_HIGH);
}

void vApplicationIdleHook(void)
{

}
void CalcTimeDemand(void)
{
	int i,j,k,l;
	float TaskPeriodList[6]={10,20,50,50,100,100};
	float TaskExeTimeList[6]={5,0.018,0.017,0.017,12,0.023};
	char  TaskName[6][10]={"LD1","UART","B1","B2","LD2","TRANS."};
	float w; 
	char RString[40];
	
	for(j=0;j<6;j++)//calculate Time Analysis for each Task
	{  
			sprintf(RString,"Time Analysis %s Task \n",TaskName[j]); //Print Task Name
			vSerialPutString((const signed char*)RString,strlen((const char*)RString));
      for(l=0;l<40000;l++) //Delay to prevent serial buffer overflow
			{
				l=l;
			}
		w=0;//init worst case exe to 0
		for(i=0;i<(TaskPeriodList[j]);i++) //calculate W for every Time instance for Task j from 1->Task Period
		{
			w=TaskExeTimeList[j];
			for(k=0;k<=(j-1);k++)
			{
				w+=(ceil((i+1)/TaskPeriodList[k])*TaskExeTimeList[k]);
			}
			sprintf(RString,"w[%d] = %5.5f \n",i,w);//Print Wi
			vSerialPutString((const signed char*)RString,strlen((const char*)RString));
			for(l=0;l<40000;l++)//Delay to prevent serial buffer overflow
			{
				l=l;
			}
		}
		if(w<TaskPeriodList[j]) //check Task Shedulablity if Wi < Pi then task schedulable
		{
			sprintf(RString,"w[%d] < Task Period, Task is Schedulable\n",i);
			vSerialPutString((const signed char*)RString,strlen((const char*)RString));
		}
		else  //Wi>Pi Task is not schedulable
		{
			sprintf(RString,"w[%d] > Task Period, Task is not Schedulable\n",i);
			vSerialPutString((const signed char*)RString,strlen((const char*)RString));
		}
		for(l=0;l<40000;l++)
		{
				l=l;
		}

	}
}

/*-----------------------------------------------------------*/


