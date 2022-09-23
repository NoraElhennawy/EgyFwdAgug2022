/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <lpc21xx.h>
#include "GPIO.h"
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		1
#define configUSE_EDF_SCHEDULER 1
#define configUSE_IDLE_HOOK			0
#define configUSE_TICK_HOOK			1
#define configCPU_CLOCK_HZ			( ( unsigned long ) 60000000 )	/* =12.0MHz xtal multiplied by 5 using the PLL. */
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )  //i.e Tick rate evrey 1ms
#define configMAX_PRIORITIES		( 4 )
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 90 )
#define configTOTAL_HEAP_SIZE		( ( size_t ) 13 * 1024 )
#define configMAX_TASK_NAME_LEN		( 20 )
#define configUSE_TRACE_FACILITY	1
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD		1

#define configQUEUE_REGISTRY_SIZE 	0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1
#define configUSE_APPLICATION_TASK_TAG  1
//TRACE TASK
//TASK TAG
#define LD1_TASK_ID    0x01
#define LD2_TASK_ID    0x02
#define B1_TASK_ID     0x03
#define B2_TASK_ID     0x04
#define TRANS_TASK_ID  0x05
#define UART_TASK_ID   0x06
//GPIO
//GPIO PINS used to Trace Tasks
#define IDLE_TRACE_PIN               PIN4
#define LD1_TRACE_PIN                PIN5
#define LD2_TRACE_PIN                PIN6
#define TRANSMIT_TRACE_PIN           PIN7
#define B1_TRACE_PIN                 PIN8
#define B2_TRACE_PIN                 PIN9
#define UART_TRACE_PIN               PIN3

//Task Statistics
extern unsigned int LD1_InTime,LD1_OutTime,LD1_TotalTime;
extern unsigned int LD2_InTime,LD2_OutTime,LD2_TotalTime;
extern unsigned int B1_InTime,B1_OutTime,B1_TotalTime;
extern unsigned int B2_InTime,B2_OutTime,B2_TotalTime;
extern unsigned int Trans_InTime,Trans_OutTime,Trans_TotalTime;
extern unsigned int UART_InTime,UART_OutTime,UART_TotalTime;
extern unsigned int TotalTime;
extern float CPU_Usage;


#define traceTASK_SWITCHED_IN() do\
{\
if((int)pxCurrentTCB->pxTaskTag ==0)\
{\
GPIO_write(PORT_0,IDLE_TRACE_PIN,PIN_IS_HIGH);\
}\
else if((int)pxCurrentTCB->pxTaskTag ==LD1_TASK_ID)\
{\
GPIO_write(PORT_0,LD1_TRACE_PIN,PIN_IS_HIGH);\
LD1_InTime=T1TC;\
}\
else if((int)pxCurrentTCB->pxTaskTag ==LD2_TASK_ID)\
{\
GPIO_write(PORT_0,LD2_TRACE_PIN,PIN_IS_HIGH);\
LD2_InTime=T1TC;\
}\
else if((int)pxCurrentTCB->pxTaskTag ==B1_TASK_ID)\
{\
GPIO_write(PORT_0,B1_TRACE_PIN,PIN_IS_HIGH);\
B1_InTime=T1TC;\
}\
else if((int)pxCurrentTCB->pxTaskTag ==B2_TASK_ID)\
{\
GPIO_write(PORT_0,B2_TRACE_PIN,PIN_IS_HIGH);\
B2_InTime=T1TC;\
}\
else if((int)pxCurrentTCB->pxTaskTag ==TRANS_TASK_ID)\
{\
GPIO_write(PORT_0,TRANSMIT_TRACE_PIN,PIN_IS_HIGH);\
Trans_InTime=T1TC;\
}\
else if((int)pxCurrentTCB->pxTaskTag ==UART_TASK_ID)\
{\
GPIO_write(PORT_0,UART_TRACE_PIN,PIN_IS_HIGH);\
UART_InTime=T1TC;\
}\
}while(0)


#define traceTASK_SWITCHED_OUT() do\
{\
if((int)pxCurrentTCB->pxTaskTag ==0)\
{\
GPIO_write(PORT_0,IDLE_TRACE_PIN,PIN_IS_LOW);\
}\
else if((int)pxCurrentTCB->pxTaskTag ==LD1_TASK_ID)\
{\
GPIO_write(PORT_0,LD1_TRACE_PIN,PIN_IS_LOW);\
LD1_OutTime=T1TC;\
LD1_TotalTime+=(LD1_OutTime-LD1_InTime);\
}\
else if((int)pxCurrentTCB->pxTaskTag ==LD2_TASK_ID)\
{\
GPIO_write(PORT_0,LD2_TRACE_PIN,PIN_IS_LOW);\
LD2_OutTime=T1TC;\
LD2_TotalTime+=(LD2_OutTime-LD2_InTime);\
}\
else if((int)pxCurrentTCB->pxTaskTag ==B1_TASK_ID)\
{\
GPIO_write(PORT_0,B1_TRACE_PIN,PIN_IS_LOW);\
B1_OutTime=T1TC;\
B1_TotalTime+=(B1_OutTime-B1_InTime);\
}\
else if((int)pxCurrentTCB->pxTaskTag ==B2_TASK_ID)\
{\
GPIO_write(PORT_0,B2_TRACE_PIN,PIN_IS_LOW);\
B2_OutTime=T1TC;\
B2_TotalTime+=(B2_OutTime-B2_InTime);\
}\
else if((int)pxCurrentTCB->pxTaskTag ==TRANS_TASK_ID)\
{\
GPIO_write(PORT_0,TRANSMIT_TRACE_PIN,PIN_IS_LOW);\
Trans_OutTime=T1TC;\
Trans_TotalTime+=(Trans_OutTime-Trans_InTime);\
}\
else if((int)pxCurrentTCB->pxTaskTag ==UART_TASK_ID)\
{\
GPIO_write(PORT_0,UART_TRACE_PIN,PIN_IS_LOW);\
UART_OutTime=T1TC;\
UART_TotalTime+=(UART_OutTime-UART_InTime);\
}\
TotalTime=T1TC;\
CPU_Usage=((LD1_TotalTime+LD2_TotalTime+B1_TotalTime+B2_TotalTime+Trans_TotalTime+UART_TotalTime)/(float)TotalTime)*100;\
}while(0)
#endif /* FREERTOS_CONFIG_H */
