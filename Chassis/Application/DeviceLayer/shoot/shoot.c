#include "shoot.h"

void Shoot_Init(void)
{
	
	for (int i = 0; i < 4; i ++)
	{
		shoot_motor[i].info_init (&shoot_motor[i]);	
		pid_val_init (shoot_motor[i].speed_pid);
	}
	
	//速度PID----------------因为第四个电机暂时不用
	for (int i = 0; i < 3; i ++)
	{	
		shoot_motor[i].speed_pid->out_max = 6000;	
		shoot_motor[i].speed_pid->kp = 10;
		shoot_motor[i].speed_pid->kd = 5;
	}
	//拨码电机速度环
	shoot_motor[SHOOT_DIAL].speed_pid->target = -Shoot_Vmax;
	
	
	//拨码电机总角度位置环
	pid_val_init (shoot_motor[SHOOT_DIAL].anglesum_pid);
	shoot_motor[SHOOT_DIAL].anglesum_pid->kp = 2;
	shoot_motor[SHOOT_DIAL].anglesum_pid->kd = 50;
	shoot_motor[SHOOT_DIAL].anglesum_pid->out_max = 6000;
	
	
}




void FricWheel_WorkState_Switch(void)
{

	if( rc_sensor_dial.FricWheel_WorkEnable == Flase ) 
	{	
		//减速
		shoot_motor[SHOOT_LRUB].speed_pid->target = 0;
		
		shoot_motor[SHOOT_RRUB].speed_pid->target = 0;	
	}
	else
	{
		shoot_motor[SHOOT_LRUB].speed_pid->target = -Shoot_Vmax;		
		
		shoot_motor[SHOOT_RRUB].speed_pid->target = Shoot_Vmax;		
	}
		
}




//连发状态下，速度太低的持续周期和堵转工作失败的持续周期
char Speed_Block_Cnt = 0, Toggle_Block_Cnt1 = 0;

//单发状态下
char Single_Block_Cnt = 0, Toggle_Block_Cnt2 = 0;

