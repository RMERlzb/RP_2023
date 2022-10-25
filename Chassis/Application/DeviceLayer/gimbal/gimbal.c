#include "gimbal.h"  

pid_ctrl_t gimbal_imu_pid[2][2];//第一列放陀螺仪的角度，第二列放陀螺仪的角速度


//跟随模式下：
imu_pid_t imu_pid_Z;
imu_pid_t imu_pid_Y;


//机械模式下：
machinery_pid_t  machinery_pid_Z;
machinery_pid_t  machinery_pid_Y;


//Z轴云台输出给底盘
float chassis_input = 0;


//陀螺仪模式
void imu_pid_init(void);
void imu_pid_cal(void);
void Chassis_Input(void);

//机械模式
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
	//Y轴误差计算
	gimbal_motor[GIMBAL_HIGH].angle_pid_update(&gimbal_motor[GIMBAL_HIGH]);	
	
	
	//Z轴误差计算	
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
	
	//从陀螺仪模式切换到机械模式，云台状态保持不变
	machinery_pid_Y.gimbal_angle_pid->target = gimbal_motor[GIMBAL_HIGH].info->angle;
	
	//此时盖子打开了
	if( Judge_Cover_State() == Open )
		Cover_Close();
	
	Chassis_Input();
	
	imu_pid_cal();
	
		
}


void MEC_Mode(void)
{
	//从机械模式切换到陀螺仪模式下，云台还是保持上一时刻状态
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
	//上下两个云台电机的陀螺仪pid环(陀螺仪角度+角速度)
	imu_pid_Z.imu_angle_pid = &gimbal_imu_pid[0][0];
	imu_pid_Z.imu_speed_pid = &gimbal_imu_pid[0][1];
	
	
	imu_pid_Y.imu_angle_pid = &gimbal_imu_pid[1][0];
	imu_pid_Y.imu_speed_pid = &gimbal_imu_pid[1][1];
	
	//-----------------------下面云台电机两个环---------------------//
	
	
	//外	
	pid_val_init(imu_pid_Z.imu_angle_pid);
	imu_pid_Z.imu_angle_pid->kp = 210;
	imu_pid_Z.imu_angle_pid->out_max = 20000;
	imu_pid_Z.imu_angle_pid->target = 0;//---------------------陀螺仪Z轴角度目标值
	
	
	//内	
	pid_val_init(imu_pid_Z.imu_speed_pid);
	imu_pid_Z.imu_speed_pid->kp = 80;
	imu_pid_Z.imu_speed_pid->ki = 1;
	imu_pid_Z.imu_speed_pid->out_max = 20000;
	imu_pid_Z.imu_speed_pid->integral_max = 10000;
	
	
	//----------------------上面云台电机两个环-----------------------//	
	
	
	//外
	pid_val_init(imu_pid_Y.imu_angle_pid);
	imu_pid_Y.imu_angle_pid->kp = 260;
	imu_pid_Y.imu_angle_pid->out_max = 20000;
	imu_pid_Y.imu_angle_pid->target = 0;//---------------------陀螺仪Y轴角度目标值
	
	//内
	pid_val_init(imu_pid_Y.imu_speed_pid);
	imu_pid_Y.imu_speed_pid->kp = 20;
	imu_pid_Y.imu_speed_pid->ki = 1;
	imu_pid_Y.imu_speed_pid->out_max = 20000;
	imu_pid_Y.imu_speed_pid->integral_max = 2000;
	
	
	
}

