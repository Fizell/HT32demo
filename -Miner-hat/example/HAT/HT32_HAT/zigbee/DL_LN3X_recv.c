/*
 * DL_LN3X.c
 *
 * Created: 2017-03-07
 *  Author: mchen_6431
 */ 

#include "package.h"
#include "usart_int.h"


//接收包使用的结构体,将端口和地址都融入了data中
typedef struct sPkgBase__
{
	u8 length;
	u8 data[63];
}sPkgBase;

//收到包头函数
static void recvHead(u8 totle);
//收到包尾0xff
static void recvTerminal(void);
//收到数据函数
static void recvData(u8 data);
//这个标志在上一个收到了0xfe时赋值为yes,否则为0
static u8 escape = no;

//串口收到1byte数据时,中断中调用此函数
void uartRevieveByte(u8 data)
{
	//add by jense 
	//printf("%c",data);
	switch(data)
	{
		case 0xff://收到结束符
		recvTerminal();
		break;
		case 0xfe://收到转义字符
		escape = yes;
		break;
		default://收到一般数据
		if(escape == yes)
		{//如果前一个是转义字符
			escape = no;
			if(data <=63)
			{
				recvHead(data);
			}
			else
			{//收到转义数据的规律是0xfe后面的数据+2还原数据
				recvData(data+2);
			}
		}
		else
		{//前一个不是转义字符,直接收到一个数据
			recvData(data);
		}
		break;
	}
}



//接收包使用的双缓冲,其中一个供程序使用,另一个用来装入下一个包.
static sPkgBase recv_temp[2];
//确定双缓冲中哪个用于接收,哪个程序分析的变量
static volatile  u8 Loading = 0;
//用于接收装入数据的缓冲
#define Load_pkg recv_temp[Loading]
//用于程序分析的缓冲
#define User_pkg recv_temp[(Loading+1)&1]

//接收计数器,表示一个包收到了多少个数据
#define RS_IDLE 0XFF//表示包还没有收到包头和长度
#define RS_DONE 0xA0//表示包已经完成了接收
static volatile u8 Recv_counter = RS_IDLE;

//如果User_pkg装有一个未处理的包,此变量为 yes
static volatile u8 Received = no;
//如果用户正在分析User_pkg中的数据此变量为 yes
static volatile u8 Locked = no;


//调用这个函数说明上一个包已经处理完了,并尝试接收一个包,
//如果已经收到包,则返回一个指向包的指针,否则返回null
sPkg* getNextPkg(void)
{
//	cli();
	sPkg* rev;
	
	Locked = no;

	if(Received == yes)
	{//两个缓冲区交换过,receive就是yes进入这里
		Received =no;
		Locked = yes;
		rev = (sPkg*)(&User_pkg);
	}
	else
	{//还没有交换,那么是否应该交换呢?
		if(Recv_counter == RS_DONE)
		{//如果另一个缓冲已经收完了就应该交换
			Loading++;
			Loading&=1;
			Recv_counter = RS_IDLE;	
			Locked = yes;
			rev = (sPkg*)(&User_pkg);			
		}
		else
		{//否则返回空,说明没有包待处理
			rev = NULL;
		}		
	}	
	//sei();
	return rev;
}



static void recvHead(u8 totle)
{
	if(Recv_counter == RS_IDLE)
	{
		Load_pkg.length = totle;
		Recv_counter = 0;
	}
}

static void recvData(u8 data)
{	
	if(Recv_counter < Load_pkg.length)
	{		
		Load_pkg.data[Recv_counter] = data;
		Recv_counter++;
	}
	else
	{
		Recv_counter = RS_IDLE;
	}
}

static void recvTerminal(void)
{
	if(Recv_counter == Load_pkg.length)
	{//收到完成的包
		if(Locked == no)
		{//调换缓冲区
			Loading++;
			Loading&=1;
			Received = yes;
			Recv_counter = RS_IDLE;
		} 
		else
		{//不调换缓冲区
			Recv_counter = RS_DONE;
		}
	}
	else
	{	//收到不完成的包,需要重新收	
		Recv_counter = RS_IDLE;
	}
}
