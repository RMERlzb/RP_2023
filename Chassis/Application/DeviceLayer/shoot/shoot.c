#include "shoot.h"

void Shoot_Init(void)
{
	
	for (int i = 0; i < 4; i ++)
	{
		shoot_motor[i].info_init (&shoot_motor[i]);	
		pid_val_init (shoot_motor[i].speed_pid);
	}
	
	//�ٶ�PID----------------��Ϊ���ĸ������ʱ����
	for (int i = 0; i < 3; i ++)
	{	
		shoot_motor[i].speed_pid->out_max = 6000;	
		shoot_motor[i].speed_pid->kp = 10;
		shoot_motor[i].speed_pid->kd = 5;
	}
	//�������ٶȻ�
	shoot_motor[SHOOT_DIAL].speed_pid->target = -Shoot_Vmax;
	
	
	//�������ܽǶ�λ�û�
	pid_val_init (shoot_motor[SHOOT_DIAL].anglesum_pid);
	shoot_motor[SHOOT_DIAL].anglesum_pid->kp = 2;
	shoot_motor[SHOOT_DIAL].anglesum_pid->kd = 50;
	shoot_motor[SHOOT_DIAL].anglesum_pid->out_max = 6000;
	
	
}




void FricWheel_WorkState_Switch(void)
{

	if( rc_sensor_dial.FricWheel_WorkEnable == Flase ) 
	{	
		//����
		shoot_motor[SHOOT_LRUB].speed_pid->target = 0;
		
		shoot_motor[SHOOT_RRUB].speed_pid->target = 0;	
	}
	else
	{
		shoot_motor[SHOOT_LRUB].speed_pid->target = -Shoot_Vmax;		
		
		shoot_motor[SHOOT_RRUB].speed_pid->target = Shoot_Vmax;		
	}
		
}




//����״̬�£��ٶ�̫�͵ĳ������ںͶ�ת����ʧ�ܵĳ�������
char Speed_Block_Cnt = 0, Toggle_Block_Cnt1 = 0;

//����״̬��
char Single_Block_Cnt = 0, Toggle_Block_Cnt2 = 0;

