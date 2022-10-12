#include "RC_Heart_Task.h"
uint32_t tick = 0;

dev_work_state_t last_state = DEV_OFFLINE, this_state = DEV_OFFLINE;

char Mec_Return_flag = 0;

//ʧ�������ñ�����ֵ
void Offline_Reset_Var(void)
{
	//�ı��еģʽ��Y�᷽��ĵ���⻷Ŀ��ֵ����Z�᷽��ĵ���⻷Ŀ��ֵһֱû�иı�
	machinery_pid_Y.gimbal_pid_angle->target = mechanical_Y;
	//������ģʽ�µ�Y���Z��������ǽǶ�Ŀ��ֵ���ñ�
	
	//ң�������ػ��жϵĸ���ȫ�ֱ���
	last_state = this_state = DEV_OFFLINE;
	
	//��ɻ�е��λ�ı�־
	Mec_Return_flag = 0;
	
	//������ģʽ�ı�־ֵ���㣨��еģʽ��������ģʽ��С����ģʽ��
	Mechanical_Mode = Imu_Mode = Gryo_Mode = 0;
}

//��ж��----���̺���̨
void Car_Unload(void)
{
	if(tick == 0)
		tick = HAL_GetTick();
	
	//500ms�����е������0ж��		
	if(HAL_GetTick() - tick >= 500)		
	{
		memset(chassis_txbuf, 0, sizeof(chassis_txbuf) );
		CAN_Send_Msg(&CAN1_TxMsg, Chassis_Control_ID, chassis_txbuf);		
				
		memset(gimbal_txbuf, 0, sizeof(gimbal_txbuf) );
		CAN_Send_Msg(&CAN1_TxMsg, Gimbal_Control_ID, gimbal_txbuf);		
	}	
	
	//�����ȼ��ٵ�0
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
	//�������״̬ͻ��
	if( this_state != last_state )
	{
	//��γɹ�ͨѶ����ʾ����
		if(this_state == DEV_ONLINE)
			rc_sensor.tran_state = DEV_BOOT;

	//���ʧ������ʾ�ػ�
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
				
		//һ��ʼͨѶ�ɹ�ʱ������̨�Լ��ص���е�Ƕ�2010λ��
		if(rc_sensor.tran_state == DEV_BOOT)
		{
			if( Mechanical_Return() )
			{
				rc_sensor.tran_state = DEV_OFF;	
				Mec_Return_flag = 1;
			}											
		}	
		
		//�ɹ���ң����ͨѶ
		if(rc_sensor.work_state == DEV_ONLINE)
		{
			rc_sensor.check(&rc_sensor);	
			tick = 0;		
		}
		
		//ʧ��----��λ������ֵ��ж��
		else
		{
			Offline_Reset_Var();
			Car_Unload();		
		}
		
    osDelay(1);
  }
  
}
