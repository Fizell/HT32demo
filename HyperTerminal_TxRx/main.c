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

//#define USART_BUFSIZE   128

//extern u8 URTxBuf[USART_BUFSIZE];
//extern u8 URRxBuf[USART_BUFSIZE];
//extern vu32 URTxReadIndex;
//extern vu32 URTxWriteIndex;
//extern vu32 URRxWriteIndex;

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
  * @brief  Main program.
  * @retval None
  * @details Main program as following
  *  - Enable peripheral clock of AFIO, USART0.
  *  - Config AFIO mode as USART0_Rx and USART0_Tx function.
  *  - USART0 configuration:
  *     - BaudRate = 115200 baud
  *     - Word Length = 8 Bits
  *     - One Stop Bit
  *     - None parity bit
  *  - USART0 Tx "Hello World!" string 10 times.
  *  - USART0 Rx character and transform to hex.(Loop)
  *
  ***********************************************************************************************************/
int main(void)
{
  
  int input;
	
	initUsart1();
	
//	initUsart2();
	
  /* USART0 Tx "Hello World!" string 10 times.                                                              */
  for (input = 0; input < 10; input++)
  {
    printf("Hello World! %d\r\n", (int)input);
		USART_SendData(COM1_PORT, 'a');
  }
	
	printf("OKOKOK!!!\n");
	
	while(1)
	{
		USART_IntConfig(COM1_PORT, USART_INT_TXDE, ENABLE);
//		printf("Jiangyizhoushabi!!\n");
		;
//		printf("Hello World! %d\r\n", (int)input);
//		USART_SendData(COM1_PORT, 'a');
	}
	
  /* USART0 Rx character and transform to hex.(Loop)                                                        */
//  while (1)
//  {	
//		USART_SendData(COM1_PORT,'a');
//    printf("Please input key for printf....");
//    input = getchar();
//    printf("\r\nYour input is %c[0x%x]\r\n\r\n", input, input);
//  }
}

#if (HT32_LIB_DEBUG == 1)
/*********************************************************************************************************//**
  * @brief  Report both the error name of the source file and the source line number.
  * @param  filename: pointer to the source file name.
  * @param  uline: error line source number.
  * @retval None
  ***********************************************************************************************************/
void assert_error(u8* filename, u32 uline)
{
  /*
     This function is called by IP library that the invalid parameters has been passed to the library API.
     Debug message can be added here.
     Example: printf("Parameter Error: file %s on line %d\r\n", filename, uline);
  */

  while (1)
  {
  }
}
#endif


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
