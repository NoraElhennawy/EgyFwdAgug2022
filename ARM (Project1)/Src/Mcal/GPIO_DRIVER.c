/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  
 *        \file  GPIO.c
 *        \brief  
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "GPIO_DRIVER.h"

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
* \Syntax          : void DIO_Write_PIN(DIO_Port_Id u32Port ,DIO_ChannelType u16PinNum,DIO_Pin_Val u8PinValue)        
* \Description     : set Dio pin with pin num  "u16PinNum" on port  "u32Port"  with   u8PinValue                            
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : u32Port    Dio Port Addr
*                    u16PinNum  Dio Pin number
*                    u8PinValue Dio Value  
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
void DIO_Write_PIN (DIO_Port_Id u32Port ,DIO_ChannelType u16PinNum,DIO_Pin_Val u8PinValue)
{
	//calculate actual pin addr in port data reg.
	volatile uint32_t * Data_Port_Ptr;
	
	Data_Port_Ptr = (uint32_t *)(u32Port+u16PinNum);//set ptr to dit addr in port
	
	if(u8PinValue)//set bit
	{
		* Data_Port_Ptr = (u16PinNum>>2);
	}
	else  //clr bit
	{
		* Data_Port_Ptr =0;
	}
}
/******************************************************************************
* \Syntax          : void DIO_Write_Port(DIO_Port_Id u32Port,DIO_Port_Data u8PortData )        
* \Description     : write u8PortData on port  "u32Port"                                      
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : u32Port    port addr
*                    u8PortData port data
* \Parameters (out): None                                                      
* \Return value:   : void                              
*******************************************************************************/
void DIO_Write_Port(DIO_Port_Id u32Port,DIO_Port_Data u8PortData )
{
  	volatile uint32_t * Data_Port_Ptr;
	  
	  Data_Port_Ptr = (uint32_t *)(u32Port+DATA_PORT_OFFSET);
	* Data_Port_Ptr = u8PortData;
}
/******************************************************************************
* \Syntax          : DIO_Port_Data DIO_Read_Port(DIO_Port_Id u32Port)        
* \Description     : Read port pins for  "u32Port"                                  
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : u32Port    port addr                     
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/
DIO_Port_Data DIO_Read_Port(DIO_Port_Id u32Port)
{
	DIO_Port_Data u8PortData;
	volatile uint32_t * Data_Port_Ptr;
	
	Data_Port_Ptr = (uint32_t *)(u32Port+DATA_PORT_OFFSET);
	u8PortData = *(Data_Port_Ptr);
	return (u8PortData);
}
/******************************************************************************
* \Syntax          : DIO_Pin_Val DIO_Read_Pin(DIO_Port_Id u32Port,DIO_ChannelType u16PinNum )        
* \Description     : Read pin value for"u16PinNum" pin from "u32Port" port                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : u32Port   Port Addr 
*                    u16PinNum Pin Number
* \Parameters (out): None                                                      
* \Return value:   : None                                 
*******************************************************************************/
DIO_Pin_Val DIO_Read_Pin(DIO_Port_Id u32Port,DIO_ChannelType u16PinNum )
{
	DIO_Pin_Val u8PinVal;
	//calculate actual pin addr in port data reg.
	volatile uint32_t * Data_Port_Ptr;
	
	Data_Port_Ptr = (uint32_t *)(u32Port+u16PinNum);//set ptr to bit addr in port
	u8PinVal=*Data_Port_Ptr;
	//u8PinVal=(u8PinVal>>2);
	
	return u8PinVal;
}

/******************************************************************************
* \Syntax          : void DIO_Toggle_PIN (DIO_Port_Id u32Port ,DIO_ChannelType u16PinNum)        
* \Description     : Toggle "u16PinNum" Pin on "u32Port" Port                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Reentrant                                             
* \Parameters (in) : u32Port   Port Addr   
*                    u16PinNum Pin number 
* \Parameters (out): None                                                      
* \Return value:   : Void                                  
*******************************************************************************/
void DIO_Toggle_PIN (DIO_Port_Id u32Port ,DIO_ChannelType u16PinNum)
{
	//calculate actual pin addr in port data reg.
	volatile uint32_t * Data_Port_Ptr;
	DIO_Port_Data u8PortData;
	
	Data_Port_Ptr = (uint32_t *)(u32Port+u16PinNum);//set ptr to dit addr in port
	u8PortData    = *Data_Port_Ptr;
	if(u8PortData)
		(* Data_Port_Ptr ) = 0;
	else
		* Data_Port_Ptr = (u16PinNum>>2);
	
}

/**********************************************************************************************************************
 *  END OF FILE: FileName.c
 *********************************************************************************************************************/
