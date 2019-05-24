/*********************************************************************************************************//**
 * @file    USART/HyperTerminal_TxRx_Interrupt/main.c
 * @version $Rev:: 2350         $
 * @date    $Date:: 2017-12-12 #$
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
#include "usart_int.h"
#include <string.h>
#include "time.h"
#include "package.h"
#include "ht32_board_config.h"
 
/*
初始化了USART1与USART0
重定向函数在 Retarget文件夹中
中断函数在 ht32f5xxx_01.c文件中
*/

void init_all(void);
void loopReceive(void);
void recievePkg(sPkg* pkg);
void loopAll(void );
int check_type(void);
void update_rssi_table(u8* data);
void send_rssi(void );
void delay(u32 num);
void send_rssi_to_station(void );
void init_GPIO_D1();
/*
读取网络中所有节点的RSSI值，通过RSSI值来评估节点的距离，选择RSSI值最大(非128)的ZigBee模块
*/
char rssi_buf_0x1a[]={0x06 ,0x80 ,0x23 ,0x3A ,0x06 ,0x1A,0x06 ,0xFF};
char rssi_buf_0x2a[]={0x06 ,0x80 ,0x23 ,0x3A ,0x06 ,0x2A,0x06 ,0xFF};
char rssi_buf_0x4a[]={0x06 ,0x80 ,0x23 ,0x3A ,0x06 ,0x4A,0x06 ,0xFF};

//rssi 回复的数据头
char rssi_react[]={0xFE ,0x07 ,0x23 ,0x80};
//RSSI表格
rssiData rssi_data_table[10]={{0x06,0x1a,0},{0x06,0x2a,0},{6,0x4a,0}};

int usart1_flag=0;


#define ZigBeeNum 4


//A2端口用来发送报警信息
newPkg(13) help_message4a={9,0xA2,0xA2,0x4A,0x06,0x68, 0x65 ,0x6c ,0x70,0x21};
//直接发送给通过基站进行测试
newPkg(13) help_message2a={9,0xA2,0xA2,0x2A,0x06,0x68, 0x65 ,0x6c ,0x70,0x21};
//发送给哦通信节点
newPkg(13) help_message1a={9,0xA2,0xA2,0x1A,0x06,0x68, 0x65 ,0x6c ,0x70,0x21};


int main(void)
{	 
	u8 i,j;
	int wear_flag = TRUE;
	FlagStatus TmpStatus = RESET;
	init_all();
	printf("hello world!\n");
	
	//一开始先发送RSSI来检测、更新通信质量
	send_rssi();
//	
//	while(1)
//	{		
//		if(GPIO_ReadInBit(HTCFG_KEY1, HTCFG_INPUT_KEY1_GPIO_PIN))
//		{
//			printf("1");
//			GPIO_WriteOutBits(HTCFG_LED1, HTCFG_OUTPUT_LED1_GPIO_PIN, 1);
//		}
//	}
	while (1)
  {
		wear_flag = GPIO_ReadInBit(HTCFG_KEY1, HTCFG_INPUT_KEY1_GPIO_PIN);
		GPIO_WriteOutBits(HTCFG_LED1, HTCFG_OUTPUT_LED1_GPIO_PIN, wear_flag);
		loopAll();
		loopReceive();
		//如果串口1接受到数据发送报警信号到0x1a
		if(usart1_flag==1)
		{
			usart1_flag=0;
			sendPkg((sPkg*)(&help_message1a));
			//printf("发送给1成功");
		}
		else if(usart1_flag==2)
		{
			usart1_flag=0;
			sendPkg((sPkg*)(&help_message2a));
			printf("发送给2成功");
		}
		else if(usart1_flag==4)
		{
			usart1_flag=0;
			sendPkg((sPkg*)(&help_message4a));
			printf("发送给4成功");
		}
		else if(usart1_flag==5)
		{
			usart1_flag=0;
			send_rssi();
		}
		else if(usart1_flag==6)
		{
			usart1_flag=0;
			send_rssi_to_station();
		}
		
	}
}

void init_GPIO_D1()
{
	
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
	
	HTCFG_OUTPUT_LED1_CLK(CKCUClock) = 1;
  HTCFG_INPUT_KEY1_CLK(CKCUClock)  = 1;
  CKCUClock.Bit.AFIO       = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
	
	AFIO_GPxConfig(HTCFG_INPUT_KEY1_ID, HTCFG_INPUT_KEY1_AFIO_PIN, AFIO_FUN_GPIO);
	GPIO_DirectionConfig(HTCFG_KEY1, HTCFG_INPUT_KEY1_GPIO_PIN, GPIO_DIR_IN);
	GPIO_PullResistorConfig(HTCFG_KEY1, HTCFG_INPUT_KEY1_GPIO_PIN, GPIO_PR_DISABLE);
	GPIO_InputConfig(HTCFG_KEY1, HTCFG_INPUT_KEY1_GPIO_PIN, ENABLE);
	AFIO_GPxConfig(HTCFG_OUTPUT_LED1_ID, HTCFG_OUTPUT_LED1_AFIO_PIN, AFIO_FUN_GPIO);
	GPIO_DirectionConfig(HTCFG_LED1, HTCFG_OUTPUT_LED1_GPIO_PIN, GPIO_DIR_OUT);

}
void init_clock()
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
 
	 /* Enable peripheral clock of AFIO, USART0, USART1                                                                  */
  CKCUClock.Bit.AFIO       = 1;
  COM1_CLK(CKCUClock)      = 1;
	COM0_CLK(CKCUClock)  		 = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
	//开通用定时器0的时钟
	CKCUClock.Bit.BFTM0      = 1;
	
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
}

