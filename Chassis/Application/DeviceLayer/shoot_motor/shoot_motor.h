#ifndef __Shoot_Motor_H
#define __Shoot_Motor_H

#include "stm32f4xx.h"                  // Device header
#include "rp_device_config.h"
#include "pid.h"

typedef struct shoot_motor_info_struct{
	uint16_t		angle;
	int16_t	  	speed;
	int16_t 		current;
  uint8_t   	temperature;
	uint16_t		angle_prev;
	int32_t			angle_sum;
	uint8_t  		offline_cnt;
	uint8_t			offline_max_cnt;	
	dev_work_state_t   		work_state;
}shoot_motor_info_t;

typedef struct shoot_motor_struct{
	shoot_motor_info_t 	*info;
	pid_ctrl_t*       		pid;
	dev_shoot_id_t				id;
	void 	(*init)(struct shoot_motor_struct *motor);
	void  (*update)(struct shoot_motor_struct *motor, uint8_t *rxBuf);
	void  (*check)(struct shoot_motor_struct *motor);
}shoot_motor_t;

extern shoot_motor_t shoot_motor[SHOOT_MOTOR_CNT];
#endif
