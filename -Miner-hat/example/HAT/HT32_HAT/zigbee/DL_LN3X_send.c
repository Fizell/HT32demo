
#include "package.h"
#include "usart_int.h"
u8 uartSendNextByte(void);




//将待发送的包转换成这个格式统一发送
typedef struct sPkgBase__
{
	u8 length;
	u8 data[63];
}sPkgBase;


//发送计数器,这个计数器记录了下一个要发送的数据在data数组中的偏移量
#define SS_IDLE 0XFF//这个数据为0xff表示目前发送空闲
#define SS_HEAD 0XA0//这个数据为0xA0表示刚刚发送了包头的0xfe等待发送包长度
static u8 Send_counter = SS_IDLE;


//发送包缓冲,待发送的包将被保存在这个结构中
static sPkgBase Send_temp;

/********************************************************
*发送包函数,这个函数负责发送一个包
*参数:	
*	sPkg* pkg		待发送的包,必须将包转换为sPkg格式进行发送
*返回值:	返回fail表示发包失败,通常是因为现在正在发送一个包
*		返回done表示发包成功.done和fail都是lee.h中定义的宏
*********************************************************/		
u8 sendPkg(sPkg* pkg)
{
	u16 i;
	u8 data;
  sPkgBase* pb = (sPkgBase*)pkg;
	if(Send_counter != SS_IDLE)
	{
		return fail;
	}

	Send_temp.length = pb->length;
	for( i = 0;i<Send_temp.length;i++)
	{
		Send_temp.data[i] = pb->data[i];	
	}
	
	//uartSendFirstByte(0xfe);
	
  /*while((HT_USART0->SR&0X40)==0);//等待发送结束
	HT_USART0->DR=0xfe;
	do{
	
	data=uartSendNextByte();
	//这里寄存器的名称不一定跟STM32F103的一样，可能存在bug
  while((HT_USART0->SR&0X40)==0);//等待发送结束
	HT_USART0->DR=data;
		
	}while(data != 0xff);*/
	
	HT_USART0->DR=0xfe;
	
	//表示法发过包头0xfe
	Send_counter = SS_HEAD;
	
	do{
		data=uartSendNextByte();
		
		USART_SendData(COM0_PORT,data);
		//等待发送完成
		while ((HT_USART0->SR & USART_FLAG_TXC) == 0);
		//输出到调试窗口
		//USART_SendData(COM1_PORT,data);
		
	}while(data!=0xff);
	
	return done;
}

/********************************************************
*获取下一个待发送的数据,串口发送缓冲空中断会调用这个函数,
*这个函数会返回本包的下一个待发送的数据.
*
*参数:	无
*返回值:	待发送的下一个数据,返回0xff是这个包的最后一个数据.
*********************************************************/
u8 uartSendNextByte(void)
{
	switch (Send_counter)
	{
	case SS_HEAD:
		Send_counter = 0;
		return Send_temp.length;
	case SS_IDLE:
		return 0xff;
	default:
		if(Send_counter == Send_temp.length)
		{
			Send_counter = SS_IDLE;
			return 0xff;
		}
		else
		{
			if(Send_temp.data[Send_counter] >= 0xfe)
			{
				Send_temp.data[Send_counter]-=2;
				return 0xfe;
			}
			else
			{
				u8 temp = Send_temp.data[Send_counter];
				Send_counter++;
				return temp;
			}
		}
	}
}
