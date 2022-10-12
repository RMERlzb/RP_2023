#include "gimbal.h"  

//6020����������ֵ����ʱ��ת������������ֵ��˳ʱ��ת
//��������yawֵ+     ����yawֵ��  

//����ģʽ�£�
pid_ctrl_t gimbal_imu_pid[2][2];
//������̨����������Ǵ���PID
imu_pid_t imu_pid_Z;
//������̨����������Ǵ���PID
imu_pid_t imu_pid_Y;


//��еģʽ�£�
pid_ctrl_t gimbal_machinery_pid[2][2];
//������̨������⻷���λ���ڻ������ǽ��ٶȻ컷PID
machinery_pid_t  machinery_pid_Z;
//������̨������⻷���λ���ڻ������ǽ��ٶȻ컷PID
machinery_pid_t  machinery_pid_Y;


void imu_pid_init(void)
{
	//����������̨�����������pid�����ǶȻ������ٶȻ���
	imu_pid_Z.imu_pid_angle = &gimbal_imu_pid[0][0];
	imu_pid_Z.imu_pid_velocity = &gimbal_imu_pid[0][1];
	imu_pid_Y.imu_pid_angle = &gimbal_imu_pid[1][0];
	imu_pid_Y.imu_pid_velocity = &gimbal_imu_pid[1][1];
	
	//������̨���������	
	//��	
	pid_val_init(imu_pid_Z.imu_pid_angle);
	imu_pid_Z.imu_pid_angle->kp = 210;
	imu_pid_Z.imu_pid_angle->out_max = 20000;
	imu_pid_Z.imu_pid_angle->target = 0;//---------------------������Z��Ƕ�Ŀ��ֵ
	//��	
	pid_val_init(imu_pid_Z.imu_pid_velocity);
	imu_pid_Z.imu_pid_velocity->kp = 80;
	imu_pid_Z.imu_pid_velocity->ki = 1;
	imu_pid_Z.imu_pid_velocity->out_max = 20000;
	imu_pid_Z.imu_pid_velocity->integral_max = 10000;
	
	//������̨���������
	pid_val_init(imu_pid_Y.imu_pid_angle);
	//��
	imu_pid_Y.imu_pid_angle->kp = 260;
	imu_pid_Y.imu_pid_angle->out_max = 20000;
	imu_pid_Y.imu_pid_angle->target = 0;//---------------------������Y��Ƕ�Ŀ��ֵ
	//��
	pid_val_init(imu_pid_Y.imu_pid_velocity);
	imu_pid_Y.imu_pid_velocity->kp = 20;
	imu_pid_Y.imu_pid_velocity->ki = 1;
	imu_pid_Y.imu_pid_velocity->out_max = 20000;
	imu_pid_Y.imu_pid_velocity->integral_max = 2000;
}



void machinery_pid_init(void)
{
	//����������̨�����������pid�����ǶȻ������ٶȻ���
	machinery_pid_Z.gimbal_pid_angle = &gimbal_machinery_pid[0][0];
	machinery_pid_Z.imu_pid_velocity = &gimbal_machinery_pid[0][1];
	machinery_pid_Y.gimbal_pid_angle = &gimbal_machinery_pid[1][0];
	machinery_pid_Y.imu_pid_velocity = &gimbal_machinery_pid[1][1];
	
	
	//������̨���������	
	//��	
	pid_val_init(machinery_pid_Z.gimbal_pid_angle);
	machinery_pid_Z.gimbal_pid_angle->kp = 10;
	machinery_pid_Z.gimbal_pid_angle->out_max = 20000;
	machinery_pid_Z.gimbal_pid_angle->target = mechanical_Z;//----------------Z���е�Ƕ�Ŀ��ֵ
	
	//��	
	pid_val_init(machinery_pid_Z.imu_pid_velocity);
	machinery_pid_Z.imu_pid_velocity->kp = 50;
	machinery_pid_Z.imu_pid_velocity->ki = 1;
	machinery_pid_Z.imu_pid_velocity->out_max = 20000;
	machinery_pid_Z.imu_pid_velocity->integral_max = 10000;
	
	
	//������̨���������
	pid_val_init(machinery_pid_Y.gimbal_pid_angle);
	//��
	machinery_pid_Y.gimbal_pid_angle->kp = 10;
	machinery_pid_Y.gimbal_pid_angle->out_max = 20000;
	machinery_pid_Y.gimbal_pid_angle->target = mechanical_Y;//----------------Y���е�Ƕ�Ŀ��ֵ
	//��
	pid_val_init(machinery_pid_Y.imu_pid_velocity);
	machinery_pid_Y.imu_pid_velocity->kp = 10;
	machinery_pid_Y.imu_pid_velocity->ki = 1;
	machinery_pid_Y.imu_pid_velocity->out_max = 20000;
	machinery_pid_Y.imu_pid_velocity->integral_max = 10000;
}



