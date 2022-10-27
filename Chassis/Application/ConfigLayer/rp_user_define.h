#ifndef __RP_USER_DEFINE_H__
#define __RP_USER_DEFINE_H__

#include "stm32f4xx.h"                  // Device header

//底盘向左yaw值+     向右yaw值—   
//云台向上pitch值-   向下pitch值+ 
//6020给正的输入值会逆时针转，给负的输入值会顺时针转


//主控板位置，1---上，0---下
#define Dashboard_Low       0
#define Dashboard_Above			1
#define Dashboard_Position  Dashboard_Above

//底盘最大速度和初始速度增加比例
#define Chassis_OutMax  		8000
#define V_Rate					Chassis_OutMax/660

#define Gimbal_OutMax       20000 

//云台的pitch极限值
#define gimbal_pitchmax  22.5f
#define gimbal_pitchmin  -38.5f

//底盘正方向的Z轴和Y轴的机械中值
#define mechanical_Z  			2050
#define mechanical_Y  			6800
#define tail_mechanical			6050

#define MOUSE_X_RATE 15.f//6.46f  //鼠标转换为通道值的比例
#define MOUSE_Y_RATE 10.f//8.49f  //鼠标转换为通道值的比例


//RM电机最大角度
#define RMmotor_angle_max  		8192

//pitch方向的6020电机的极限的角度
#define gimbal_angle_Ymax  		7300		//头低着的时候
#define gimbal_angle_Ymin  		6000		//头抬着的时候

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
#define Gryo_Speed_ZMax  300
#define Gryo_Speed_ZMin  150
#define Gryo_Speed_Z     250

//角度8192转变为360°的比例
#define Mec_To_Angle_Rate		( RMmotor_angle_max / 360 )   //22.75差不多

//弧度转换比例
#define Radian_Rate 57.f

//低通滤波比例
#define	FilterRate  0.2f

//枪管子弹发射的最大速度
#define Shoot_Vmax	2000	

//射击摩擦轮转速
#define Fri_15 4400
#define Fri_18 4700
#define Fri_20 4950
#define Fri_22 5200
#define Fri_30 7020
#define Fri_DF Fri_30

#define Temp 80

#endif