void init_all()
{
	init_clock();
	init_BFTM0();
	USART1_Init();
	USART0_Init();
	init_GPIO_D1();
}

void loopAll()
{ 
	u16 i,j;
	u8 reclen=0;  
	u8 type;
	u8 data[216];
	//接受10次数据
	for(i = 0;i<10;i++)
			loopReceive(); 
	if(ZigBee_RX_STA&0X8000)			//接收到一次数据了
	 {
		
		 reclen=ZigBee_RX_STA&0X7FFF;	//得到数据长度

		 for(j=0,i=0;i<reclen;i++)
		{
			uartRevieveByte(ZigBee_RX_BUF[i]);
			printf("%c",ZigBee_RX_BUF[i]);
		
			if(i>5&&i<reclen-1)
				data[j++]=ZigBee_RX_BUF[i];
		}
			ZigBee_RX_BUF[i]='\0';
			data[j+1]='\0';
			type=check_type();
		 //如果是rssi类型就更新一下rssi表
		 if(type==rssi_type)
				update_rssi_table(data);
		 else if(type==response_type)
		 {
				
		 }
		 //printf(" %s",data);
		 ZigBee_RX_STA=0;	 
	 } 						
}

int check_type()
{
	//查看收到的信息是什么类型的
	if(ZigBee_RX_BUF[2]==0x23)
	{
		if(ZigBee_RX_BUF[1]==rssi_react[1]&&ZigBee_RX_BUF[3]==rssi_react[3])
			return rssi_type;
	}
	//A2端口是报警信息
	else if(ZigBee_RX_BUF[2]==0xA2)
			return help_data_type;
	//A3是转发端口
	else if(ZigBee_RX_BUF[2]==0xA3)
			return transfer_data_type;
	else
			return response_type; 
}

//搜索最近的RSSI值
void send_rssi()
{
	//我们连续发送RSSI值如果没有适当延时的话会造成数据混乱
	//存在一个问题，当发送俩个RSSI值时可以正常运行，但是当发送3个时
	//最后一个数据发不出去
	//暴力延时，没有精准的延时
	
	sendPkg((sPkg*)(&rssi_buf_0x1a)); //用于检测0x1a的RSSI值
	
	delay(0x03);
	
	//接收数据
	loopAll();
	
	/*sendPkg((sPkg*)(&rssi_buf_0x2a)); //用于检测0x2a的RSSI值
	
	delay(0x12);
	
	//接收数据
	loopAll();*/
	//延时
	delay(0x04);
	
	sendPkg((sPkg*)(&rssi_buf_0x4a)); //用于检测0x4a的RSSI值
	
	//接收数据
	loopAll();

}

void send_rssi_to_station(void )
{
	//测量通信基站的通信质量
	sendPkg((sPkg*)(&rssi_buf_0x2a)); //用于检测0x2a的RSSI值
	delay(0x04);
	loopAll();

}

void update_rssi_table(u8* data)
{
	//更新RSSI表的值
	//目前只测量0x1A,0x4A这俩个节点的，默认0x2A通信基站的通信质量很好
	u8 i;
	for(i=0;i<ZigBeeNum;i++)
	{
		//printf("%c%c%c",0x64,rssi_data_table[i].ID[1],data[0]);
		if(rssi_data_table[i].ID[1]==data[0]&&data[0]<128)
		{
			rssi_data_table[i].data=data[2];
			printf("!%c",rssi_data_table[i].data);
		}
	}
		//printf("!%c%c",data[0],ZigBee_RX_BUF[8]);
		//打印输出更新后的结果
	for(i=0;i<ZigBeeNum-1;i++)
		{
			printf("%c%c%c\r\n",rssi_data_table[i].ID[0],rssi_data_table[i].ID[1],\
			rssi_data_table[i].data);
		}
}

//这个函数需要在工作中不断被调用,它会尝试一次接收包,
//如果接收成功就交给recievePkg处理,并再次尝试,直到收不到新的包
void loopReceive(void)
{
	sPkg* pkg;
	pkg = getNextPkg();
	while(pkg != NULL)
	{
		recievePkg(pkg);
		pkg = getNextPkg();
	}
}

//收到一个包后会调用这个函数,这个函数根据包的目的端口选择相应的程序进行处理
void recievePkg(sPkg* pkg)
{
	//printf((char*) pkg);//通过串口1发送给电脑，用于测试
	switch(pkg->dis_port)
	{
		case 0xb0:		
		if(pkg->data[0] == 0x01)
		{
		//	greenTog();
		//闪烁LED,提示系统正在运行.
		}
		break;
		default:
		break;		
	}
}

void delay(u32 num)
{
	u32 i,j;
	while(--num>0)
	{
		j=0xfffe;
		while(--j>0);
	}
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