void Gimbal_Init(void)
{
	gimbal_motor[GIMBAL_LOW].init(&gimbal_motor[GIMBAL_LOW]);	
	pid_val_init(gimbal_motor[GIMBAL_LOW].pid);
	gimbal_motor[GIMBAL_LOW].pid->out_max = 30000;
	//����ģʽ����̨�����PID�������̵����P����
	gimbal_motor[GIMBAL_LOW].pid->kp = 0.5;
	gimbal_motor[GIMBAL_LOW].pid->kd = 2.5;
	
	gimbal_motor[GIMBAL_HIGH].init(&gimbal_motor[GIMBAL_HIGH]);	
	pid_val_init(gimbal_motor[GIMBAL_HIGH].pid);
	gimbal_motor[GIMBAL_HIGH].pid->out_max = 30000;
}

void imu_pid_cal(void)
{
	//yaw�ᴮ��PID���
	single_pid_ctrl (imu_pid_Z.imu_pid_angle, imu_pid_Z.imu_pid_angle->err );
	single_pid_ctrl (imu_pid_Z.imu_pid_velocity, \
									imu_pid_Z.imu_pid_angle->out - ggz * cos( pitch / Radian_Rate) );
	
	gimbal_txbuf[0] = (int16_t) imu_pid_Z.imu_pid_velocity->out>>8;
	gimbal_txbuf[1] = (int16_t) imu_pid_Z.imu_pid_velocity->out;
	
	//pitch�ᴮ��PID���
	single_pid_ctrl (imu_pid_Y.imu_pid_angle, imu_pid_Y.imu_pid_angle->err );
	single_pid_ctrl (imu_pid_Y.imu_pid_velocity, imu_pid_Y.imu_pid_angle->out - ggy);
	
	gimbal_txbuf[2] = (int16_t) imu_pid_Y.imu_pid_velocity->out>>8;
	gimbal_txbuf[3] = (int16_t) imu_pid_Y.imu_pid_velocity->out;

}

void mec_pid_cal(void)
{
	//yaw�ᴮ��PID���
	single_pid_ctrl (machinery_pid_Z.gimbal_pid_angle, machinery_pid_Z.gimbal_pid_angle->err );
	single_pid_ctrl (machinery_pid_Z.imu_pid_velocity, \
									machinery_pid_Z.gimbal_pid_angle->out - ggz * cos( pitch / Radian_Rate));	
	gimbal_txbuf[0] = (int16_t) machinery_pid_Z.imu_pid_velocity->out>>8;
	gimbal_txbuf[1] = (int16_t) machinery_pid_Z.imu_pid_velocity->out;

	//pitch�ᴮ��PID���
	single_pid_ctrl (machinery_pid_Y.gimbal_pid_angle, machinery_pid_Y.gimbal_pid_angle->err );
	single_pid_ctrl (machinery_pid_Y.imu_pid_velocity, machinery_pid_Y.gimbal_pid_angle->out - ggy);
	
	gimbal_txbuf[2] = (int16_t) machinery_pid_Y.imu_pid_velocity->out>>8;
	gimbal_txbuf[3] = (int16_t) machinery_pid_Y.imu_pid_velocity->out;

}

