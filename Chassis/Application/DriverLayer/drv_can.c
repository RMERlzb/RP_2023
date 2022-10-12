#include "drv_can.h"

CANTxMsg_t CAN1_TxMsg;
CANRxMsg_t CAN1_RxMsg;

CANTxMsg_t CAN2_TxMsg;
CANRxMsg_t CAN2_RxMsg;

int8_t chassis_txbuf[8] = {0};
int8_t gimbal_txbuf[8] = {0};
int8_t shoot_txbuf[8] = {0};

static void CAN_Filter_Init(CAN_FilterTypeDef *sFilterConfig);

void CAN_Init(void)
{
	//CAN1��ʶ��������
	CAN_FilterTypeDef CAN1_Filter;
	CAN_Filter_Init(&CAN1_Filter);
	HAL_CAN_ConfigFilter(&hcan1, &CAN1_Filter);	
	
	//CAN2��ʶ��������
	CAN_FilterTypeDef CAN2_Filter;
	CAN_Filter_Init(&CAN2_Filter);
	HAL_CAN_ConfigFilter(&hcan2, &CAN2_Filter);	
	
	//ʹ�ܽ����ж�
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
	
	//����CAN1��CAN2
	HAL_CAN_Start(&hcan1);
	HAL_CAN_Start(&hcan2);
}

static void CAN_Filter_Init(CAN_FilterTypeDef *sFilterConfig)
{
	sFilterConfig->FilterIdHigh = 0;						
	sFilterConfig->FilterIdLow = 0;							
	sFilterConfig->FilterMaskIdHigh = 0;					
	sFilterConfig->FilterMaskIdLow = 0;						
	sFilterConfig->FilterFIFOAssignment = CAN_FILTER_FIFO0;	
	sFilterConfig->FilterBank = 0;							
	sFilterConfig->FilterMode = CAN_FILTERMODE_IDMASK;	
	sFilterConfig->FilterScale = CAN_FILTERSCALE_32BIT;	
	sFilterConfig->FilterActivation = ENABLE;				
	sFilterConfig->SlaveStartFilterBank = 0;
	
}

void CAN_Send_Msg(CANTxMsg_t *msg, uint16_t mailbox_id, int8_t *sendbuff)
{
	//����֡������
	msg->TxMessage.StdId = mailbox_id;
	msg->TxMessage.IDE = CAN_ID_STD;
	msg->TxMessage.DLC = 8;
	msg->TxMessage.RTR = CAN_RTR_DATA;
	
	for(int i=0; i<8; i++)
	{
		msg->data[i] = sendbuff[i];
	}
	
	//�����CAN1Ҫ����
	if(msg == &CAN1_TxMsg)
		HAL_CAN_AddTxMessage(&hcan1, &(msg->TxMessage), msg->data, 0);
	
	//�����CAN2Ҫ����
	if(msg == &CAN2_TxMsg)
		HAL_CAN_AddTxMessage(&hcan2, &(msg->TxMessage), msg->data, 0);
	
}
//��can_protocol.c�ļ���
__WEAK void CAN1_rxDataHandler(uint32_t rxId, uint8_t*rxBuf)
{
}

__WEAK void CAN2_rxDataHandler(uint32_t rxId, uint8_t*rxBuf)
{
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	
	if(hcan->Instance == CAN1)
	{
		
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN1_RxMsg.RxMessage, CAN1_RxMsg.data);
		
		CAN1_rxDataHandler(CAN1_RxMsg.RxMessage.StdId, CAN1_RxMsg.data);
	}
	
	if(hcan->Instance == CAN2)
	{
		
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &CAN2_RxMsg.RxMessage, CAN2_RxMsg.data);
		
		CAN2_rxDataHandler(CAN2_RxMsg.RxMessage.StdId, CAN2_RxMsg.data);
	}
}	
