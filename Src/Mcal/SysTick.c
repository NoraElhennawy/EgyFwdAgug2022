/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  <Write File Name>
 *       Module:  -
 *
 *  Description:  <Write File DESCRIPTION here>     
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
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
void SysTick_Init(void)
{
	NVIC_ST_RELOAD_R = SYSTICK_RELOAD_1_SEC;
	NVIC_ST_CURRENT_R =0;
	NVIC_ST_CTRL_R = NVIC_ST_CTRL_INTEN|NVIC_ST_CTRL_CLK_SRC|NVIC_ST_CTRL_ENABLE;//enable interruppt,clck,systick
	SysTickExtendCnt=0;
}
/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
void SysTick_Delay_Sec(uint8_t u8DelaySec)
{
	//NVIC_ST_RELOAD_R = SYSTICK_RELOAD_1_SEC;
	SysTickExtendCnt=u8DelaySec;
}
/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
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
	if(SysTickExtendCnt)
	{
		SysTickExtendCnt--;
	}
	else //seconds count has reached 0
	{
			if(SysTickIntHandler != (void*)0)
			{
					SysTickIntHandler();
			}
	}
}
/**********************************************************************************************************************
 *  END OF FILE: Std_Types.h
 *********************************************************************************************************************/
