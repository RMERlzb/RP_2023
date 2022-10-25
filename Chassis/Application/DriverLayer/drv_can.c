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
	//CAN1标识符过滤器
	CAN_FilterTypeDef CAN1_Filter;
	CAN_Filter_Init(&CAN1_Filter);
	HAL_CAN_ConfigFilter(&hcan1, &CAN1_Filter);	
	
	//CAN2标识符过滤器
	CAN_FilterTypeDef CAN2_Filter;
	CAN_Filter_Init(&CAN2_Filter);
	HAL_CAN_ConfigFilter(&hcan2, &CAN2_Filter);	
	
	//使能接收中断
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
	
	//开启CAN1和CAN2
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
	//数据帧的填入
	msg->TxMessage.StdId = mailbox_id;
	msg->TxMessage.IDE = CAN_ID_STD;
	msg->TxMessage.DLC = 8;
	msg->TxMessage.RTR = CAN_RTR_DATA;
	
	for(int i=0; i<8; i++)
	{
		msg->data[i] = sendbuff[i];
	}
	
	//如果是CAN1要发送
	if(msg == &CAN1_TxMsg)
		HAL_CAN_AddTxMessage(&hcan1, &(msg->TxMessage), msg->data, 0);
	
	//如果是CAN2要发送
	if(msg == &CAN2_TxMsg)
		HAL_CAN_AddTxMessage(&hcan2, &(msg->TxMessage), msg->data, 0);
	
}



uint8_t HXZP_Tx_uint8(uint32_t std, uint8_t *data,char can,uint32_t DL)
{
	uint8_t i;
	CANTxMsg_t TxMes;
	uint32_t CAN_Tx_Mailbox;
	
	TxMes.TxMessage.IDE = CAN_ID_STD;
	TxMes.TxMessage.RTR = CAN_RTR_DATA;
	TxMes.TxMessage.DLC = DL;    //------------个数
	TxMes.TxMessage.StdId = std; 	  //ID号
	
	TxMes.data[0] = data[0];
	TxMes.data[1] = data[1];	
	TxMes.data[2] = data[2];	
	TxMes.data[3] = data[3];

	TxMes.data[4] = data[4];
	TxMes.data[5] = data[5];	
	TxMes.data[6] = data[6];	
	TxMes.data[7] = data[7];
	
	if(can == 1)i = HAL_CAN_AddTxMessage(&hcan1,&TxMes.TxMessage,TxMes.data,  &CAN_Tx_Mailbox);
	if(can == 2)i = HAL_CAN_AddTxMessage(&hcan2,&TxMes.TxMessage,TxMes.data,  &CAN_Tx_Mailbox);
	return i;
}

//在can_protocol.c文件中
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
