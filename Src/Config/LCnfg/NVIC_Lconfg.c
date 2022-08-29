/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  
 *        \file  NVIC_LCnfg.c
 *        \brief  
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "stdint.h"
#include "NVIC_DRIVER.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/
#define MAX_INT_CNGF    2  // number of configured interrupts //should be inside NVIC_Config.h
#define GRP_SUBGRP_CNFG  NVIC_APINT_PRIGROUP_7_1 // to be set by user to req group subgroup config
/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
uint16_t NVIC_Cnfg[MAX_INT_CNGF][2]={{20,5},{30,3}};
/*********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
void NVIC_Enable_Int(uint16_t Interrupt_Num);
void NVIC_Set_Pri(uint16_t Interrupt_Num,uint16_t Interrupt_Pri);
//void NVIC_SET_GRP_Pri(uint16_t Interrupt_GrpCnfg);
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
void NVIC_Init(void)
	{
		uint8_t u8Ind;
		for(u8Ind=0;u8Ind<MAX_INT_CNGF;u8Ind++)
		{
			NVIC_Enable_Int(NVIC_Cnfg[u8Ind][0]);
			NVIC_Set_Pri(NVIC_Cnfg[u8Ind][0],NVIC_Cnfg[u8Ind][1]);
			//NVIC_SET_GRP_Pri(NVIC_Cnfg[u8Ind][2]);
		}

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
	void NVIC_Enable_Int(uint16_t Interrupt_Num)
	{
		uint32_t OffsetInd;
		OffsetInd=(Interrupt_Num%32);
		OffsetInd=(0x01<<(OffsetInd));
		//can be done with array of pointers to ENx registers 
		//NVIC_ENx[Ind]|=OffsetInd;
		  if(Interrupt_Num<32)
				NVIC_EN0_R|=OffsetInd;
			else if((32<=Interrupt_Num)&&(Interrupt_Num<64))
				NVIC_EN1_R|=(0x01<<(OffsetInd));
			else if((64<=Interrupt_Num)&&(Interrupt_Num<96))
				NVIC_EN2_R|=(0x01<<(OffsetInd));
			else if((96<=Interrupt_Num)&&(Interrupt_Num<128))	
				NVIC_EN3_R|=(0x01<<(OffsetInd));
			else if((128<=Interrupt_Num)&&(Interrupt_Num<138))
				NVIC_EN4_R|=(0x01<<(OffsetInd));
												
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
	void NVIC_Set_Pri(uint16_t Int_Num ,uint16_t Interrupt_Pri)
	{
	volatile uint32_t  Pri_Reg_Ptr ;
		uint32_t  H_Ind,V_Ind;
		
		H_Ind=        (Int_Num/4);
		V_Ind =(Int_Num%4);
		Pri_Reg_Ptr= *((volatile uint32_t *)(0xE000E400+V_Ind));
		switch(V_Ind)
		{
			case 0:
				Pri_Reg_Ptr|= ((Interrupt_Pri&0x07)<<5);
			break;
			case 1:
				Pri_Reg_Ptr|= ((Interrupt_Pri&0x07)<<13);
			break;
			case 2:
				Pri_Reg_Ptr|= ((Interrupt_Pri&0x07)<<21);
			break;
			case 3:
				Pri_Reg_Ptr|= ((Interrupt_Pri&0x07)<<29);
			break;			
		}
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
	/*void NVIC_SET_GRP_Pri(uint16_t Interrupt_GrpCnfg)
	{
	
	}*/
	
/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
