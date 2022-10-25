#ifndef __CANTask_H__
#define __CANTask_H__

#include "stm32f4xx.h"                  // Device header
#include "can.h"
/*
	CAN�ķ����������������
	�з�������ͽ������䣬��������ʼ����շ�����
	
	CAN����غ�����
		CAN�ĳ�ʼ��
		CAN���˲�����
		CAN�ķ��ͺ���
		CAN�Ľ����жϣ�CAN1_rxDataHandler������can_protocol������
	
	����̨�ķ������飬���̵ķ�������
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
