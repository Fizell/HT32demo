/*********************************************************************************************************//**
 * @file    USART/HyperTerminal_TxRx_Interrupt/ht32f5xxxx_01_it.c
 * @version $Rev:: 2970         $
 * @date    $Date:: 2018-08-03 #$
 * @brief   This file provides all interrupt service routine.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_board.h"
#include "usart_int.h"
#include "package.h"
#include "ht32_board_config.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup USART_Examples USART
  * @{
  */

/** @addtogroup HyperTerminal_TxRx_Interrupt
  * @{
  */



/* Private define ------------------------------------------------------------------------------------------*/
//串口接收缓存区 	
u8 ZigBee_RX_BUF[ZigBee_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
u8  ZigBee_TX_BUF[ZigBee_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
vu16 ZigBee_RX_STA=0;   	
extern int usart1_flag;


/* Private define ------------------------------------------------------------------------------------------*/


/* COM1 definitions                                                                                         */
u8 URTxBuf[USART_BUFSIZE] = {0x41, 0x42, 0x43, 0x44, 0x45};
u8 URRxBuf[USART_BUFSIZE];
vu32 URTxReadIndex = 0;
vu32 URTxWriteIndex = 0;
vu32 URRxWriteIndex = 0;

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
void NMI_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Hard Fault exception.
 * @retval  None
 ************************************************************************************************************/
void HardFault_Handler(void)
{
  #if 1

  static vu32 gIsContinue = 0;
  /*--------------------------------------------------------------------------------------------------------*/
  /* For development FW, MCU run into the while loop when the hardfault occurred.                           */
  /* 1. Stack Checking                                                                                      */
  /*    When a hard fault exception occurs, MCU push following register into the stack (main or process     */
  /*    stack). Confirm R13(SP) value in the Register Window and typing it to the Memory Windows, you can   */
  /*    check following register, especially the PC value (indicate the last instruction before hard fault).*/
  /*    SP + 0x00    0x04    0x08    0x0C    0x10    0x14    0x18    0x1C                                   */
  /*           R0      R1      R2      R3     R12      LR      PC    xPSR                                   */
  while (gIsContinue == 0)
  {
  }
  /* 2. Step Out to Find the Clue                                                                           */
  /*    Change the variable "gIsContinue" to any other value than zero in a Local or Watch Window, then     */
  /*    step out the HardFault_Handler to reach the first instruction after the instruction which caused    */
  /*    the hard fault.                                                                                     */
  /*--------------------------------------------------------------------------------------------------------*/

  #else

  /*--------------------------------------------------------------------------------------------------------*/
  /* For production FW, you shall consider to reboot the system when hardfault occurred.                    */
  /*--------------------------------------------------------------------------------------------------------*/
  NVIC_SystemReset();

  #endif
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PendSVC exception.
 * @retval  None
 ************************************************************************************************************/
void PendSV_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles UARTx interrupt.  (用于调式程序，打印出调式信息)
 * @retval  None
 ************************************************************************************************************/
void COM1_IRQHandler(void)
{
  /* Tx, move data from buffer to UART                                                                      */
  if (USART_GetFlagStatus(COM1_PORT, USART_FLAG_TXDE))
  {
    if (URTxWriteIndex > 0)
    {
      u8 u8WriteIndex;
      u8 u8ReadIndex;
      USART_SendData(COM1_PORT, URTxBuf[URTxReadIndex]);
      URTxReadIndex++;
      u8WriteIndex = URTxWriteIndex;
      u8ReadIndex = URTxReadIndex;
      if (u8WriteIndex == u8ReadIndex)
      {
        URTxWriteIndex = 0;
        URTxReadIndex = 0;
        USART_IntConfig(COM1_PORT, USART_INT_TXDE, DISABLE);
      }
    }
  }
  /* Rx, move data from UART to buffer                                                                      */
  if (USART_GetFlagStatus(COM1_PORT, USART_FLAG_RXDR))
  {
    //Why? USART_ClearFlag(COM1_PORT, USART_FLAG_RXDR);
    URRxBuf[URRxWriteIndex] = USART_ReceiveData(COM1_PORT);
		printf("%c", URRxBuf[URRxWriteIndex]);
		if(URRxBuf[URRxWriteIndex]=='1')
			usart1_flag=1;
		else if(URRxBuf[URRxWriteIndex]=='2')
			usart1_flag=2;
		else if(URRxBuf[URRxWriteIndex]=='4')
			usart1_flag=4;
		else if(URRxBuf[URRxWriteIndex]=='5')
			usart1_flag=5;
		else if(URRxBuf[URRxWriteIndex]=='6')
			usart1_flag=6;
    URRxWriteIndex = (URRxWriteIndex + 1) % USART_BUFSIZE;
  }

}
/*********************************************************************************************************//**
 * @brief   This function handles UARTx interrupt.  (用于ZigBee通信)
 * @retval  None
 ************************************************************************************************************/

void COM0_IRQHandler(void)
{
	u8 res;	
  // Rx, move data from UART to buffer                                                                      
  if (USART_GetFlagStatus(COM0_PORT, USART_FLAG_RXDR))
  {
		res = USART_ReceiveData(COM0_PORT);
		if(res==0x21)
     res=0x21;			
		if((ZigBee_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
		{ 
			if(ZigBee_RX_STA<ZigBee_MAX_RECV_LEN)	//还可以接收数据
			{
				 BFTM_SetCounter(HT_BFTM0, 0);//计数器清空 
				if(ZigBee_RX_STA==0) 				//使能定时器0的中断 
				{
					BFTM_EnaCmd(HT_BFTM0, ENABLE);//使能定时器0
				}
				ZigBee_RX_BUF[ZigBee_RX_STA++]=res;	//记录接收到的值	 
			}else 
			{
				ZigBee_RX_STA|=1<<15;				//强制标记接收完成
			} 
		}
		
	//	printf("%c", res);
    
  }

}

/*********************************************************************************************************//**
 * @brief  	通用定时器0中断  每0.1s触发一下。用来判断ZigBee模块是否接受完成
 * @retval  None
 ************************************************************************************************************/
void BFTM0_IRQHandler(void)
{
  
  //BFTM_ClearFlag(HT_BFTM0);
	//这一步需要仔细斟酌，可能存在Bug
	while (BFTM_GetFlagStatus(HT_BFTM0) != SET);
  {	 			   
		ZigBee_RX_STA|=1<<15;	//标记接收完成
		BFTM_ClearFlag(HT_BFTM0);  //清除BTIM0更新中断标志    
		BFTM_EnaCmd(HT_BFTM0, DISABLE);//关闭定时器0
	}	    
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
