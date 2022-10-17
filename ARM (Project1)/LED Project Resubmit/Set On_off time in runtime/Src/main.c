#include<stdint.h>
#include"SysTick.c"
#include"GPIO_DRIVER.c"
#include"GPIO_LCnfg.c"
#include"NVIC_Lconfg.c"
//void SystemInit (void);
//define LED to be led on portf pin1
#define LED  						     DIO_PIN1
#define START_SET_TIME_SW    DIO_PIN0
#define SET_ON_OFF_TIME_SW   DIO_PIN4 
//Define sw state
#define	SW1_OFF    0x01
#define	SW1_ON     0x00
#define	SW2_OFF    0x10
#define	SW2_ON     0x00
//defines defualt led on /off time in seconds
#define ON_TIMER         5
#define OFF_TIMER        2
//current Led state
#define LD_OFF_TIMER     0
#define LD_ON_TIMER      1
//#define LAST_COUNT_TIME  0xfffffe  //last count 999.938ms instead of 1000ms to make up for procesing overhead 
uint8_t  Load_Timer_State;
uint8_t  u8Led_OnTime,u8Led_OffTime;
//set on off timer state machine states
#define SET_ON_TIME       1
#define UPDATE_ON_TIME    2
#define SET_OFF_TIME      3
#define UPDATE_OFF_TIME   4
#define FINISHED_SET_TIME 5
uint8_t u8SwState;

void Toggle_Led(void);

void Toggle_Led()
{
	//static uint8_t  Load_Timer_State=LD_OFF_TIMER;
	
	DIO_Toggle_PIN(DIO_PORT_F,LED);
	if(Load_Timer_State==LD_ON_TIMER)
	{
		Load_Timer_State = LD_OFF_TIMER;
		SysTick_Delay_Sec(u8Led_OffTime);
	}
	else
	{
		Load_Timer_State = LD_ON_TIMER;
		SysTick_Delay_Sec(u8Led_OnTime);
	}			
}
DIO_Pin_Val u8ScanSw(DIO_ChannelType Pin)
{
	int u16Delay;
	DIO_Pin_Val SW_State;
	
	//Delay to prevent typematic read
	/*for(u16Delay=0;u16Delay<1000;u16Delay++)
	{
		u16Delay=u16Delay;
	}*/
	
	SW_State =DIO_Read_Pin(DIO_PORT_F,Pin);
	
	return(SW_State);
}
void vSetLedOnOff_Time(void)
{
 static uint8_t u8SetTimerState,LedOnTime,LedOffTime,SetTimer;
	u8SetTimerState=SET_ON_TIME;
	LedOnTime=0;
	LedOffTime=0;
	SetTimer=(u8ScanSw(START_SET_TIME_SW));
	
	while(SetTimer==SW1_ON)
	{
	switch(u8SetTimerState)
		{
			case SET_ON_TIME:
			if(u8ScanSw(START_SET_TIME_SW)==SW1_ON)
			{	
				if(u8ScanSw(SET_ON_OFF_TIME_SW )==SW2_ON)
					u8SetTimerState=UPDATE_ON_TIME;
			}
			else
			{
				if(LedOnTime) //if user set on timer and cleared set button then move to set off time
				{
					u8SetTimerState = SET_OFF_TIME;
				}
			}
			break;

			case UPDATE_ON_TIME:
				if(u8ScanSw(SET_ON_OFF_TIME_SW )==SW2_OFF)
				{
					LedOnTime++;
					if(u8ScanSw(START_SET_TIME_SW)==SW1_ON)
					{
						u8SetTimerState=SET_ON_TIME;
					}
					else //user finshed on timer setting move to off timer
					{
						u8SetTimerState=SET_OFF_TIME;
					}										
				}
				//check user not finshed setting timer
			break;		
				
			case SET_OFF_TIME:
			if(u8ScanSw(START_SET_TIME_SW)==SW1_ON)
			{	
			if(u8ScanSw(SET_ON_OFF_TIME_SW )==SW2_ON)
					u8SetTimerState=UPDATE_OFF_TIME;
			}
			else
			{
				if(LedOffTime) //if user set off timer and cleared set button then user finished
				{
					//SetTimer=0;
					u8SetTimerState=FINISHED_SET_TIME;
				}
			}
			break;		
			case UPDATE_OFF_TIME:
				if(u8ScanSw(SET_ON_OFF_TIME_SW )==SW2_OFF)
				{
					LedOffTime++;
					//check user not finshed setting timer
					if(u8ScanSw(START_SET_TIME_SW)==SW1_ON)
					{
						u8SetTimerState=SET_OFF_TIME;
					}
					else //user finshed off timer setting break
					{
						//SetTimer=0;
						u8SetTimerState=FINISHED_SET_TIME;
					}										
				}
			break;	
			case FINISHED_SET_TIME:
				SetTimer=SW1_OFF;
			//update global ON/OFF time
			u8Led_OffTime = LedOffTime;
			u8Led_OnTime  = LedOnTime;
			break;					
			
		}
	//	SetTimer=(u8ScanSw(START_SET_TIME_SW));
  }
	u8SetTimerState=SET_ON_TIME;//for debug only
}
int main()
{
	NVIC_Init();
	GPIO_Init();
	SysTick_Init();
	Load_Timer_State=LD_OFF_TIMER;
//Load On,Off time with default values
	u8Led_OffTime= OFF_TIMER;
	u8Led_OnTime = ON_TIMER;
	SysTick_AttachCallbackFn(Toggle_Led);
	SysTick_Delay_Sec(u8Led_OffTime);
			
	while(1)
	{
		//check user input for ON/OFF Led Timer
		if(u8ScanSw(START_SET_TIME_SW)==SW1_ON)
		{
			vSetLedOnOff_Time();
		}
	
 		
	}
}
