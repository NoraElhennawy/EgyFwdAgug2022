/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  <SysTick.c>
 *       Module:  -
 *
 *  Description:  API functions for systick timer : initilsize, delay, attach call back function    
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include <stdint.h>
#include <SysTick.h>
/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

SysTickCallBckFnPtr SysTickIntHandler;
// SysTickSecondsCnt;
/*********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/******************************************************************************
* \Syntax          : void SysTick_Init(void)        
* \Description     : Initialize systick driver                                   
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                    
* \Parameters (out): None                                                      
* \Return value:   : None                                  
*******************************************************************************/
void SysTick_Init(void)
{
	NVIC_ST_RELOAD_R = SYSTICK_RELOAD_1_SEC;
	NVIC_ST_CURRENT_R =0;
	NVIC_ST_CTRL_R = NVIC_ST_CTRL_INTEN|NVIC_ST_CTRL_CLK_SRC|NVIC_ST_CTRL_ENABLE;//enable interruppt,clck,systick
	SysTickExtendCnt=0;
}
/******************************************************************************
* \Syntax          : void SysTick_Delay_Sec(DelaySec)        
* \Description     : API function to issue systick interrupt every DelaySec                                   
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : DelaySec : delay in seconds                      
* \Parameters (out): None                                                      
* \Return value:   : None                                  
*******************************************************************************/
void SysTick_Delay_Sec(uint8_t u8DelaySec)
{
	//NVIC_ST_RELOAD_R = SYSTICK_RELOAD_1_SEC;
	SysTickExtendCnt=u8DelaySec;
}
/******************************************************************************
* \Syntax          : void SysTick_AttachCallbackFn(SysTickCallBckFnPtr FnPtr)        
* \Description     : Attach user call back function to be excuted in systick handler                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : FnPtr   pointer to  user call back function                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/

void SysTick_AttachCallbackFn(SysTickCallBckFnPtr FnPtr) 
{
	SysTickIntHandler = FnPtr;
}

void SysTick_Handler1(void)
{

	if(SysTickExtendCnt==0) //seconds count has reached 0
	{
			if(SysTickIntHandler != (void*)0)
			{
					SysTickIntHandler();
			}
	}
	else
	{
		SysTickExtendCnt--;
	}
}
/**********************************************************************************************************************
 *  END OF FILE: Std_Types.h
 *********************************************************************************************************************/
