/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  
 *        \file  GPIO_LCnfg.c
 *        \brief  
 *
 *      \details  API functions to take GPIO configuration from user 
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
* \Syntax          : void GPIO_Init(void)        
* \Description     : initialize GPIO                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : None                     
* \Parameters (out): None                                                      
* \Return value:   : None                                  
*******************************************************************************/
void GPIO_Init(void)
	{
//enable portf clk ,unlock port , commet pins 0->5
		GPIO_PortCNFG(GPIO_PORTF_ID,GPIO_ENABLE_PORT,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5,GPIO_PCTL_DEFAULT);
//set in DEN reg pins0->5 as digital		
		GPIO_PortCNFG(GPIO_PORTF_ID,GPIO_EN_DIG,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5,GPIO_PCTL_DEFAULT);
//set pins 1,2,3 as o/p and rest of pins I/p
		GPIO_PortCNFG(GPIO_PORTF_ID,GPIO_DIR,GPIO_PIN1|GPIO_PIN2|GPIO_PIN3,GPIO_PCTL_DEFAULT);//set pin 1,2,3 o/p and other i/p
//set pull up reg for pin0,4
		GPIO_PortCNFG(GPIO_PORTF_ID,GPIO_EN_PUR,GPIO_PIN0|GPIO_PIN4,GPIO_PCTL_DEFAULT);
	}
	/******************************************************************************
* \Syntax          : void GPIO_PortCNFG(PortID,ConfigurationAction,conf Pins , Pcontrol register config)        
* \Description     : Configure GPIO Port passed to it with user configuration action                                   	
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : PortID: Port Id for port to be configured
*                    CnfgPins : Pin numbers
*                    CnfgAction : user selection form one of predefine gpio configuration action
*                    
*                    GPIO_ENABLE_PORT : enable port clck , unlock port, commit pins
*	                   GPIO_DIR:  Set pin direction IN/OUT , direction config is set from CnfgPins
*                    GPIO_EN_DIG: Digital enable
*                    GPIO_SET_ALT_FNC: set alternative function for pin	,Alternative fnc select is passed 
*	                    through "PCTL_Cnfg"
*	                   PCTL_Cnfg: contains Alternative fnc select if cofgAction is GPIO_SET_ALT_FNC else set to 0  
* \Parameters (out): None                                                      
* \Return value:   : None                                   
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
