#include<stdint.h>
#include"SysTick.c"
#include"GPIO_DRIVER.c"

//void SystemInit (void);
//define LED to be led on portf pin1
#define LED  DIO_PIN1
//defines for led on /off time in seconds
#define ON_TIMER         10
#define OFF_TIMER        2
//current Led state
#define LD_OFF_TIMER     0
#define LD_ON_TIMER      1
//#define LAST_COUNT_TIME  0xfffffe  //last count 999.938ms instead of 1000ms to make up for procesing overhead 

void Toggle_Led(void);
void GPIO_Init()
{
	int Dummy;
//	GPIOA.DATA|=0x01;
	SYSCTL_RCGCGPIO_R|=0x20;
	Dummy++;
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY ;
	GPIO_PORTF_CR_R |= 0x1f;
	GPIO_PORTF_DEN_R|=0x1f;
	GPIO_PORTF_AFSEL_R &= ~(0x1f);
	GPIO_PORTF_DIR_R |=0x0E;
	GPIO_PORTF_PUR_R |=0x11;
	
};
void Toggle_Led()
{
	static uint8_t  Load_Timer_State=LD_OFF_TIMER;
	
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
	SysTick_AttachCallbackFn(Toggle_Led);
	SysTick_Delay_Sec(OFF_TIMER);
	while(1)
	{
 		
	}
}
