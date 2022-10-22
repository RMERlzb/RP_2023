#ifndef __Gimbal_Task_H
#define __Gimbal_Task_H

#include "stm32f4xx.h"   
#include "cmsis_os.h"
#include "string.h"

#include "rp_device_config.h"
#include "RC_Dial_Task.h"
#include "gimbal.h"
#include "RC_Heart_Task.h"

extern void StartGimbal_Task(void const * argument);

#endif
