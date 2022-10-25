#include "gimbal.h"  

pid_ctrl_t gimbal_imu_pid[2][2];//��һ�з������ǵĽǶȣ��ڶ��з������ǵĽ��ٶ�


//����ģʽ�£�
imu_pid_t imu_pid_Z;
imu_pid_t imu_pid_Y;


//��еģʽ�£�
machinery_pid_t  machinery_pid_Z;
machinery_pid_t  machinery_pid_Y;


//Z����̨���������
float chassis_input = 0;


//������ģʽ
void imu_pid_init(void);
void imu_pid_cal(void);
void Chassis_Input(void);

//��еģʽ
void machinery_pid_init(void);
void mec_pid_cal(void);

void Cover_State_Judge(void);


	
void Gimbal_Reset(void)
{
	
	machinery_pid_Y.gimbal_angle_pid->target = mechanical_Y;
	machinery_pid_Z.gimbal_angle_pid->target = mechanical_Z;

	
	chassis_input = 0;
}


void Gimbal_Init(void)
{
	Servo_Init();
	
	gimbal_self_init();
	
	imu_pid_init();
	
	machinery_pid_init();
	
}

void Gimbal_Send(void)
{
	
	CAN_Send_Msg (&CAN1_TxMsg, Gimbal_Control_ID, gimbal_txbuf );
	memset ( gimbal_txbuf, 0, sizeof(gimbal_txbuf) );

}

char Mechanical_Return(void)
{
	//Y��������
	gimbal_motor[GIMBAL_HIGH].angle_pid_update(&gimbal_motor[GIMBAL_HIGH]);	
	
	
	//Z��������	
	gimbal_motor[GIMBAL_LOW].angle_pid_update(&gimbal_motor[GIMBAL_LOW]);	
	
	Cover_Close();
	
	if(abs(machinery_pid_Y.gimbal_angle_pid->err) <= 50 && \
		abs(machinery_pid_Z.gimbal_angle_pid->err) <= 50 )
	{
		imu_pid_Z.imu_angle_pid->target = yaw;
		imu_pid_Y.imu_angle_pid->target = pitch;
		
		gimbal_motor[GIMBAL_LOW].dir = Dir_Header;
		
		return 1;
	}
		

	
	mec_pid_cal();

	Gimbal_Send();
	
	return 0;
	
}



void IMU_Mode(void)
{
	
	//��������ģʽ�л�����еģʽ����̨״̬���ֲ���
	machinery_pid_Y.gimbal_angle_pid->target = gimbal_motor[GIMBAL_HIGH].info->angle;
	
	//��ʱ���Ӵ���
	if( Judge_Cover_State() == Open )
		Cover_Close();
	
	Chassis_Input();
	
	imu_pid_cal();
	
		
}


void MEC_Mode(void)
{
	//�ӻ�еģʽ�л���������ģʽ�£���̨���Ǳ�����һʱ��״̬
	imu_pid_Z.imu_angle_pid->target = yaw;
	imu_pid_Y.imu_angle_pid->target = pitch;
	
	Cover_State_Judge();
	
	mec_pid_cal();

}

void gimbal_self_init(void)
{
	
	gimbal_motor[GIMBAL_LOW].info_init(&gimbal_motor[GIMBAL_LOW]);
	pid_val_init(gimbal_motor[GIMBAL_LOW].angle_pid);
	
	
	gimbal_motor[GIMBAL_HIGH].info_init(&gimbal_motor[GIMBAL_HIGH]);	
	pid_val_init(gimbal_motor[GIMBAL_HIGH].angle_pid);
	
}


