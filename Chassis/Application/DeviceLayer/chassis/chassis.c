#include "chassis.h"

void Chassis_Init(void)
{
	for (int i = 0; i < 4; i ++)
	{
		chassis_motor[i].info_init (&chassis_motor[i]);		
		pid_val_init (chassis_motor[i].speed_pid);
		
		chassis_motor[i].speed_pid->kp = 6;
		chassis_motor[i].speed_pid->ki = 0.2;
		chassis_motor[i].speed_pid->kd = 0.5;
	}
}

	

void Chassis_MEC_Input(void)
{
	float speed[4] = {0};	
	
	float speed_x = 0, speed_y = 0, speed_z = 0;
	float angle_err = 0, speed_xx = 0, speed_yy = 0;
	
	speed_z = 0;
	
	
	/*---------------------------云台在正方向-------------------------*/
	if( gimbal_motor[GIMBAL_LOW].dir == Dir_Header )
	{	
			
		//-------------------非键盘模式下
		if( rc_sensor.dial->Keyboard_Mode == Flase )
		{		
			speed_x = (rc_sensor.info)->ch3;
			speed_y = (rc_sensor.info)->ch2;
		}
				
		//-------------------键盘模式下
		else
		{
			speed_x = rc_sensor.info->Ch[3];
			speed_y = rc_sensor.info->Ch[2]; 
		}
		
	}
	
	/*---------------------------云台在反方向--------------------------*/
	else
	{		
		//-------------------非键盘模式下
		if( rc_sensor.dial->Keyboard_Mode == Flase )
		{		
			
			speed_x = -(rc_sensor.info)->ch3;
			speed_y = -(rc_sensor.info)->ch2;
		}	
		
		//-------------------键盘模式下
		else
		{
			speed_x = -rc_sensor.info->Ch[3];
			speed_y = -rc_sensor.info->Ch[2]; 
		}
		
	}
	
	
	//机械模式下CH0直接赋值
	//---------------非键盘--------------------//
	if( rc_sensor.dial->Keyboard_Mode == Flase )
	{
		if ( rc_sensor.dial->Mechanical_Mode == True )
			speed_z = (rc_sensor.info)->ch0 * 0.6f;
	}

	//---------------键盘模式-----------------//
	else
	{
		if ( rc_sensor.dial->Mechanical_Mode == True )
			speed_z = rc_sensor.info->Ch[0] * 0.6f;
	}		
	
	
	
	//陀螺仪模式
	if ( rc_sensor.dial->Imu_Mode == True )
	{
		
		//如果是小陀螺模式
		if( rc_sensor.dial->This_Gryo_Mode == True )		
		{		
			angle_err = ( gimbal_motor[GIMBAL_LOW].info->angle - mechanical_Z ) / Mec_To_Angle_Rate ;
			
			if( gimbal_motor[GIMBAL_LOW].info->angle - mechanical_Z >= RMmotor_angle_max / 2 )
				angle_err -= 360;
			
			angle_err /= Radian_Rate;
				
			speed_xx = speed_x * cos (angle_err) + speed_y * sin (angle_err);
			speed_yy = - speed_x * sin (angle_err) + speed_y * cos (angle_err);
			
			//非键盘模式小陀螺旋转速度固定
			if( rc_sensor.dial->Keyboard_Mode == Flase )
				speed_z = Gryo_Speed_Z;	
			
			else
				speed_z = Keyboard_SpeedZ; 	
			
			speed_x = speed_xx;		
			speed_y = speed_yy;
			
			if( gimbal_motor[GIMBAL_LOW].dir == Dir_Tail )
			{
				speed_x *= -1;
				speed_y *= -1;
			}
		}	
		
		else
			//yaw轴的电机利用自身的位置PID计算出来的值给底盘旋转速度		
			speed_z = chassis_input;
	}
	
	
	speed[0] = (speed_x + speed_y + speed_z) * V_Rate;
	speed[1] = (- speed_x + speed_y + speed_z) * V_Rate;
	speed[2] = (speed_x - speed_y + speed_z) * V_Rate;
	speed[3] = (- speed_x - speed_y + speed_z) * V_Rate;

	//速度大于最大限幅，等比例缩小
	for ( int i = 0; i < 4; i ++ )
	{
		if ( MyAbs_Float (speed[i]) > Chassis_OutMax )
		{
			if( speed[i] <= 0 )
				speed[i] *= - Chassis_OutMax / speed[i];
			
			else
				speed[i] *=  Chassis_OutMax / speed[i];
			
		}												
	}
	
	(chassis_motor[CHAS_LF].speed_pid)->target = speed[0];
	(chassis_motor[CHAS_RF].speed_pid)->target = speed[1];
	(chassis_motor[CHAS_LB].speed_pid)->target = speed[2];
	(chassis_motor[CHAS_RB].speed_pid)->target = speed[3];

}

void Chassis_Txbuf_FillIn(void)
{
  int16_t speed_limit[4] = {0}; 
	
	single_speedpid_cal(&chassis_motor[CHAS_LF]);
	speed_limit[0] = (int16_t)chassis_motor[CHAS_LF].speed_pid->out;
	
	single_speedpid_cal(&chassis_motor[CHAS_RF]);
	speed_limit[1] = (int16_t)chassis_motor[CHAS_RF].speed_pid->out;
	
	single_speedpid_cal(&chassis_motor[CHAS_LB]);
	speed_limit[2] = (int16_t)chassis_motor[CHAS_LB].speed_pid->out;
	
	single_speedpid_cal(&chassis_motor[CHAS_RB]);
	speed_limit[3] = (int16_t)chassis_motor[CHAS_RB].speed_pid->out;

	//功率限制---电流
	Chassis_Motor_Power_Limit(speed_limit);
	
	
	chassis_txbuf[0] = speed_limit[0] >> 8;
	chassis_txbuf[1] = speed_limit[0];

	
	chassis_txbuf[2] = speed_limit[1] >> 8;
	chassis_txbuf[3] = speed_limit[1];
	
	
	chassis_txbuf[4] = speed_limit[2] >> 8;
	chassis_txbuf[5] = speed_limit[2];
	
	
	chassis_txbuf[6] = speed_limit[3] >> 8;
	chassis_txbuf[7] = speed_limit[3];
}	



void Chassis_send(void)
{
	Chassis_Txbuf_FillIn();
	
	CAN_Send_Msg ( &CAN1_TxMsg, Chassis_Control_ID, chassis_txbuf );
	
	memset ( chassis_txbuf, 0, sizeof(chassis_txbuf) );
}
