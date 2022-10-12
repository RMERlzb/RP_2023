#include "chassis.h"

void Chassis_Init(void)
{
	for(int i = 0; i < 4; i ++)
	{
		chassis_motor[i].init (&chassis_motor[i]);	
		pid_val_init (chassis_motor[i].pid);
		chassis_motor[i].pid->kp = 6;
		chassis_motor[i].pid->ki = 0.2;
		chassis_motor[i].pid->kd = 0.5;
	}
}
void Chassis_MEC_Input(void)
{
	float speed[4] = {0};	
	float speed_x = 0, speed_y = 0, speed_z = 0;
	float AngleErr = 0, speed_xx = 0, speed_yy = 0;
	
	speed_x = (rc_sensor.info)->ch3;
	speed_y = (rc_sensor.info)->ch2;
	speed_z = 0;
	
	//机械模式下CH0直接赋值
	if ( Mechanical_Mode )
	{
		
		speed_z = (rc_sensor.info)->ch0 * 0.6f;
		
	}
	//陀螺仪模式
	else if ( Imu_Mode )
	{
		
		//yaw轴的电机利用自身的位置PID计算出来的值给底盘旋转速度		
		
		speed_z = gimbal_motor[GIMBAL_LOW].pid->out;
			
		//如果是小陀螺模式
		if( Gryo_Mode )		
		{		
			AngleErr = ( gimbal_motor[GIMBAL_LOW].info->angle - mechanical_Z ) / Mec_To_Angle_Rate ;
			
			if(gimbal_motor[GIMBAL_LOW].info->angle - mechanical_Z >= gimbal_angle_max / 2)
				AngleErr -= 360;
			
			AngleErr /= Radian_Rate;
				
			speed_xx = speed_x * cos (AngleErr) + speed_y * sin (AngleErr);
			speed_yy = - speed_x * sin (AngleErr) + speed_y * cos (AngleErr);
			
			speed_z = Gryo_Speed_Z;	
			speed_x = speed_xx;		
			speed_y = speed_yy;
		}	
		
	}
	
	speed[0] = (speed_x + speed_y + speed_z) * V_Rate;
	speed[1] = (- speed_x + speed_y + speed_z) * V_Rate;
	speed[2] = (speed_x - speed_y + speed_z) * V_Rate;
	speed[3] = (- speed_x - speed_y + speed_z) * V_Rate;
			
	for (int i = 0; i < 4; i ++)
	{
	
		if (MyAbs_Float (speed[i]) > V_Max)
		{
			if(speed[i] <= 0)
				speed[i] *= - V_Max / speed[i];
			
			else
				speed[i] *=  V_Max / speed[i];
			
		}												
	}
	
	(chassis_motor[CHAS_LF].pid)->target = speed[0];
	(chassis_motor[CHAS_RF].pid)->target = speed[1];
	(chassis_motor[CHAS_LB].pid)->target = speed[2];
	(chassis_motor[CHAS_RB].pid)->target = speed[3];

}
void Chassis_send(void)
{
	
	single_pid_ctrl (chassis_motor[CHAS_LF].pid, \
									(chassis_motor[CHAS_LF].pid)->target - (chassis_motor[CHAS_LF].info)->speed);
	chassis_txbuf[0] = (int16_t) (chassis_motor[CHAS_LF].pid)->out>>8;
	chassis_txbuf[1] = (int16_t) (chassis_motor[CHAS_LF].pid)->out;

	single_pid_ctrl (chassis_motor[CHAS_RF].pid,\
									(chassis_motor[CHAS_RF].pid)->target - (chassis_motor[CHAS_RF].info)->speed);
	chassis_txbuf[2] = (int16_t) (chassis_motor[CHAS_RF].pid)->out>>8;
	chassis_txbuf[3] = (int16_t) (chassis_motor[CHAS_RF].pid)->out;
	
	single_pid_ctrl (chassis_motor[CHAS_LB].pid,\
									(chassis_motor[CHAS_LB].pid)->target - (chassis_motor[CHAS_LB].info)->speed);
	chassis_txbuf[4] = (int16_t) (chassis_motor[CHAS_LB].pid)->out>>8;
	chassis_txbuf[5] = (int16_t) (chassis_motor[CHAS_LB].pid)->out;
	
	single_pid_ctrl (chassis_motor[CHAS_RB].pid,\
									(chassis_motor[CHAS_RB].pid)->target - (chassis_motor[CHAS_RB].info)->speed);
	chassis_txbuf[6] = (int16_t) (chassis_motor[CHAS_RB].pid)->out>>8;
	chassis_txbuf[7] = (int16_t) (chassis_motor[CHAS_RB].pid)->out;
	
	
	CAN_Send_Msg (&CAN1_TxMsg , Chassis_Control_ID , chassis_txbuf);
	
	memset (chassis_txbuf, 0, sizeof(chassis_txbuf) );
}