void imu_pid_init(void)
{
	//����������̨�����������pid��(�����ǽǶ�+���ٶ�)
	imu_pid_Z.imu_angle_pid = &gimbal_imu_pid[0][0];
	imu_pid_Z.imu_speed_pid = &gimbal_imu_pid[0][1];
	
	
	imu_pid_Y.imu_angle_pid = &gimbal_imu_pid[1][0];
	imu_pid_Y.imu_speed_pid = &gimbal_imu_pid[1][1];
	
	//-----------------------������̨���������---------------------//
	
	
	//��	
	pid_val_init(imu_pid_Z.imu_angle_pid);
	imu_pid_Z.imu_angle_pid->kp = 210;
	imu_pid_Z.imu_angle_pid->out_max = 20000;
	imu_pid_Z.imu_angle_pid->target = 0;//---------------------������Z��Ƕ�Ŀ��ֵ
	
	
	//��	
	pid_val_init(imu_pid_Z.imu_speed_pid);
	imu_pid_Z.imu_speed_pid->kp = 80;
	imu_pid_Z.imu_speed_pid->ki = 1;
	imu_pid_Z.imu_speed_pid->out_max = 20000;
	imu_pid_Z.imu_speed_pid->integral_max = 10000;
	
	
	//----------------------������̨���������-----------------------//	
	
	
	//��
	pid_val_init(imu_pid_Y.imu_angle_pid);
	imu_pid_Y.imu_angle_pid->kp = 260;
	imu_pid_Y.imu_angle_pid->out_max = 20000;
	imu_pid_Y.imu_angle_pid->target = 0;//---------------------������Y��Ƕ�Ŀ��ֵ
	
	//��
	pid_val_init(imu_pid_Y.imu_speed_pid);
	imu_pid_Y.imu_speed_pid->kp = 20;
	imu_pid_Y.imu_speed_pid->ki = 1;
	imu_pid_Y.imu_speed_pid->out_max = 20000;
	imu_pid_Y.imu_speed_pid->integral_max = 2000;
	
	
	
}

void imu_pid_cal(void)
{


	
//---------------------------Ŀ��ֵ�ĸ��ĵط�-------------------------//	
	//��������Ӿ�����
	if( rc_sensor_dial.Version_Control_Enable == True )
	{			
		
		imu_pid_Z.imu_angle_pid->target = vision_sensor_info.RxPacket.RxData.yaw_angle / 22.7555f - 180;
		
		
		imu_pid_Y.imu_angle_pid->target = vision_sensor_info.RxPacket.RxData.pitch_angle / 22.7555f - 180;
				
	}
	
	//�������Ӿ��Ͳ�������ң����������̨
	else
	{
		//������ģʽ�½Ƕ������仯�ʣ�ע���Ǹ���
		imu_pid_Z.imu_angle_pid->target += (rc_sensor.info)->ch0 * -0.0005f;
			
		imu_pid_Y.imu_angle_pid->target += (rc_sensor.info)->ch1 * -0.0008f ;
	
		
	}
	
	
//--------------------------------------Z���--------------------------------//


//----------------------------------����------------------------------//
	
//-------------�⻷	
	imu_pid_Z.imu_angle_pid->measure = yaw;
	MyRampFloat (&imu_pid_Z.imu_angle_pid->target, yaw, 180, &imu_pid_Z.imu_angle_pid->err);
	single_pid_ctrl ( imu_pid_Z.imu_angle_pid, imu_pid_Z.imu_angle_pid->err );
	
	
//-------------�ڻ�	
	imu_pid_Z.imu_speed_pid->target =  imu_pid_Z.imu_angle_pid->out;
	imu_pid_Z.imu_speed_pid->measure = ggz;
	imu_pid_Z.imu_speed_pid->err = imu_pid_Z.imu_speed_pid->target - imu_pid_Z.imu_speed_pid->measure;
	single_pid_ctrl ( imu_pid_Z.imu_speed_pid, imu_pid_Z.imu_speed_pid->err );
	
	
//-------------------------------�������--------------------------//	
	gimbal_txbuf[0] = (int16_t) imu_pid_Z.imu_speed_pid->out>>8;
	gimbal_txbuf[1] = (int16_t) imu_pid_Z.imu_speed_pid->out;
	
	
	
	
	
	
//--------------------------------------Y���--------------------------------//	



//----------------------------------����------------------------------//
//-------------�⻷	
	ConstrainFloat( &imu_pid_Y.imu_angle_pid->target, gimbal_pitchmax, gimbal_pitchmin);
	imu_pid_Y.imu_angle_pid->measure = pitch;
	imu_pid_Y.imu_angle_pid->err = imu_pid_Y.imu_angle_pid->target - imu_pid_Y.imu_angle_pid->measure;
	single_pid_ctrl ( imu_pid_Y.imu_angle_pid, imu_pid_Y.imu_angle_pid->err );


//-------------�ڻ�	
	imu_pid_Y.imu_speed_pid->target =  imu_pid_Y.imu_angle_pid->out;
	imu_pid_Y.imu_speed_pid->measure = ggy;
	imu_pid_Y.imu_speed_pid->err = imu_pid_Y.imu_speed_pid->target - ggy ;
	single_pid_ctrl ( imu_pid_Y.imu_speed_pid, imu_pid_Y.imu_speed_pid->err );
	

//-------------------------------�������--------------------------//		
	gimbal_txbuf[2] = (int16_t) imu_pid_Y.imu_speed_pid->out>>8;
	gimbal_txbuf[3] = (int16_t) imu_pid_Y.imu_speed_pid->out;
	

}




