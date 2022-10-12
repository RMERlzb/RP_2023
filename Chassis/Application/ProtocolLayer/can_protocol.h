#ifndef __CAN_PROTOCOL_H
#define __CAN_PROTOCOL_H

#include "stm32f4xx.h"                  // Device header
#include "drv_can.h"
#include "chassis_motor.h"
#include "gimbal_motor.h"
#include "shoot_motor.h"
/*
	��Ҫ����Ը������������������Ϣ���д洢
	CAN1���յ���Ϣ�����drv_can.h�е�HAL_CAN_RxFifo0MsgPendingCallback����
	Ȼ�󱣴��յ����ʼ���Ϣ����������ļ��ĺ���
	
	���������ID������
*/

#define CHASSIS_CAN_ID_LF 	(0x201U)
#define CHASSIS_CAN_ID_RF		(0x202U)
#define CHASSIS_CAN_ID_LB		(0x203U)
#define CHASSIS_CAN_ID_RB		(0x204U)

#define GIMBAL_CAN_ID_LOW 	(0x205U)
#define GIMBAL_CAN_ID_HIGH  (0x206U)

#define Shoot_RubMotor_R	(0x201U)
#define Shoot_RubMotor_L	(0x202U)
#define Shoot_DialMotor		(0x203U)
#define Shoot_GunMotor		(0x204U)

extern void CAN1_rxDataHandler(uint32_t rxId, uint8_t *rxBuf);
extern void CAN2_rxDataHandler(uint32_t rxId, uint8_t *rxBuf);
#endif