void imu_pid_cal(void)
{


	
//---------------------------目标值的更改地方-------------------------//	
	//如果开启视觉控制
	if( rc_sensor_dial.Version_Control_Enable == True )
	{			
		
		imu_pid_Z.imu_angle_pid->target = vision_sensor_info.RxPacket.RxData.yaw_angle / 22.7555f - 180;
		
		
		imu_pid_Y.imu_angle_pid->target = vision_sensor_info.RxPacket.RxData.pitch_angle / 22.7555f - 180;
				
	}
	
	//开启了视觉就不能再用遥控器控制云台
	else
	{
		//陀螺仪模式下角度增量变化率，注意是负的
		imu_pid_Z.imu_angle_pid->target += (rc_sensor.info)->ch0 * -0.0005f;
			
		imu_pid_Y.imu_angle_pid->target += (rc_sensor.info)->ch1 * -0.0008f ;
	
		
	}
	
	
//--------------------------------------Z轴的--------------------------------//


//----------------------------------计算------------------------------//
	
//-------------外环	
	imu_pid_Z.imu_angle_pid->measure = yaw;
	MyRampFloat (&imu_pid_Z.imu_angle_pid->target, yaw, 180, &imu_pid_Z.imu_angle_pid->err);
	single_pid_ctrl ( imu_pid_Z.imu_angle_pid, imu_pid_Z.imu_angle_pid->err );
	
	
//-------------内环	
	imu_pid_Z.imu_speed_pid->target =  imu_pid_Z.imu_angle_pid->out;
	imu_pid_Z.imu_speed_pid->measure = ggz;
	imu_pid_Z.imu_speed_pid->err = imu_pid_Z.imu_speed_pid->target - imu_pid_Z.imu_speed_pid->measure;
	single_pid_ctrl ( imu_pid_Z.imu_speed_pid, imu_pid_Z.imu_speed_pid->err );
	
	
//-------------------------------填充数组--------------------------//	
	gimbal_txbuf[0] = (int16_t) imu_pid_Z.imu_speed_pid->out>>8;
	gimbal_txbuf[1] = (int16_t) imu_pid_Z.imu_speed_pid->out;
	
	
	
	
	
	
//--------------------------------------Y轴的--------------------------------//	



//----------------------------------计算------------------------------//
//-------------外环	
	ConstrainFloat( &imu_pid_Y.imu_angle_pid->target, gimbal_pitchmax, gimbal_pitchmin);
	imu_pid_Y.imu_angle_pid->measure = pitch;
	imu_pid_Y.imu_angle_pid->err = imu_pid_Y.imu_angle_pid->target - imu_pid_Y.imu_angle_pid->measure;
	single_pid_ctrl ( imu_pid_Y.imu_angle_pid, imu_pid_Y.imu_angle_pid->err );


//-------------内环	
	imu_pid_Y.imu_speed_pid->target =  imu_pid_Y.imu_angle_pid->out;
	imu_pid_Y.imu_speed_pid->measure = ggy;
	imu_pid_Y.imu_speed_pid->err = imu_pid_Y.imu_speed_pid->target - ggy ;
	single_pid_ctrl ( imu_pid_Y.imu_speed_pid, imu_pid_Y.imu_speed_pid->err );
	

//-------------------------------填充数组--------------------------//		
	gimbal_txbuf[2] = (int16_t) imu_pid_Y.imu_speed_pid->out>>8;
	gimbal_txbuf[3] = (int16_t) imu_pid_Y.imu_speed_pid->out;
	

}




void Chassis_Input(void)
{
	
	float kp_temp = 0, kd_temp = 0,out_max_temp = 0;
	
	chassis_input = 0;
	
	//保存机械模式下Z轴的混环PID的外环信息（其实就是Z轴电机的信息），目标值修改
	kp_temp = machinery_pid_Z.gimbal_angle_pid->kp;
	kd_temp = machinery_pid_Z.gimbal_angle_pid->kd;
	out_max_temp = machinery_pid_Z.gimbal_angle_pid->out_max;
	
	
	
	//附新的值
	machinery_pid_Z.gimbal_angle_pid->kp = 0.5;
	machinery_pid_Z.gimbal_angle_pid->kd = 3;
	machinery_pid_Z.gimbal_angle_pid->out_max = 1000;		
	
	
	//---------------------------------头尾跟随判断目标值-----------------------------//
	if( rc_sensor_dial.Imu_Mode == True )
	{
		//说明取消了小陀螺
		if( rc_sensor_dial.Gryo_Mode == Flase )
		{
			//跟头
			if( gimbal_motor[GIMBAL_LOW].info->angle > 0 && gimbal_motor[GIMBAL_LOW].info->angle <= 4050 )
			{
				
				machinery_pid_Z.gimbal_angle_pid->target = mechanical_Z;
				
				gimbal_motor[GIMBAL_LOW].dir = Dir_Header;
				
			}
				
			
			//跟尾
			else if( gimbal_motor[GIMBAL_LOW].info->angle > 4050 && gimbal_motor[GIMBAL_LOW].info->angle <= 8192 )
			{
				
				machinery_pid_Z.gimbal_angle_pid->target = tail_mechanical;
				
				gimbal_motor[GIMBAL_LOW].dir = Dir_Tail;
				
			}
				
			
		}
		
		//旋转速度赋值
		single_anglepid_cal(&gimbal_motor[GIMBAL_LOW]);
		
		chassis_input = gimbal_motor[GIMBAL_LOW].angle_pid->out;
		
	}
	

	//变回机械模式Z轴PID外环的信息
	machinery_pid_Z.gimbal_angle_pid->kp = kp_temp;
	machinery_pid_Z.gimbal_angle_pid->kd = kd_temp;
	machinery_pid_Z.gimbal_angle_pid->out_max = out_max_temp;
	
	
}





