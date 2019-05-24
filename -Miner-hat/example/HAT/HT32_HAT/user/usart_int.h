/*********************************************************************************************************//**
 * @file    USART/HyperTerminal_TxRx_Interrupt/usart_int.h
 * @version $Rev:: 2350         $
 * @date    $Date:: 2017-12-12 #$
 * @brief   The header file of the USART interrupt functions.
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

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __USART_INT_H
#define __USART_INT_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "ht32_config.h"
#include "ht32_board.h"
#include <string.h>
/*端口定义声名  --------------------------------------------------------------------------------------------*/


#define COM0_CLK(CK)                (CK.Bit.USART0)
#define COM0_PORT                   (HT_USART0 )   //这一个宏定义有可能出错
#define COM0_IRQn                   (USART0_IRQn)
#define COM0_IRQHandler             (USART0_IRQHandler)

#define COM0_TX_GPIO_ID             (GPIO_PA)
#define COM0_TX_AFIO_PIN            (AFIO_PIN_2)
#define COM0_TX_AFIO_MODE           (AFIO_FUN_USART_UART)

#define COM0_RX_GPIO_ID             (GPIO_PA)
#define COM0_RX_AFIO_PIN            (AFIO_PIN_3)
#define COM0_RX_AFIO_MODE           (AFIO_FUN_USART_UART)

/*端口定义声名  --------------------------------------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------------------------------------*/
#define USART_BUFSIZE   128

extern u8 URTxBuf[USART_BUFSIZE];
extern u8 URRxBuf[USART_BUFSIZE];
extern vu32 URTxReadIndex;
extern vu32 URTxWriteIndex;
extern vu32 URRxWriteIndex;



/*   函数声明   ----------------------------------------------------------------------------------------------*/

void USART1_Init(void );
void USART0_Init(void );

/*   函数声明   ----------------------------------------------------------------------------------------------*/

#endif
