/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  
 *        \file  GPIO_LCnfg.c
 *        \brief  
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "GPIO_Config.h"
#include <TM4C123.h>
/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

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
void GPIO_Init(void)
	{
//enable portf clk ,unlock port , commint pins 0->5
		GPIO_PortCNFG(GPIO_PORTF_ID,GPIO_ENABLE_PORT,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5,GPIO_PCTL_DEFAULT);
//set in DEN reg pins0->5 as digital		
		GPIO_PortCNFG(GPIO_PORTF_ID,GPIO_EN_DIG,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5,GPIO_PCTL_DEFAULT);
//set pins 1,2,3 as o/p and rest of pins I/p
		GPIO_PortCNFG(GPIO_PORTF_ID,GPIO_DIR,GPIO_PIN1|GPIO_PIN2|GPIO_PIN3,GPIO_PCTL_DEFAULT);//set pin 1,2,3 o/p and other i/p
//set pull up reg for pin0,4
		GPIO_PortCNFG(GPIO_PORTF_ID,GPIO_EN_PUR,GPIO_PIN0|GPIO_PIN4,GPIO_PCTL_DEFAULT);
	}
	/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Description
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
	void GPIO_PortCNFG(uint8_t PortID, uint8_t CnfgAction, uint8_t CnfgPins,uint32_t PCTL_Cnfg)
	{
		GPIOA_Type* GPIO_Port;
		int Dummy;
		
		switch(PortID)
		{
			case GPIO_PORTA_ID:
				GPIO_Port=GPIOA;
			break;
			case GPIO_PORTB_ID:
				GPIO_Port=GPIOB;
			break;			
			case GPIO_PORTC_ID:
				GPIO_Port=GPIOC;
			break;
			case GPIO_PORTD_ID:
				GPIO_Port=GPIOD;
			break;
			case GPIO_PORTE_ID:
				GPIO_Port=GPIOE;
			break;
			case GPIO_PORTF_ID:
				GPIO_Port=GPIOF;
			break;
			default:
				GPIO_Port=GPIOF;//defualt to portf //to do invoke error msg
			break;
		}
		switch(CnfgAction)
		{
			case GPIO_ENABLE_PORT:
				SYSCTL_RCGCGPIO_R|=(0x1<<PortID);
				Dummy++;
				GPIO_Port->LOCK = GPIO_LOCK_KEY ;
				GPIO_Port->CR   = CnfgPins;
				break;
			case GPIO_EN_DIG:
				GPIO_Port->DEN |= CnfgPins;
				break;
			case GPIO_DIR:
				GPIO_Port->DIR = CnfgPins; //to do check with for loop current restrict user to set 0 for in pin and 1 for o/p  pin 
			  break;
			case GPIO_EN_PUR:
				GPIO_Port->PUR |= CnfgPins;
				break;
			case GPIO_SET_ALT_FNC:
				GPIO_Port->AFSEL = CnfgPins;
				GPIO_Port->PCTL  = PCTL_Cnfg;
				break;
		}
	}
	/*
	void GPIO_CNFG_ALT_FNC(uint8_t PortID,uint32_t PCTL_Cnfg)
	{
			GPIOA_Type* GPIO_Port;	
		
		switch(PortID)
		{
			case GPIO_PORTA_ID:
				GPIO_Port=GPIOA;
			break;
			case GPIO_PORTB_ID:
				GPIO_Port=GPIOB;
			break;			
			case GPIO_PORTC_ID:
				GPIO_Port=GPIOC;
			break;
			case GPIO_PORTD_ID:
				GPIO_Port=GPIOD;
			break;
			case GPIO_PORTE_ID:
				GPIO_Port=GPIOE;
			break;
			case GPIO_PORTF_ID:
				GPIO_Port=GPIOF;
			break;
			default:
				GPIO_Port=GPIOF;//defualt to portf //to do invoke error msg
			break;
		}
		GPIO_Port->PCTL =PCTL_Cnfg;
	}*/
/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
