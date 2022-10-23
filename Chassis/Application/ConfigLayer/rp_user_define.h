#ifndef __RP_USER_DEFINE_H__
#define __RP_USER_DEFINE_H__

#include "stm32f4xx.h"                  // Device header

//底盘向左yaw值+     向右yaw值—   
//云台向上pitch值-   向下pitch值+ 
//6020给正的输入值会逆时针转，给负的输入值会顺时针转


//底盘最大速度和初始速度增加比例
#define Chassis_VMax  		8000
#define V_Rate					Chassis_VMax/660

//云台的pitch极限值
#define gimbal_pitchmax  22.5f
#define gimbal_pitchmin  -38.5f

//底盘正方向的Z轴和Y轴的机械中值
#define mechanical_Z  				2050
#define mechanical_Y  				6800
#define tail_mechanical				6050


//RM电机最大角度
#define RMmotor_angle_max  		8192

//pitch方向的6020电机的极限的角度
#define gimbal_angle_Ymax  		7300		//头低着的时候
#define gimbal_angle_Ymin  		5900		//头抬着的时候

//CAN发送的电机指令
#define Gimbal_Control_ID 		0x1ff
#define Chassis_Control_ID 		0x200
#define Shoot_Control_ID			0x200

//拨码位置对应的模式值(从上往下 1---3---2)
					//左
#define keyboard_mode		1
#define imu_mode 				3
#define mec_mode 				2


	//陀螺仪模式下的右拨码
#define continue_shot_mode			1
#define version_control_mode    3
#define fricwheel_state_trans		2	

	//机械模式下的右拨码
#define single_shot_mode				1
#define cover_state_trans				2

//小陀螺模式下speed_z的赋值
#define Gryo_Speed_Z    250

//角度8192转变为360°的比例
#define Mec_To_Angle_Rate		( RMmotor_angle_max / 360 )   //22.75差不多

//弧度转换比例
#define Radian_Rate 57.f

//低通滤波比例
#define	FilterRate  0.2f

//枪管子弹发射的最大速度
#define Shoot_Vmax	2000	
#endif
