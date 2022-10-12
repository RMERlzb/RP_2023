#ifndef __Imu_Task_H__
#define __Imu_Task_H__

#include "stm32f4xx.h"                  // Device header
#include "string.h"
#include "cmsis_os.h"
#include "pid.h"
#include "bmi.h"
#include "gimbal.h"
#include "rp_user_define.h"
/*
	任务优先级高于普通级别
	
	会初始化好陀螺仪设备
	对Z和Y的角速度进行了滤波
	在任务里会一直得到欧拉角和两个角速度
	两个6020电机有各自的imu_pid_t类型的PID，并做好PID的参数赋值
	
	通过陀螺仪的角度和角速度建立起串级PID，并把输出量给到对应的6020电机
	能够做到云台到达指定的yaw角度和pitch角度
*/
extern short ggx, ggy, ggz;
extern short aax, aay, aaz;
extern float pitch, roll, yaw;

extern void StartImuTask(void const * argument);
	
#endif
