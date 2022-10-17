#include<stdint.h>
#include"SysTick.c"
#include"GPIO_DRIVER.c"
#include"GPIO_LCnfg.c"

//void SystemInit (void);
//define LED to be led on portf pin1
#define LED  DIO_PIN1
//defines for led on /off time in seconds
#define ON_TIMER         2
#define OFF_TIMER        2
//current Led state
#define LD_OFF_TIMER     0
#define LD_ON_TIMER      1
//#define LAST_COUNT_TIME  0xfffffe  //last count 999.938ms instead of 1000ms to make up for procesing overhead 
uint8_t  Load_Timer_State;
void Toggle_Led(void);
/******************************************************************************
* \Syntax          : void Toggle_Led(void)        
* \Description     : Toggle user defined LED On /OFF
*                     turns led on for "ON_TIMER" seconds then turns led off
*                     for "OFF_TIMER" seconds										 
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                     
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/

void Toggle_Led()
{
	//static uint8_t  Load_Timer_State=LD_OFF_TIMER;
	
	DIO_Toggle_PIN(DIO_PORT_F,LED);
	if(Load_Timer_State==LD_ON_TIMER)
	{
		Load_Timer_State = LD_OFF_TIMER;
		SysTick_Delay_Sec(OFF_TIMER);
	}
	else
	{
		Load_Timer_State = LD_ON_TIMER;
		SysTick_Delay_Sec(ON_TIMER);
	}			
}
int main()
{
	GPIO_Init();
	SysTick_Init();
	Load_Timer_State=LD_OFF_TIMER;
	SysTick_AttachCallbackFn(Toggle_Led);
	SysTick_Delay_Sec(OFF_TIMER);
	while(1)
	{
 		
	}
}
