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

//����һ��RSSI�ṹ�壬��·�ɱ�һ������¼��ÿ��ģ���RSSIֵ
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
״̬�궨��
*/
#define yes 1
#define no 0
#define NULL 0
#define fail 1
#define done 0
#define true 1
#define falth 0

/*
�������ݵ�����
*/
#define response_type 			  0  	//�ظ���Ϣ����
#define rssi_type							1		//RSSIֵ ͨ����������
#define help_data_type				2		//�����Ϣ����
#define transfer_data_type		3		//ת����Ϣ����

#define ZigBee_MAX_RECV_LEN		100					//�����ջ����ֽ���
#define ZigBee_MAX_SEND_LEN		100					//����ͻ����ֽ���
#define ZigBee_RX_EN 			1					//0,������;1,����.

extern u8  ZigBee_RX_BUF[ZigBee_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  ZigBee_TX_BUF[ZigBee_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern vu16 ZigBee_RX_STA;   						//��������״̬

/*
��������
*/
void uartRevieveByte(u8 data);
sPkg* getNextPkg(void);
u8 sendPkg(sPkg* pkg);

#endif /* PACKAGE_H_ */
