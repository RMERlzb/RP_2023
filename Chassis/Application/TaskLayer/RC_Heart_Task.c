#include "RC_Heart_Task.h"
uint32_t tick = 0;

dev_work_state_t RC_last_state = DEV_OFFLINE, RC_this_state = DEV_OFFLINE;

char Mec_Return_flag = 0;

void Offline_Reset_Var(void);
void Car_Unload(void);
void Judge_RC_Transtate(void);


void StartRC_HeartTask(void const * argument)
{
  rc_sensor.init(&rc_sensor);
  for(;;)
  {		
		rc_sensor.heart_beat(&rc_sensor);	
		
		Judge_RC_Transtate();
				
		//一开始通讯成功时，让云台自己回到机械角度2010位置
		if(rc_sensor.tran_state == DEV_BOOT)
		{
			if( Mechanical_Return() )
			{
				rc_sensor.tran_state = DEV_OFF;	
				Mec_Return_flag = True;
			}											
		}	
		
		//成功与遥控器通讯
		if( RC_ONLINE )
		{
			Cover_Awake();
			rc_sensor.check(&rc_sensor);	
			tick = 0;		
		}
		
		//失联----复位变量的值，卸力
		else
		{
			
			Cover_Sleep();
			Offline_Reset_Var();
			Car_Unload();
		
		}
		
    osDelay(1);
  }
  
}


//失联下重置变量的值
void Offline_Reset_Var(void)
{
	
	Gimbal_Reset();
	
	rc_sensor.dial_reset(&rc_sensor);
	
	
	//遥控器开关机判断的辅助全局变量
	RC_last_state = RC_this_state = DEV_OFFLINE;
	
	//完成机械回位的标志
	Mec_Return_flag = Flase;
	
}


void Car_Unload(void)
{
	
	if( tick == 0 )
		tick = HAL_GetTick();
	
	if( HAL_GetTick() - tick >= 500 )		
	{
		memset( chassis_txbuf, 0, sizeof(chassis_txbuf) );
		CAN_Send_Msg( &CAN1_TxMsg, Chassis_Control_ID, chassis_txbuf );		
				
		memset( gimbal_txbuf, 0, sizeof(gimbal_txbuf) );
		CAN_Send_Msg( &CAN1_TxMsg, Gimbal_Control_ID, gimbal_txbuf );			

		memset( shoot_txbuf, 0, sizeof(shoot_txbuf) );
		CAN_Send_Msg( &CAN2_TxMsg, Shoot_Control_ID, shoot_txbuf );			
	}	
	
	//底盘先减速到0，发射装置也减速到0
	else
	{
		chassis_motor[CHAS_LF].speed_pid->target = 0;
		chassis_motor[CHAS_RF].speed_pid->target = 0;
		chassis_motor[CHAS_LB].speed_pid->target = 0;
		chassis_motor[CHAS_RB].speed_pid->target = 0;	
		
		
		shoot_motor[SHOOT_RRUB].speed_pid->target = 0;
		shoot_motor[SHOOT_LRUB].speed_pid->target = 0;	
		shoot_motor[SHOOT_DIAL].speed_pid->target = 0;
		shoot_motor[SHOOT_GUN].speed_pid->target = 0;
		
		Chassis_send();	
		
		Shoot_Send();
		
	}				
}


void Judge_RC_Transtate(void)
{
	
	RC_this_state = rc_sensor.work_state;
	//如果工作状态突变
	if( RC_this_state != RC_last_state )
	{
	//这次成功通讯，表示开机
		if( RC_this_state == DEV_ONLINE )
			rc_sensor.tran_state = DEV_BOOT;

	//这次失联，表示关机
		else
			rc_sensor.tran_state = DEV_OFF;	
	}
	RC_last_state = RC_this_state;

}



