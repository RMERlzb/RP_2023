#ifndef __Chassis_Task_H__
#define __Chassis_Task_H__
#include "stm32f4xx.h"                  // Device header
#include "string.h"
#include "cmsis_os.h"

#include "drv_can.h"
#include "chassis.h"
#include "gimbal.h"
#include "rp_driver_config.h"
#include "rp_user_define.h"
#include "rc_sensor.h"

/*
	该任务是普通级别
	负责底盘和云台的控制
*/

extern void StartChassis_Task(void const * argument);
extern void StartWorkStateTask(void const * argument);
#endif