void Block_Manage(void)
{	
	//�����������ö���
	rc_sensor_dial_t* dial = &rc_sensor_dial;
	
	//����һ�¶�ת����½Ƕ��ܺ���������ǰ�����νǶ�֮������
	float temp_err = 0;
	
	
	//------------------------------�������俪ʼ------------------------------------------//
	if( dial->Continue_ShootEnable == Flase )
		dial->ContinueShoot_Cnt = dial->ContinueShoot_Block_ToggleDir = 0;
		
	else
	{
		
		if( dial->ContinueShoot_Cnt == 0 )
			dial->ContinueShoot_Cnt = HAL_GetTick();
		
		if( HAL_GetTick() - dial->ContinueShoot_Cnt >= 1000 )
		{			
					
			dial->ContinueShoot_Cnt = 0;
			
	//----------------------------���ٳ���2s��ֻ�����һ�Σ�Ҫ��Ҫ���̣�--------------------------//
			if( MyAbs_Float(shoot_motor[SHOOT_DIAL].info->speed) <= 5 && \
					dial->Continue_Shoot_BlockFlag == Flase )
			{
				Speed_Block_Cnt ++;		
				
				if( Speed_Block_Cnt >= 3 )
				{
					Speed_Block_Cnt = 0;
					//�ر��ٶ�PID
					dial->Continue_Shoot_BlockFlag = True;			
				
					shoot_motor[SHOOT_DIAL].anglesum_pid->target += 40000;			
					
					shoot_motor[SHOOT_DIAL].anglesum_pid_update(&shoot_motor[SHOOT_DIAL]);
				}			
			}			
					
			
			
			
			//-------------------��������Ͳ����ת��������������ʱ------------------------//
			if( MyAbs_Float(shoot_motor[SHOOT_DIAL].info->speed) >= 200 && \
					dial->Continue_Shoot_BlockFlag == Flase )
				dial->ContinueShoot_Block_ToggleDir = Speed_Block_Cnt = 0;	
			
			
			
			
			//--------------------------��ɴ��ԵĶ�ת����(���Ŵ�)--------------------------//
			temp_err = shoot_motor[SHOOT_DIAL].anglesum_pid->err ;
			
			if( dial->Continue_Shoot_BlockFlag == True && MyAbs_Float(temp_err) <= 4000 )	
			{
				dial->Continue_Shoot_BlockFlag = Flase;
				dial->ContinueShoot_Block_ToggleDir = 0;
				Toggle_Block_Cnt1 = 0;
			}
			
			
			
			//-------------------------��ת������������(�����׳�ȥ)--------------------------//
			if( dial->Continue_Shoot_BlockFlag == True && MyAbs_Float(temp_err) > 4000 )
			{
				Toggle_Block_Cnt1 ++;
				
				//��ת���ֻ������������ӳ�һ��
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
	
	//------------------------------�����������-------------------------------//
	
	
	
	
	
	
	//-------------------------------������ʼ-----------------------------------//
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
			
			
		//------------------------------������סʱ��̫��-------------------------------//	
			if( MyAbs_Float(temp_err) >= 15000 && dial->Single_Shoot_BlockFlag == Flase )
			{	
				
				Single_Block_Cnt ++;
				
				//��Ϊ�Ѿ��ڽǶ��ܺ�Ŀ��ֵ����һ�Σ��ȼӻ���������ֻ���ڳ��ִ���������ת������̲���Ҫ�ӻ���
				if( Single_Block_Cnt >= 3 )
				{
					Single_Block_Cnt = 0;
					
					shoot_motor[SHOOT_DIAL].anglesum_pid->target += 45 * 820;
					
					shoot_motor[SHOOT_DIAL].anglesum_pid->target += 30000;	
					
					dial->Single_Shoot_BlockFlag = True;
					
				}
				
			}
			
			
		//----------------------------��������Ͳ����ת����������----------------------------//
			
			if( dial->Single_Shoot_BlockFlag == Flase && MyAbs_Float(temp_err) <= 500 )
				dial->SingleShoot_Block_ToggleDir = Single_Block_Cnt =0;
			
			
			
		//-----------------------------�������˴��ԵĶ�ת����------------------------------//
			if( dial->Single_Shoot_BlockFlag == True &&  MyAbs_Float(temp_err) <= 2000 )
			{	
				dial->Single_Shoot_BlockFlag = Flase;
				dial->SingleShoot_Block_ToggleDir = 0;
				Toggle_Block_Cnt2 = 0;
			}

			
		//--------------------------------��ת��������-------------------------------------//	
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
	
	//--------------------------------��������----------------------------------//
	
	
	
}

char flag = 0;


void Shoot(void)
{
	
	rc_sensor_dial_t* dial = &rc_sensor_dial;
	
	FricWheel_WorkState_Switch();
	
	flag = Shoot_Power_Judge();
	
	//�������ʵ�����²Ÿ���PID������
	if( Shoot_Power_Judge() == 1 )
	{
			//����
		if( dial->Single_ShootEnable == True && dial->Single_Shoot_BlockFlag == Flase )
		{		
					
			if( dial->Var_Change_Enable == True )
			{	
				//�����target˵����ת�ӵ�Ŀ��ֵ
				shoot_motor[SHOOT_DIAL].anglesum_pid->target += -45 * 820;
				
				dial->Var_Change_Enable = Flase;
			}
			
			shoot_motor[SHOOT_DIAL].anglesum_pid_update(&shoot_motor[SHOOT_DIAL]);
			
		}
		
		//������
		if( dial->Continue_ShootEnable == True && dial->Continue_Shoot_BlockFlag == Flase )
		{
			shoot_motor[SHOOT_DIAL].speed_pid->target = -Shoot_Vmax;
			shoot_motor[SHOOT_DIAL].speed_pid_update(&shoot_motor[SHOOT_DIAL]);
			
		}
			
	}
	
	//��������ʣ��ٶ�PID��Ŀ��ֵΪ0���Ƕ�PID����Ŀ��ֵ�������Բ��øĶ�
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
	
	
	//�ٶȻ����Ʋ��������������䲢��û�ж�ת������ʧ��
	if( ( dial->Continue_ShootEnable == True && dial->Continue_Shoot_BlockFlag == Flase ) \
		  || rc_sensor.work_state == DEV_OFFLINE )
	{
		
		single_speedpid_cal(&shoot_motor[SHOOT_DIAL]);
		shoot_txbuf[4] = (int16_t) shoot_motor[SHOOT_DIAL].speed_pid->out>>8;
		shoot_txbuf[5] = (int16_t) shoot_motor[SHOOT_DIAL].speed_pid->out;
		
	}
	
	
	//λ�û����Ʋ����������η�����߳����κζ�ת
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
