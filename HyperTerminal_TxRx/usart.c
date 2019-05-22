/*********************************************************************************************************//**
 * @file    USART/HyperTerminal_TxRx/main.c
 * @version $Rev:: 686          $
 * @date    $Date:: 2016-05-26 #$
 * @brief   Main program.
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
#include <string.h>

void initUsart1()
{
	
	
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	USART_InitTypeDef USART_InitStructure;
	
  CKCUClock.Bit.AFIO   = 1;
  COM1_CLK(CKCUClock)  = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Config AFIO mode as USART0_Rx and USART0_Tx function.                                                  */
  AFIO_GPxConfig(COM1_TX_GPIO_ID, COM1_TX_AFIO_PIN, AFIO_FUN_USART_UART);
  AFIO_GPxConfig(COM1_RX_GPIO_ID, COM1_RX_AFIO_PIN, AFIO_FUN_USART_UART);

  /* USART0 configuration ----------------------------------------------------------------------------------*/
  /* USART0 configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - None parity bit
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
	
	USART_Init(COM1_PORT, &USART_InitStructure);
	
	
	
	USART_TxCmd(COM1_PORT, ENABLE);
  USART_RxCmd(COM1_PORT, ENABLE);
	
	NVIC_EnableIRQ(COM1_IRQn);
//	USART_IntConfig(COM1_PORT, USART_INT_TXDE, ENABLE);
	
	
	
	/* COM1 Tx                                                                                                 */
}

//void initUsart2()
//{
//	
//	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
//	USART_InitTypeDef USART_InitStructure;
//	
//  CKCUClock.Bit.AFIO   = 1;
//  COM2_CLK(CKCUClock)  = 1;
//  CKCU_PeripClockConfig(CKCUClock, ENABLE);

//  /* Config AFIO mode as USART0_Rx and USART0_Tx function.                                                  */
//  AFIO_GPxConfig(COM2_TX_GPIO_ID, COM2_TX_AFIO_PIN, AFIO_FUN_USART_UART);
//  AFIO_GPxConfig(COM2_RX_GPIO_ID, COM2_RX_AFIO_PIN, AFIO_FUN_USART_UART);

//  /* USART0 configuration ----------------------------------------------------------------------------------*/
//  /* USART0 configured as follow:
//        - BaudRate = 115200 baud
//        - Word Length = 8 Bits
//        - One Stop Bit
//        - None parity bit
//  */
//  USART_InitStructure.USART_BaudRate = 115200;
//  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
//  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
//  USART_InitStructure.USART_Parity = USART_PARITY_NO;
//  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;

//  USART_Init(COM2_PORT, &USART_InitStructure);
//	
//	USART_IntConfig(COM2_PORT, USART_INT_TXDE, ENABLE);
//	
//  USART_TxCmd(COM2_PORT, ENABLE);
//  USART_RxCmd(COM2_PORT, ENABLE);
//	
//	NVIC_EnableIRQ(COM1_IRQn);
//	
//}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