void Block_Manage(void)
{	
	//方便下面引用而已
	rc_sensor_dial_t* dial = &rc_sensor_dial;
	
	//保存一下堵转情况下角度总和误差或者是前后两次角度之间的误差
	float temp_err = 0;
	
	
	//------------------------------连续发射开始------------------------------------------//
	if( dial->Continue_ShootEnable == Flase )
		dial->ContinueShoot_Cnt = dial->ContinueShoot_Block_ToggleDir = 0;
		
	else
	{
		
		if( dial->ContinueShoot_Cnt == 0 )
			dial->ContinueShoot_Cnt = HAL_GetTick();
		
		if( HAL_GetTick() - dial->ContinueShoot_Cnt >= 1000 )
		{			
					
			dial->ContinueShoot_Cnt = 0;
			
	//----------------------------低速持续2s，只会进入一次（要求要苛刻）--------------------------//
			if( MyAbs_Float(shoot_motor[SHOOT_DIAL].info->speed) <= 5 && \
					dial->Continue_Shoot_BlockFlag == Flase )
			{
				Speed_Block_Cnt ++;		
				
				if( Speed_Block_Cnt >= 3 )
				{
					Speed_Block_Cnt = 0;
					//关闭速度PID
					dial->Continue_Shoot_BlockFlag = True;			
				
					shoot_motor[SHOOT_DIAL].anglesum_pid->target += 40000;			
					
					shoot_motor[SHOOT_DIAL].anglesum_pid_update(&shoot_motor[SHOOT_DIAL]);
				}			
			}			
					
			
			
			
			//-------------------如果本来就不会堵转，连续发射正常时------------------------//
			if( MyAbs_Float(shoot_motor[SHOOT_DIAL].info->speed) >= 200 && \
					dial->Continue_Shoot_BlockFlag == Flase )
				dial->ContinueShoot_Block_ToggleDir = Speed_Block_Cnt = 0;	
			
			
			
			
			//--------------------------完成粗略的堵转工作(误差放大)--------------------------//
			temp_err = shoot_motor[SHOOT_DIAL].anglesum_pid->err ;
			
			if( dial->Continue_Shoot_BlockFlag == True && MyAbs_Float(temp_err) <= 4000 )	
			{
				dial->Continue_Shoot_BlockFlag = Flase;
				dial->ContinueShoot_Block_ToggleDir = 0;
				Toggle_Block_Cnt1 = 0;
			}
			
			
			
			//-------------------------堵转工作出现问题(更容易出去)--------------------------//
			if( dial->Continue_Shoot_BlockFlag == True && MyAbs_Float(temp_err) > 4000 )
			{
				Toggle_Block_Cnt1 ++;
				
				//堵转出现会进入这里，所以延迟一次
				if( Toggle_Block_Cnt1 >= 2 )
				{
					Toggle_Block_Cnt1 = 0;
					
					dial->ContinueShoot_Block_ToggleDir = !dial->ContinueShoot_Block_ToggleDir;
				
					if( !dial->ContinueShoot_Block_ToggleDir )
						shoot_motor[SHOOT_DIAL].anglesum_pid->target += 40000;			
					else
						shoot_motor[SHOOT_DIAL].anglesum_pid->target -= 40000;
				}
				
			}
			
			
		
		
				
			
		}		
			
	}
	
	//------------------------------连续发射结束-------------------------------//
	
	
	
	
	
	
	//-------------------------------单发开始-----------------------------------//
	if( dial->Single_ShootEnable == 0 )
		dial->SingleShoot_Cnt = dial->SingleShoot_Block_ToggleDir = 0;
	
	else
	{
		
		if( dial->SingleShoot_Cnt == 0 )	
			dial->SingleShoot_Cnt = HAL_GetTick();	
		
		if( HAL_GetTick() - dial->SingleShoot_Cnt >= 1000 )
		{								
			
			dial->SingleShoot_Cnt = 0;		
			
			temp_err = shoot_motor[SHOOT_DIAL].anglesum_pid->err;
			
			
		//------------------------------单发卡住时间太长-------------------------------//	
			if( MyAbs_Float(temp_err) >= 15000 && dial->Single_Shoot_BlockFlag == Flase )
			{	
				
				Single_Block_Cnt ++;
				
				//因为已经在角度总和目标值减了一次，先加回来，并且只能在出现从正常到堵转这个过程才需要加回来
				if( Single_Block_Cnt >= 3 )
				{
					Single_Block_Cnt = 0;
					
					shoot_motor[SHOOT_DIAL].anglesum_pid->target += 45 * 820;
					
					shoot_motor[SHOOT_DIAL].anglesum_pid->target += 30000;	
					
					dial->Single_Shoot_BlockFlag = True;
					
				}
				
			}
			
			
		//----------------------------如果本来就不会堵转，单发正常----------------------------//
			
			if( dial->Single_Shoot_BlockFlag == Flase && MyAbs_Float(temp_err) <= 500 )
				dial->SingleShoot_Block_ToggleDir = Single_Block_Cnt =0;
			
			
			
		//-----------------------------如果完成了粗略的堵转处理------------------------------//
			if( dial->Single_Shoot_BlockFlag == True &&  MyAbs_Float(temp_err) <= 2000 )
			{	
				dial->Single_Shoot_BlockFlag = Flase;
				dial->SingleShoot_Block_ToggleDir = 0;
				Toggle_Block_Cnt2 = 0;
			}

			
		//--------------------------------堵转发生问题-------------------------------------//	
			if( dial->Single_Shoot_BlockFlag == True &&  MyAbs_Float(temp_err) > 2000 )
			{
				Toggle_Block_Cnt2 ++;
				
				if( Toggle_Block_Cnt2 >= 2 )
				{
					Toggle_Block_Cnt2 = 0;
					
					dial->SingleShoot_Block_ToggleDir = !dial->SingleShoot_Block_ToggleDir;
					
					if( !dial->SingleShoot_Block_ToggleDir )
						shoot_motor[SHOOT_DIAL].anglesum_pid->target += 30000;	
			
					else
						shoot_motor[SHOOT_DIAL].anglesum_pid->target -= 30000;		
				}
				
			}
												
		}
		
	}
	
	//--------------------------------单发结束----------------------------------//
	
	
	
}

