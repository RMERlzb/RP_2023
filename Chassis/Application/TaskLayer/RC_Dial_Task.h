#ifndef __RC_Dial_Task_H__
#define __RC_Dial_Task_H__

#include "stm32f4xx.h"                  // Device header
#include "cmsis_os.h"
#include "rc_sensor.h"
#include "rp_user_define.h"

extern char Mechanical_Mode, Imu_Mode, Gryo_Mode;

void StartRC_DialTask(void const * argument);
#endif
