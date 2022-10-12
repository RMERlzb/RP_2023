#ifndef __CAN_PROTOCOL_H
#define __CAN_PROTOCOL_H

#include "stm32f4xx.h"                  // Device header
#include "drv_can.h"
#include "chassis_motor.h"
#include "gimbal_motor.h"
#include "shoot_motor.h"
/*
	主要负责对各个电机反馈回来的信息进行存储
	CAN1接收到信息后进入drv_can.h中的HAL_CAN_RxFifo0MsgPendingCallback函数
	然后保存收到的邮件信息，进入这个文件的函数
	
	各个电机的ID号如下
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
