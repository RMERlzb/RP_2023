#ifndef __shoot_H__
#define __shoot_H__

#include "stm32f4xx.h"                  // Device header
#include "shoot_motor.h"
#include "drv_can.h"
#include "rp_user_define.h"
#include "string.h"
#include "RC_Dial_Task.h"
#include "rp_math.h"
#include "pid.h"

void Shoot_Init(void);
void FricWheel_WorkState_Switch(void);
void Block_Manage(void);
void Shoot(void);
void Shoot_Send(void);
#endif
