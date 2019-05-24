/*
 * package.h
 *
 * Created: 2016/6/14 15:36:14
 *  Author: lee-home
 */ 


#ifndef PACKAGE_H_
#define PACKAGE_H_
#include "ht32.h"
#include "ht32_board.h"

typedef struct sPkg__
{
	u8 length;
	u8 src_port;
	u8 dis_port;
	u8 remote_addrH;
	u8 remote_addrL;
	u8 data[59];	
	}sPkg;

//定义一个RSSI结构体，像路由表一样，记录下每个模块的RSSI值
typedef struct rssiData_
{
	u8 ID[2];
	u8 data;
}rssiData;
	
#define newPkg(num)	\
struct 						\
{							\
	u8 length;			\
	u8 src_port;			\
	u8 dis_port;			\
	u8 remote_addrH;\
	u8 remote_addrL;\
	u8 data[num];		\
}

/*
状态宏定义
*/
#define yes 1
#define no 0
#define NULL 0
#define fail 1
#define done 0
#define true 1
#define falth 0

/*
定义数据的类型
*/
#define response_type 			  0  	//回复信息类型
#define rssi_type							1		//RSSI值 通信质量测量
#define help_data_type				2		//求救信息类型
#define transfer_data_type		3		//转发信息类型

#define ZigBee_MAX_RECV_LEN		100					//最大接收缓存字节数
#define ZigBee_MAX_SEND_LEN		100					//最大发送缓存字节数
#define ZigBee_RX_EN 			1					//0,不接收;1,接收.

extern u8  ZigBee_RX_BUF[ZigBee_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  ZigBee_TX_BUF[ZigBee_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern vu16 ZigBee_RX_STA;   						//接收数据状态

/*
函数声明
*/
void uartRevieveByte(u8 data);
sPkg* getNextPkg(void);
u8 sendPkg(sPkg* pkg);

#endif /* PACKAGE_H_ */
