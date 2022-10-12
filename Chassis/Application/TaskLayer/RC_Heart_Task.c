#include "RC_Heart_Task.h"
uint32_t tick = 0;

dev_work_state_t last_state = DEV_OFFLINE, this_state = DEV_OFFLINE;

char Mec_Return_flag = 0;

//失联下重置变量的值
void Offline_Reset_Var(void)
{
	//改变机械模式下Y轴方向的电机外环目标值，而Z轴方向的电机外环目标值一直没有改变
	machinery_pid_Y.gimbal_pid_angle->target = mechanical_Y;
	//陀螺仪模式下的Y轴和Z轴的陀螺仪角度目标值不用变
	
	//遥控器开关机判断的辅助全局变量
	last_state = this_state = DEV_OFFLINE;
	
	//完成机械回位的标志
	Mec_Return_flag = 0;
	
	//把所有模式的标志值清零（机械模式，陀螺仪模式，小陀螺模式）
	Mechanical_Mode = Imu_Mode = Gryo_Mode = 0;
}

//车卸力----底盘和云台
void Car_Unload(void)
{
	if(tick == 0)
		tick = HAL_GetTick();
	
	//500ms后所有电机发送0卸力		
	if(HAL_GetTick() - tick >= 500)		
	{
		memset(chassis_txbuf, 0, sizeof(chassis_txbuf) );
		CAN_Send_Msg(&CAN1_TxMsg, Chassis_Control_ID, chassis_txbuf);		
				
		memset(gimbal_txbuf, 0, sizeof(gimbal_txbuf) );
		CAN_Send_Msg(&CAN1_TxMsg, Gimbal_Control_ID, gimbal_txbuf);		
	}	
	
	//底盘先减速到0
	else
	{
		(chassis_motor[CHAS_LF].pid)->target = 0;
		(chassis_motor[CHAS_RF].pid)->target = 0;
		(chassis_motor[CHAS_LB].pid)->target = 0;
		(chassis_motor[CHAS_RB].pid)->target = 0;	
		
		Chassis_send();				
	}				
}


void Judge_RC_Transtate(void)
{
	
	this_state = rc_sensor.work_state;
	//如果工作状态突变
	if( this_state != last_state )
	{
	//这次成功通讯，表示开机
		if(this_state == DEV_ONLINE)
			rc_sensor.tran_state = DEV_BOOT;

	//这次失联，表示关机
		else
			rc_sensor.tran_state = DEV_OFF;	
	}
	last_state = this_state;

}


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
				Mec_Return_flag = 1;
			}											
		}	
		
		//成功与遥控器通讯
		if(rc_sensor.work_state == DEV_ONLINE)
		{
			rc_sensor.check(&rc_sensor);	
			tick = 0;		
		}
		
		//失联----复位变量的值，卸力
		else
		{
			Offline_Reset_Var();
			Car_Unload();		
		}
		
    osDelay(1);
  }
  
}
