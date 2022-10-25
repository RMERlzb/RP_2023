#ifndef __CANTask_H__
#define __CANTask_H__

#include "stm32f4xx.h"                  // Device header
#include "can.h"
/*
	CAN的发送与接收驱动函数
	有发送邮箱和接收邮箱，邮箱包括邮件和收发数组
	
	CAN的相关函数：
		CAN的初始化
		CAN的滤波配置
		CAN的发送函数
		CAN的接收中断，CAN1_rxDataHandler函数在can_protocol函数里
	
	有云台的发送数组，底盘的发送数组
*/
typedef struct {
	CAN_RxHeaderTypeDef RxMessage;
	uint8_t data[8];
} CANRxMsg_t;

typedef struct {
	CAN_TxHeaderTypeDef TxMessage;
	uint8_t	data[8];
} CANTxMsg_t;

extern CANTxMsg_t CAN1_TxMsg;
extern CANRxMsg_t CAN1_RxMsg;

extern CANTxMsg_t CAN2_TxMsg;
extern CANRxMsg_t CAN2_RxMsg;

extern int8_t chassis_txbuf[8];
extern int8_t gimbal_txbuf[8];
extern int8_t shoot_txbuf[8];

void CAN_Init(void);
static void CAN_Filter_Init(CAN_FilterTypeDef *sFilterConfig);
void CAN_Send_Msg(CANTxMsg_t *msg,uint16_t mailbox_id,int8_t *sendbuff);
uint8_t HXZP_Tx_uint8(uint32_t std, uint8_t *data,char can,uint32_t DL);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
#endif
