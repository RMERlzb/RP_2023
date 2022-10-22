#include "can_protocol.h"

void CAN1_rxDataHandler(uint32_t rxId, uint8_t *rxBuf)
{
	if(rxId == CHASSIS_CAN_ID_LF)
	{
		// 更新底盘电机数据
		chassis_motor[CHAS_LF].info_update(&chassis_motor[CHAS_LF], rxBuf);
		chassis_motor[CHAS_LF].check(&chassis_motor[CHAS_LF]);
	}
	/* 右前轮 */
	else if(rxId == CHASSIS_CAN_ID_RF)
	{
		// 更新底盘电机数据
		chassis_motor[CHAS_RF].info_update(&chassis_motor[CHAS_RF], rxBuf);
		chassis_motor[CHAS_RF].check(&chassis_motor[CHAS_RF]);
	}
	/* 左后轮 */
	else if(rxId == CHASSIS_CAN_ID_LB)
	{
		// 更新底盘电机数据
		chassis_motor[CHAS_LB].info_update(&chassis_motor[CHAS_LB], rxBuf);
		chassis_motor[CHAS_LB].check(&chassis_motor[CHAS_LB]);
	}
	/* 右后轮 */
	else if(rxId == CHASSIS_CAN_ID_RB)
	{
		// 更新底盘电机数据
		chassis_motor[CHAS_RB].info_update(&chassis_motor[CHAS_RB], rxBuf);
		chassis_motor[CHAS_RB].check(&chassis_motor[CHAS_RB]);
	}
	//下云台
	else if(rxId == GIMBAL_CAN_ID_LOW)
	{
		gimbal_motor[GIMBAL_LOW].info_update(&gimbal_motor[GIMBAL_LOW], rxBuf);
		gimbal_motor[GIMBAL_LOW].check(&gimbal_motor[GIMBAL_LOW]);
	}
	//上云台
	else if(rxId == GIMBAL_CAN_ID_HIGH)
	{
		gimbal_motor[GIMBAL_HIGH].info_update(&gimbal_motor[GIMBAL_HIGH], rxBuf);
		gimbal_motor[GIMBAL_HIGH].check(&gimbal_motor[GIMBAL_HIGH]);
	}
}

void CAN2_rxDataHandler(uint32_t rxId, uint8_t *rxBuf)
{	
	if(rxId == Shoot_FricWheel_L)
	{
		shoot_motor[SHOOT_LRUB].info_update(&shoot_motor[SHOOT_LRUB], rxBuf);
		shoot_motor[SHOOT_LRUB].check(&shoot_motor[SHOOT_LRUB]);
	}
	
	else if(rxId == Shoot_FricWheel_R)
	{
		shoot_motor[SHOOT_RRUB].info_update(&shoot_motor[SHOOT_RRUB], rxBuf);
		shoot_motor[SHOOT_RRUB].check(&shoot_motor[SHOOT_RRUB]);
	}	
	else if(rxId == Shoot_DialMotor)
	{
		shoot_motor[SHOOT_DIAL].info_update(&shoot_motor[SHOOT_DIAL], rxBuf);
		shoot_motor[SHOOT_DIAL].check(&shoot_motor[SHOOT_DIAL]);
	}	
	else if(rxId == Shoot_GunMotor)
	{
		shoot_motor[SHOOT_GUN].info_update(&shoot_motor[SHOOT_GUN], rxBuf);
		shoot_motor[SHOOT_GUN].check(&shoot_motor[SHOOT_GUN]);
	}
}
