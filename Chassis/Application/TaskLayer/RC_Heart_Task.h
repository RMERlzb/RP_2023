#ifndef __RC_Heart_Task_H__
#define __RC_Heart_Task_H__

#include "stm32f4xx.h"                  // Device header
#include "cmsis_os.h"
#include "rc_sensor.h"
#include "chassis.h"
#include "rp_user_define.h"
#include "gimbal.h"  
#include "RC_Dial_Task.h"
/*
	任务优先级高于普通
	tick用于在500ms后一直给底盘和云台发送0数组
	一直检测遥控器是否断电，如果断电了，在Chassis_Task函数中会停止底盘和云台的控制
	遥控器断电后立刻读取系统时钟值，并且让底盘目标值为0进行减速，云台的目标值清0
	500ms后一直发送数组0
*/
extern char Mec_Return_flag;
void StartRC_HeartTask(void const * argument);

#endif
