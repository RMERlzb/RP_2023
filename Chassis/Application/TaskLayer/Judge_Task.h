#ifndef __Judge_Task_H
#define __Judge_Task_H

#include "stm32f4xx.h"   
#include "cmsis_os.h"
#include "string.h"

#include "judge_sensor.h"
#include "judge_potocol.h"
#include "slave.h"
#include "rp_user_define.h"
void StartJudge_Task(void const * argument);
#endif
