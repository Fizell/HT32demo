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
//���ڽ��ջ����� 	
u8 ZigBee_RX_BUF[ZigBee_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
u8  ZigBee_TX_BUF[ZigBee_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
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
 * @brief   This function handles UARTx interrupt.  (���ڵ�ʽ���򣬴�ӡ����ʽ��Ϣ)
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
 * @brief   This function handles UARTx interrupt.  (����ZigBeeͨ��)
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
		if((ZigBee_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if(ZigBee_RX_STA<ZigBee_MAX_RECV_LEN)	//�����Խ�������
			{
				 BFTM_SetCounter(HT_BFTM0, 0);//��������� 
				if(ZigBee_RX_STA==0) 				//ʹ�ܶ�ʱ��0���ж� 
				{
					BFTM_EnaCmd(HT_BFTM0, ENABLE);//ʹ�ܶ�ʱ��0
				}
				ZigBee_RX_BUF[ZigBee_RX_STA++]=res;	//��¼���յ���ֵ	 
			}else 
			{
				ZigBee_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
			} 
		}
		
	//	printf("%c", res);
    
  }

}

/*********************************************************************************************************//**
 * @brief  	ͨ�ö�ʱ��0�ж�  ÿ0.1s����һ�¡������ж�ZigBeeģ���Ƿ�������
 * @retval  None
 ************************************************************************************************************/
void BFTM0_IRQHandler(void)
{
  
  //BFTM_ClearFlag(HT_BFTM0);
	//��һ����Ҫ��ϸ���ã����ܴ���Bug
	while (BFTM_GetFlagStatus(HT_BFTM0) != SET);
  {	 			   
		ZigBee_RX_STA|=1<<15;	//��ǽ������
		BFTM_ClearFlag(HT_BFTM0);  //���BTIM0�����жϱ�־    
		BFTM_EnaCmd(HT_BFTM0, DISABLE);//�رն�ʱ��0
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
