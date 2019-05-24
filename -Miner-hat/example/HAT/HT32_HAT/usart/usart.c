#include "usart_int.h"

/*
该文件存放的是串口的初始化
关于串口中断在 ht32f5xxx_01._it.c
串口的重定向输出在 Retarget分组

串口0用于ZigBee模块接发数据
串口1用于重定向打印调式信息
*/

/* Private variables ---------------------------------------------------------------------------------------*/

void USART1_Init(void )
{
	 USART_InitTypeDef USART_InitStructure;
  
  /* Config AFIO mode as COM1_Rx and COM1_Tx function.                                                      */
  AFIO_GPxConfig(COM1_TX_GPIO_ID, COM1_TX_AFIO_PIN, AFIO_FUN_USART_UART);
  AFIO_GPxConfig(COM1_RX_GPIO_ID, COM1_RX_AFIO_PIN, AFIO_FUN_USART_UART);

  /* COM1 configuration ------------------------------------------------------------------------------------*/
  /* COM1 configured as follow:
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
  USART_Init(COM1_PORT  , &USART_InitStructure);

  /* Seting COM1_PORT interrupt-flag                                                                        */
  USART_IntConfig(COM1_PORT, USART_INT_RXDR, ENABLE);
  /* Enable COM1_PORT                                                                                       */
  USART_TxCmd(COM1_PORT, ENABLE);
  USART_RxCmd(COM1_PORT, ENABLE);
  /* Configure USART0 & USART1 interrupt                                                                    */
  NVIC_EnableIRQ(COM1_IRQn);
	
  //USART_IntConfig(COM1_PORT, USART_INT_TXDE, ENABLE);
  
}
void USART0_Init(void )
{
	USART_InitTypeDef USART_InitStructure;
  
  /* Config AFIO mode as COM1_Rx and COM1_Tx function.                                                      */
  AFIO_GPxConfig(COM0_TX_GPIO_ID, COM0_TX_AFIO_PIN, AFIO_FUN_USART_UART);
  AFIO_GPxConfig(COM0_RX_GPIO_ID, COM0_RX_AFIO_PIN, AFIO_FUN_USART_UART);

  /* COM1 configuration ------------------------------------------------------------------------------------*/
  /* COM1 configured as follow:
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
  USART_Init(COM0_PORT  , &USART_InitStructure);

  /* Seting COM0_PORT interrupt-flag                                                                        */
  USART_IntConfig(COM0_PORT, USART_INT_RXDR, ENABLE);
  /* Enable COM0_PORT                                                                                       */
  USART_TxCmd(COM0_PORT, ENABLE);
  USART_RxCmd(COM0_PORT, ENABLE);
  /* Configure USART0  interrupt                                                                    */
  NVIC_EnableIRQ(COM0_IRQn);
	//USART_IntConfig(COM0_PORT, USART_INT_TXDE, ENABLE);
}