void machinery_pid_init(void)
{
	
	//上下两个云台电机的混环PID(外环电机角度+内环陀螺仪角速度)
	machinery_pid_Z.gimbal_angle_pid = gimbal_motor[GIMBAL_LOW].angle_pid;
	machinery_pid_Z.imu_speed_pid = &gimbal_imu_pid[0][1];
		
	
	machinery_pid_Y.gimbal_angle_pid = gimbal_motor[GIMBAL_HIGH].angle_pid;
	machinery_pid_Y.imu_speed_pid = &gimbal_imu_pid[1][1];
	
	
	
	//-----------------------下面云台电机两个环---------------------//
	

	//外	
	pid_val_init(machinery_pid_Z.gimbal_angle_pid);
	machinery_pid_Z.gimbal_angle_pid->kp = 10;
	machinery_pid_Z.gimbal_angle_pid->out_max = 20000;
	machinery_pid_Z.gimbal_angle_pid->target = mechanical_Z;//-----------Z轴机械角度目标值一开始的
		
	//内	
	pid_val_init(machinery_pid_Z.imu_speed_pid);
	machinery_pid_Z.imu_speed_pid->kp = 50;
	machinery_pid_Z.imu_speed_pid->ki = 1;
	machinery_pid_Z.imu_speed_pid->out_max = 20000;
	machinery_pid_Z.imu_speed_pid->integral_max = 10000;
	
	
	
	//----------------------上面云台电机两个环-----------------------//	
	
	//外	
	pid_val_init(machinery_pid_Y.gimbal_angle_pid);
	machinery_pid_Y.gimbal_angle_pid->kp = 10;
	machinery_pid_Y.gimbal_angle_pid->out_max = 20000;
	machinery_pid_Y.gimbal_angle_pid->target = mechanical_Y;//-----------Y轴机械角度目标值一开始的
	
	
	//内
	pid_val_init(machinery_pid_Y.imu_speed_pid);
	machinery_pid_Y.imu_speed_pid->kp = 10;
	machinery_pid_Y.imu_speed_pid->ki = 1;
	machinery_pid_Y.imu_speed_pid->out_max = 20000;
	machinery_pid_Y.imu_speed_pid->integral_max = 10000;
	
	
}

void mec_pid_cal(void)
{

//----------------------------Y轴---------------------------//
	
	//----外环目标值---//	
	machinery_pid_Y.gimbal_angle_pid->target += (rc_sensor.info)->ch1 * -0.005f;		
	ConstrainFloat( &machinery_pid_Y.gimbal_angle_pid->target, gimbal_angle_Ymax, gimbal_angle_Ymin);	

	
	//----外环测量值和误差----//	
	gimbal_motor[GIMBAL_HIGH].angle_pid_update(&gimbal_motor[GIMBAL_HIGH]);

	
	//----外环的计算-------//
	single_pid_ctrl ( machinery_pid_Y.gimbal_angle_pid, machinery_pid_Y.gimbal_angle_pid->err );
	
	
	
	//-----内环目标值----//
	machinery_pid_Y.imu_speed_pid->target = machinery_pid_Y.gimbal_angle_pid->out;
	
	
	//-----内环测量值和误差----//
	machinery_pid_Y.imu_speed_pid->measure = ggy * cos( pitch / Radian_Rate) ;
	machinery_pid_Y.imu_speed_pid->err = machinery_pid_Y.imu_speed_pid->target - machinery_pid_Y.imu_speed_pid->measure;
	
	
	//-----内环的计算----//
	single_pid_ctrl ( machinery_pid_Y.imu_speed_pid, machinery_pid_Y.imu_speed_pid->err );


//----------------------数组填充---------------------------//	
	gimbal_txbuf[2] = (int16_t) machinery_pid_Y.imu_speed_pid->out>>8;
	gimbal_txbuf[3] = (int16_t) machinery_pid_Y.imu_speed_pid->out;
	

		
	
//----------------------------Z轴-----------------------------//

	//----外环测量值和误差-----//
	gimbal_motor[GIMBAL_LOW].angle_pid_update(&gimbal_motor[GIMBAL_LOW]);
	
	
	//----外环的计算----//
	single_pid_ctrl ( machinery_pid_Z.gimbal_angle_pid, machinery_pid_Z.gimbal_angle_pid->err );
		
		
	//-----内环目标值----//
	machinery_pid_Z.imu_speed_pid->target = machinery_pid_Z.gimbal_angle_pid->out;
	
	
	//-----内环测量值和误差----//
	machinery_pid_Z.imu_speed_pid->measure = ggz ;
	machinery_pid_Z.imu_speed_pid->err = machinery_pid_Z.imu_speed_pid->target - machinery_pid_Z.imu_speed_pid->measure;
	
	
	//----内环的计算-----//
	single_pid_ctrl ( machinery_pid_Z.imu_speed_pid, machinery_pid_Z.imu_speed_pid->err );

	
	//-----填充数组-----//
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