char flag = 0;


void Shoot(void)
{
	
	rc_sensor_dial_t* dial = &rc_sensor_dial;
	
	FricWheel_WorkState_Switch();
	
	flag = Shoot_Power_Judge();
	
	//不超功率的情况下才更新PID的数据
	if( Shoot_Power_Judge() == 1 )
	{
			//单发
		if( dial->Single_ShootEnable == True && dial->Single_Shoot_BlockFlag == Flase )
		{		
					
			if( dial->Var_Change_Enable == True )
			{	
				//这里的target说的是转子的目标值
				shoot_motor[SHOOT_DIAL].anglesum_pid->target += -45 * 820;
				
				dial->Var_Change_Enable = Flase;
			}
			
			shoot_motor[SHOOT_DIAL].anglesum_pid_update(&shoot_motor[SHOOT_DIAL]);
			
		}
		
		//连续发
		if( dial->Continue_ShootEnable == True && dial->Continue_Shoot_BlockFlag == Flase )
		{
			shoot_motor[SHOOT_DIAL].speed_pid->target = -Shoot_Vmax;
			shoot_motor[SHOOT_DIAL].speed_pid_update(&shoot_motor[SHOOT_DIAL]);
			
		}
			
	}
	
	//如果超功率，速度PID的目标值为0，角度PID由于目标值不变所以不用改动
	else
	{
		shoot_motor[SHOOT_DIAL].speed_pid->target = 0;
	}
	
}	


void Shoot_Txbuf_FillIn(void)
{
	
	rc_sensor_dial_t* dial = &rc_sensor_dial;
	
	single_speedpid_cal(&shoot_motor[SHOOT_LRUB]);
	shoot_txbuf[0] = (int16_t) shoot_motor[SHOOT_LRUB].speed_pid->out>>8;
	shoot_txbuf[1] = (int16_t) shoot_motor[SHOOT_LRUB].speed_pid->out;

	single_speedpid_cal(&shoot_motor[SHOOT_RRUB]);
	shoot_txbuf[2] = (int16_t) shoot_motor[SHOOT_RRUB].speed_pid->out>>8;
	shoot_txbuf[3] = (int16_t) shoot_motor[SHOOT_RRUB].speed_pid->out;
	
	
	//速度环控制拨码电机，连续发射并且没有堵转或者是失联
	if( ( dial->Continue_ShootEnable == True && dial->Continue_Shoot_BlockFlag == Flase ) \
		  || rc_sensor.work_state == DEV_OFFLINE )
	{
		
		single_speedpid_cal(&shoot_motor[SHOOT_DIAL]);
		shoot_txbuf[4] = (int16_t) shoot_motor[SHOOT_DIAL].speed_pid->out>>8;
		shoot_txbuf[5] = (int16_t) shoot_motor[SHOOT_DIAL].speed_pid->out;
		
	}
	
	
	//位置环控制拨码电机，单次发射或者出现任何堵转
	if(	dial->Single_ShootEnable == True || dial->Continue_Shoot_BlockFlag == True \
		  || dial->Single_Shoot_BlockFlag == True )
	{
		
		shoot_motor[SHOOT_DIAL].speed_pid_update(&shoot_motor[SHOOT_DIAL]);		
		single_anglesum_pid_cal(&shoot_motor[SHOOT_DIAL]);
		shoot_txbuf[4] = (int16_t) shoot_motor[SHOOT_DIAL].anglesum_pid->out>>8;
		shoot_txbuf[5] = (int16_t) shoot_motor[SHOOT_DIAL].anglesum_pid->out;	
		
	}	
	
	
}


void Shoot_Send(void)
{
	
	Shoot_Txbuf_FillIn();
	
	CAN_Send_Msg (&CAN2_TxMsg, Shoot_Control_ID, shoot_txbuf);
	
	memset (shoot_txbuf, 0, sizeof(shoot_txbuf) );
}
