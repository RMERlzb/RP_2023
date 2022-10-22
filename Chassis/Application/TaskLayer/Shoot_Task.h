#ifndef __Shoot_Task_H
#define __Shoot_Task_H

#include "stm32f4xx.h"   
#include "cmsis_os.h"
#include "string.h"

#include "rp_device_config.h"
#include "RC_Dial_Task.h"
#include "shoot.h"

extern void StartShoot_Task(void const * argument);

#endif