void Chassis_Input(void)
{
	
	float kp_temp = 0, kd_temp = 0,out_max_temp = 0;
	
	chassis_input = 0;
	
	//�����еģʽ��Z��Ļ컷PID���⻷��Ϣ����ʵ����Z��������Ϣ����Ŀ��ֵ�޸�
	kp_temp = machinery_pid_Z.gimbal_angle_pid->kp;
	kd_temp = machinery_pid_Z.gimbal_angle_pid->kd;
	out_max_temp = machinery_pid_Z.gimbal_angle_pid->out_max;
	
	
	
	//���µ�ֵ
	machinery_pid_Z.gimbal_angle_pid->kp = 0.5;
	machinery_pid_Z.gimbal_angle_pid->kd = 3;
	machinery_pid_Z.gimbal_angle_pid->out_max = 1000;		
	
	
	//---------------------------------ͷβ�����ж�Ŀ��ֵ-----------------------------//
	if( rc_sensor_dial.Imu_Mode == True )
	{
		//˵��ȡ����С����
		if( rc_sensor_dial.Gryo_Mode == Flase )
		{
			//��ͷ
			if( gimbal_motor[GIMBAL_LOW].info->angle > 0 && gimbal_motor[GIMBAL_LOW].info->angle <= 4050 )
			{
				
				machinery_pid_Z.gimbal_angle_pid->target = mechanical_Z;
				
				gimbal_motor[GIMBAL_LOW].dir = Dir_Header;
				
			}
				
			
			//��β
			else if( gimbal_motor[GIMBAL_LOW].info->angle > 4050 && gimbal_motor[GIMBAL_LOW].info->angle <= 8192 )
			{
				
				machinery_pid_Z.gimbal_angle_pid->target = tail_mechanical;
				
				gimbal_motor[GIMBAL_LOW].dir = Dir_Tail;
				
			}
				
			
		}
		
		//��ת�ٶȸ�ֵ
		single_anglepid_cal(&gimbal_motor[GIMBAL_LOW]);
		
		chassis_input = gimbal_motor[GIMBAL_LOW].angle_pid->out;
		
	}
	

	//��ػ�еģʽZ��PID�⻷����Ϣ
	machinery_pid_Z.gimbal_angle_pid->kp = kp_temp;
	machinery_pid_Z.gimbal_angle_pid->kd = kd_temp;
	machinery_pid_Z.gimbal_angle_pid->out_max = out_max_temp;
	
	
}





void machinery_pid_init(void)
{
	
	//����������̨����Ļ컷PID(�⻷����Ƕ�+�ڻ������ǽ��ٶ�)
	machinery_pid_Z.gimbal_angle_pid = gimbal_motor[GIMBAL_LOW].angle_pid;
	machinery_pid_Z.imu_speed_pid = &gimbal_imu_pid[0][1];
		
	
	machinery_pid_Y.gimbal_angle_pid = gimbal_motor[GIMBAL_HIGH].angle_pid;
	machinery_pid_Y.imu_speed_pid = &gimbal_imu_pid[1][1];
	
	
	
	//-----------------------������̨���������---------------------//
	

	//��	
	pid_val_init(machinery_pid_Z.gimbal_angle_pid);
	machinery_pid_Z.gimbal_angle_pid->kp = 10;
	machinery_pid_Z.gimbal_angle_pid->out_max = 20000;
	machinery_pid_Z.gimbal_angle_pid->target = mechanical_Z;//-----------Z���е�Ƕ�Ŀ��ֵһ��ʼ��
		
	//��	
	pid_val_init(machinery_pid_Z.imu_speed_pid);
	machinery_pid_Z.imu_speed_pid->kp = 50;
	machinery_pid_Z.imu_speed_pid->ki = 1;
	machinery_pid_Z.imu_speed_pid->out_max = 20000;
	machinery_pid_Z.imu_speed_pid->integral_max = 10000;
	
	
	
	//----------------------������̨���������-----------------------//	
	
	//��	
	pid_val_init(machinery_pid_Y.gimbal_angle_pid);
	machinery_pid_Y.gimbal_angle_pid->kp = 10;
	machinery_pid_Y.gimbal_angle_pid->out_max = 20000;
	machinery_pid_Y.gimbal_angle_pid->target = mechanical_Y;//-----------Y���е�Ƕ�Ŀ��ֵһ��ʼ��
	
	
	//��
	pid_val_init(machinery_pid_Y.imu_speed_pid);
	machinery_pid_Y.imu_speed_pid->kp = 10;
	machinery_pid_Y.imu_speed_pid->ki = 1;
	machinery_pid_Y.imu_speed_pid->out_max = 20000;
	machinery_pid_Y.imu_speed_pid->integral_max = 10000;
	
	
}

