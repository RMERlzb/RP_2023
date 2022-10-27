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
				
		//һ��ʼͨѶ�ɹ�ʱ������̨�Լ��ص���е�Ƕ�2010λ��
		if(rc_sensor.tran_state == DEV_BOOT)
		{
			if( Mechanical_Return() )
			{
				rc_sensor.tran_state = DEV_OFF;	
				Mec_Return_flag = True;
			}											
		}	
		
		//�ɹ���ң����ͨѶ
		if( RC_ONLINE )
		{
			Cover_Awake();
			rc_sensor.check(&rc_sensor);	
			tick = 0;		
		}
		
		//ʧ��----��λ������ֵ��ж��
		else
		{
			
			Cover_Sleep();
			Offline_Reset_Var();
			Car_Unload();
		
		}
		
    osDelay(1);
  }
  
}


//ʧ�������ñ�����ֵ
void Offline_Reset_Var(void)
{
	
	Gimbal_Reset();
	
	rc_sensor.dial_reset(&rc_sensor);
	
	
	//ң�������ػ��жϵĸ���ȫ�ֱ���
	RC_last_state = RC_this_state = DEV_OFFLINE;
	
	//��ɻ�е��λ�ı�־
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
	
	//�����ȼ��ٵ�0������װ��Ҳ���ٵ�0
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
	//�������״̬ͻ��
	if( RC_this_state != RC_last_state )
	{
	//��γɹ�ͨѶ����ʾ����
		if( RC_this_state == DEV_ONLINE )
			rc_sensor.tran_state = DEV_BOOT;

	//���ʧ������ʾ�ػ�
		else
			rc_sensor.tran_state = DEV_OFF;	
	}
	RC_last_state = RC_this_state;

}



