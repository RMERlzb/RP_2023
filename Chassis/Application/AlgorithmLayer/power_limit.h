#ifndef __POWER_LIMIT_H
#define __POWER_LIMIT_H


#include "judge_potocol.h"
#include "rp_user_define.h"
#include "rp_math.h"

void Chassis_Motor_Power_Limit(int16_t *data);
	
char Shoot_Power_Judge(void);	
	
void Chassis_2022_CAP_Power_Limit(int16_t *data);
	
void Chassis_Turn_Power_Limit(int16_t *data);
	
void Judge_Offline_Power_Limit(int16_t *data);
		
#endif