void mec_pid_cal(void)
{

//----------------------------Y��---------------------------//
	
	//----�⻷Ŀ��ֵ---//	
	machinery_pid_Y.gimbal_angle_pid->target += (rc_sensor.info)->ch1 * -0.005f;		
	ConstrainFloat( &machinery_pid_Y.gimbal_angle_pid->target, gimbal_angle_Ymax, gimbal_angle_Ymin);	

	
	//----�⻷����ֵ�����----//	
	gimbal_motor[GIMBAL_HIGH].angle_pid_update(&gimbal_motor[GIMBAL_HIGH]);

	
	//----�⻷�ļ���-------//
	single_pid_ctrl ( machinery_pid_Y.gimbal_angle_pid, machinery_pid_Y.gimbal_angle_pid->err );
	
	
	
	//-----�ڻ�Ŀ��ֵ----//
	machinery_pid_Y.imu_speed_pid->target = machinery_pid_Y.gimbal_angle_pid->out;
	
	
	//-----�ڻ�����ֵ�����----//
	machinery_pid_Y.imu_speed_pid->measure = ggy * cos( pitch / Radian_Rate) ;
	machinery_pid_Y.imu_speed_pid->err = machinery_pid_Y.imu_speed_pid->target - machinery_pid_Y.imu_speed_pid->measure;
	
	
	//-----�ڻ��ļ���----//
	single_pid_ctrl ( machinery_pid_Y.imu_speed_pid, machinery_pid_Y.imu_speed_pid->err );


//----------------------�������---------------------------//	
	gimbal_txbuf[2] = (int16_t) machinery_pid_Y.imu_speed_pid->out>>8;
	gimbal_txbuf[3] = (int16_t) machinery_pid_Y.imu_speed_pid->out;
	

		
	
//----------------------------Z��-----------------------------//

	//----�⻷����ֵ�����-----//
	gimbal_motor[GIMBAL_LOW].angle_pid_update(&gimbal_motor[GIMBAL_LOW]);
	
	
	//----�⻷�ļ���----//
	single_pid_ctrl ( machinery_pid_Z.gimbal_angle_pid, machinery_pid_Z.gimbal_angle_pid->err );
		
		
	//-----�ڻ�Ŀ��ֵ----//
	machinery_pid_Z.imu_speed_pid->target = machinery_pid_Z.gimbal_angle_pid->out;
	
	
	//-----�ڻ�����ֵ�����----//
	machinery_pid_Z.imu_speed_pid->measure = ggz ;
	machinery_pid_Z.imu_speed_pid->err = machinery_pid_Z.imu_speed_pid->target - machinery_pid_Z.imu_speed_pid->measure;
	
	
	//----�ڻ��ļ���-----//
	single_pid_ctrl ( machinery_pid_Z.imu_speed_pid, machinery_pid_Z.imu_speed_pid->err );

	
	//-----�������-----//
	gimbal_txbuf[0] = (int16_t) machinery_pid_Z.imu_speed_pid->out>>8;
	gimbal_txbuf[1] = (int16_t) machinery_pid_Z.imu_speed_pid->out;
	

}

void Cover_State_Judge(void)
{
	
	if( rc_sensor_dial.Cover_OpenEnable == True )
		Cover_Open();
	
	else
		Cover_Close();
}


