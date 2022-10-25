#include "Auto.h"
uint32_t CNT = 0;
/*
注意：
	由视觉掌握云台控制权写在了imu_pid_cal中了
*/
extern UART_HandleTypeDef huart3;

#define VISION_TX_BUF_LEN LEN_VISION_TX_PACKET

uint8_t vision_dma_txbuf[VISION_TX_BUF_LEN];

Vision_Cmd_Id_t AUTO_CMD_MODE = CMD_AIM_OFF;

char tar_change_enable = 0;

void Vision_TX(void)
{

	//-------------------------发送帧头--------------------------------
	vision_dma_txbuf[sof] = VISION_FRAME_HEADER;
	
	vision_dma_txbuf[Cmd_ID] = AUTO_CMD_MODE;	
	
	
	
	//------------------------填充发送数组------------------------------------
	Append_CRC8_Check_Sum(vision_dma_txbuf, LEN_FRAME_HEADER);
	
	memcpy(&vision_dma_txbuf[Data], &vision_sensor.info->TxPacket.TxData, LEN_TX_DATA);
	
	Append_CRC16_Check_Sum(vision_dma_txbuf, LEN_VISION_TX_PACKET);
	
	
	/*保存帧头*/
	
	vision_sensor.info->TxPacket.FrameHeader.sof    = vision_dma_txbuf[sof];
	vision_sensor.info->TxPacket.FrameHeader.cmd_id = (Vision_Cmd_Id_t)vision_dma_txbuf[Cmd_ID];
	vision_sensor.info->TxPacket.FrameHeader.crc8   = vision_dma_txbuf[Crc8];
	
	/*保存帧尾*/
	
	vision_sensor.info->TxPacket.FrameTailer.crc16 = (uint16_t)vision_dma_txbuf[TX_CRC16] <<8 | \
																									 (uint16_t)vision_dma_txbuf[TX_CRC16 + 1] ;			
	
	/*修改目标值，当回到视觉控制的时候，现在的云台在哪里就在哪里画圆圈*/
	if( rc_sensor_dial.Version_Control_Enable == True )
	{
		if( tar_change_enable == True )
		{
			tar_change_enable = Flase;
			
			vision_sensor.info->TxPacket.TxData.yaw    = (yaw + 180) * 22.7555f;
			
			vision_sensor.info->TxPacket.TxData.pitch  = (pitch + 180) * 22.7555f;

		}
	}
	
	//如果视觉控制关闭，或者遥控关闭了再开机
	if( rc_sensor_dial.Version_Control_Enable == Flase || \
		  rc_sensor.work_state == DEV_OFFLINE )
		tar_change_enable = True;
	
	
	//-----------------------DMA形式发送---------------------------------------
	HAL_UART_Transmit_DMA(&huart3, vision_dma_txbuf, LEN_VISION_TX_PACKET);
			
}