void Gimbal_Send(void)
{
	CAN_Send_Msg (&CAN1_TxMsg, Gimbal_Control_ID, gimbal_txbuf);
	memset (gimbal_txbuf, 0, sizeof(gimbal_txbuf));

}
char Mechanical_Return(void)
{
	//Y��������
	machinery_pid_Y.gimbal_pid_angle->err = mechanical_Y - gimbal_motor[GIMBAL_HIGH].info->angle; 
	
	//Z��������	
	RampFloat (mechanical_Z, gimbal_motor[GIMBAL_LOW].info->angle, \
						4096, &machinery_pid_Z.gimbal_pid_angle->err);
	
	if(abs(machinery_pid_Y.gimbal_pid_angle->err) <= 50 && abs(machinery_pid_Z.gimbal_pid_angle->err) <=50 )
		return 1;

	mec_pid_cal();

	Gimbal_Send();
	
	return 0;
}



void IMU_Mode(void)
{
	
	//��������ģʽ�л�����еģʽ����̨״̬���ֲ���
	machinery_pid_Y.gimbal_pid_angle->target = gimbal_motor[GIMBAL_HIGH].info->angle;
	
	//��������Ƕ�Ŀ��ֵ
	imu_pid_Z.imu_pid_angle->target += (rc_sensor.info)->ch0 * imu_angle_target_change_rate;
	imu_pid_Y.imu_pid_angle->target += (rc_sensor.info)->ch1 * imu_angle_target_change_rate * 0.8f;
	
	//pitch�Ƕ��޷���������
	ConstrainFloat (&imu_pid_Y.imu_pid_angle->target, gimbal_pitchmax, gimbal_pitchmin);
	imu_pid_Y.imu_pid_angle->err = imu_pid_Y.imu_pid_angle->target - pitch;
	
	//yaw�Ƕ���Ŀ��ֵ��Ϊͬ����Χ�����Ҵ�������������
	MyRampFloat (&imu_pid_Z.imu_pid_angle->target, yaw, 180, &imu_pid_Z.imu_pid_angle->err);

	imu_pid_cal();
	
	RampFloat (mechanical_Z, gimbal_motor[GIMBAL_LOW].info->angle, \
						gimbal_angle_max / 2, &gimbal_motor[GIMBAL_LOW].pid->err);
	
	//��ת�ٶȸ�ֵ
	single_pid_ctrl (gimbal_motor[GIMBAL_LOW].pid, gimbal_motor[GIMBAL_LOW].pid->err);
		
}



void MEC_Mode(void)
{
	//�ӻ�еģʽ�л���������ģʽ�£���̨���Ǳ�����һʱ��״̬
	imu_pid_Z.imu_pid_angle->target = yaw;
	imu_pid_Y.imu_pid_angle->target = pitch;
	
	//��������Ŀ��ֵ�ĸ�ֵ���޷�
	machinery_pid_Y.gimbal_pid_angle->target += (rc_sensor.info)->ch1 * mec_angle_target_change_rate;
	
	//Y��Ŀ��ֵ�޷���������
	ConstrainFloat( &machinery_pid_Y.gimbal_pid_angle->target, gimbal_angle_Ymax, gimbal_angle_Ymin);	
	machinery_pid_Y.gimbal_pid_angle->err = machinery_pid_Y.gimbal_pid_angle->target - gimbal_motor[GIMBAL_HIGH].info->angle; 
	
	//Z��������	
	RampFloat (mechanical_Z, gimbal_motor[GIMBAL_LOW].info->angle, \
						gimbal_angle_max / 2, &machinery_pid_Z.gimbal_pid_angle->err);
	
	mec_pid_cal();

}

