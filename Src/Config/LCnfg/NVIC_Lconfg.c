/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  
 *        \file  NVIC_LCnfg.c
 *        \brief  
 *
 *      \details  user configuration for NVIC interrupts enable, set periorty,set grp/subgrp 
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
#define MAX_INT_CNGF    4  // number of configured interrupts //should be inside NVIC_Config.h
#define GRP_SUBGRP_CNFG  NVIC_APINT_PRIGROUP_7_1 // to be set by user to req group subgroup config
/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
uint16_t NVIC_Cnfg[MAX_INT_CNGF][2]={{32,2},{33,3},{34,4},{35,5}};
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
void NVIC_SET_GRP_Pri(uint16_t Interrupt_GrpCnfg);
/******************************************************************************
* \Syntax          : void NVIC_Init(void)        
* \Description     : initialize NVIC                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : Noneion                     
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/
void NVIC_Init(void)
	{
		uint8_t u8Ind;
		NVIC_SET_GRP_Pri(GRP_SUBGRP_CNFG);
		for(u8Ind=0;u8Ind<MAX_INT_CNGF;u8Ind++)
		{
			NVIC_Enable_Int(NVIC_Cnfg[u8Ind][0]);
			NVIC_Set_Pri(NVIC_Cnfg[u8Ind][0],NVIC_Cnfg[u8Ind][1]);
			
		}

	}
/******************************************************************************
* \Syntax          : void NVIC_Enable_Int(Interrupt Number)        
* \Description     : enable interrupt in NVIC with interrupt number passed to it                                     
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
* \Syntax          : void NVIC_Set_Pri(Int_Num ,Interrupt_Pri)        
* \Description     : set priorty for interrupt                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : Int_Num        interrupt number
*            	       Interrupt_Pri  interrupt priorty
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
	void NVIC_Set_Pri(uint16_t Int_Num ,uint16_t Interrupt_Pri)
	{
	volatile uint32_t  Pri_Reg_Ptr ;
		uint32_t  H_Ind,V_Ind;
		
		H_Ind=  (Int_Num>>2);     // (Int_Num/4);
		V_Ind =(Int_Num%4);
		Pri_Reg_Ptr= *((volatile uint32_t *)(0xE000E400+V_Ind));
		switch(V_Ind)
		{
			case 0:
				Pri_Reg_Ptr&=~NVIC_PRI0_INT0_M;
				Pri_Reg_Ptr|= ((Interrupt_Pri&0x07)<<5);
			break;
			case 1:
				Pri_Reg_Ptr&=~NVIC_PRI0_INT1_M;
				Pri_Reg_Ptr|= ((Interrupt_Pri&0x07)<<13);
			break;
			case 2:
				Pri_Reg_Ptr&=~NVIC_PRI0_INT2_M;
				Pri_Reg_Ptr|= ((Interrupt_Pri&0x07)<<21);
			break;
			case 3:
				Pri_Reg_Ptr&=~NVIC_PRI0_INT3_M;
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
	void NVIC_SET_GRP_Pri(uint16_t Interrupt_GrpCnfg)
	{
		NVIC_APINT_R&=~(NVIC_APINT_VECTKEY_M|NVIC_APINT_PRIGROUP_M);//clr change bits
		NVIC_APINT_R|=(NVIC_APINT_VECTKEY|Interrupt_GrpCnfg);
	
		
	}
	
/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
