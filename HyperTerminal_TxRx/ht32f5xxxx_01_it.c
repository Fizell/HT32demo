/*********************************************************************************************************//**
 * @file    USART/HyperTerminal_TxRx/ht32f5xxxx_01_it.c
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

#define USART_BUFSIZE   128

u8 URTxBuf[USART_BUFSIZE] = {0x41, 0x42, 0x43, 0x44, 0x45};
u8 URRxBuf[USART_BUFSIZE];
vu32 URTxReadIndex = 0;
vu32 URTxWriteIndex = 0;
vu32 URRxWriteIndex = 0;


/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup USART_Examples USART
  * @{
  */

/** @addtogroup HyperTerminal_TxRx
  * @{
  */


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
 * @brief   This function handles USART1 interrupt.  (用于调式程序，打印出调式信息)
 * @retval  None
 ************************************************************************************************************/
void COM1_IRQHandler(void)
{
  /* Rx, move data from USART to buffer                                                                      */
  if (USART_GetFlagStatus(COM1_PORT, USART_FLAG_RXDR))
  {
		
    //Why? USART_ClearFlag(COM1_PORT, USART_FLAG_RXDR);
    URRxBuf[URRxWriteIndex] = USART_ReceiveData(COM1_PORT);
//		printf("%c", URRxBuf[URRxWriteIndex]);
		if(URRxBuf[URRxWriteIndex] == '1')
			printf("Yes!\n");
		USART_SendData(COM1_PORT, URRxBuf[URRxWriteIndex]);
    URRxWriteIndex = (URRxWriteIndex + 1) % USART_BUFSIZE;
		
		USART_IntConfig(COM1_PORT, USART_INT_TXDE, DISABLE);
  }
	
	

}

//void COM2_IRQHandler(void)
//{
//  /* Tx, move data from buffer to USART                                                                      */
//  if (USART_GetFlagStatus(COM2_PORT, USART_FLAG_TXDE))
//  {
//    if (URTxWriteIndex > 0)
//    {
//      u8 u8WriteIndex;
//      u8 u8ReadIndex;
//      USART_SendData(COM2_PORT, URTxBuf[URTxReadIndex]);
//      URTxReadIndex++;
//      u8WriteIndex = URTxWriteIndex;
//      u8ReadIndex = URTxReadIndex;
//      if (u8WriteIndex == u8ReadIndex)
//      {
//        URTxWriteIndex = 0;
//        URTxReadIndex = 0;
//        USART_IntConfig(COM2_PORT, USART_INT_TXDE, DISABLE);
//      }
//    }
//  }
//  /* Rx, move data from USART to buffer                                                                      */
//  if (USART_GetFlagStatus(COM2_PORT, USART_FLAG_RXDR))
//  {
//    //Why? USART_ClearFlag(COM1_PORT, USART_FLAG_RXDR);
//    URRxBuf[URRxWriteIndex] = USART_ReceiveData(COM2_PORT);
//		USART_SendData(COM2_PORT, URRxBuf[URRxWriteIndex]);
//    URRxWriteIndex = (URRxWriteIndex + 1) % USART_BUFSIZE;
//  }

//}

/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
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
