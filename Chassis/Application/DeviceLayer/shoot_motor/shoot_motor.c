#include "shoot_motor.h"
void shoot_motor_init(shoot_motor_t *motor);
void shoot_motor_update(shoot_motor_t *motor, uint8_t *rxbuf);
void shoot_motor_check(shoot_motor_t *motor);

shoot_motor_info_t shoot_motor_info[SHOOT_MOTOR_CNT]; 

pid_ctrl_t shoot_pid[SHOOT_MOTOR_CNT];

shoot_motor_t shoot_motor[SHOOT_MOTOR_CNT] = {
	[SHOOT_DIAL] = {
		.info = &shoot_motor_info[SHOOT_DIAL],
		.pid  = &shoot_pid[SHOOT_DIAL],
		.id   = DEV_ID_SHOOT_DIAL,
		.init = shoot_motor_init,
		.update = shoot_motor_update,
		.check = shoot_motor_check,
	},
	[SHOOT_LRUB] = {
		.info = &shoot_motor_info[SHOOT_LRUB],
		.pid  = &shoot_pid[SHOOT_LRUB],
		.id   = DEV_ID_SHOOT_LRUB,
		.init = shoot_motor_init,
		.update = shoot_motor_update,
		.check = shoot_motor_check,
	},
	[SHOOT_RRUB] = {
		.info = &shoot_motor_info[SHOOT_RRUB],
		.pid  = &shoot_pid[SHOOT_RRUB],
		.id   = DEV_ID_SHOOT_RRUB,
		.init = shoot_motor_init,
		.update = shoot_motor_update,
		.check = shoot_motor_check,
	},
	[SHOOT_GUN] = {
		.info = &shoot_motor_info[SHOOT_GUN],
		.pid  = &shoot_pid[SHOOT_GUN],
		.id   = DEV_ID_SHOOT_GUN,
		.init = shoot_motor_init,
		.update = shoot_motor_update,
		.check = shoot_motor_check,
	},
};


void shoot_motor_init(shoot_motor_t *motor)
{
	(motor->info)->angle = 0;
	(motor->info)->current = 0;
	(motor->info)->speed = 0;
	(motor->info)->temperature = 0;
	(motor->info)->offline_max_cnt = 50;
	(motor->info)->offline_cnt = 0;
	(motor->info)->work_state = DEV_ONLINE;
}
void shoot_motor_update(shoot_motor_t *motor,uint8_t *rxbuf)
{
	(motor->info)->angle = (int16_t)rxbuf[0]<<8 | rxbuf[1];
	(motor->info)->speed = (int16_t)rxbuf[2]<<8 | rxbuf[3];
	(motor->info)->current = (int16_t)rxbuf[4]<<8 | rxbuf[5];
	(motor->info)->temperature = rxbuf[6];
	(motor->info)->offline_cnt = 0;
}	
void shoot_motor_check(shoot_motor_t *motor)
{
	(motor->info)->offline_cnt++;
	if((motor->info)->offline_cnt > (motor->info)->offline_max_cnt){
		(motor->info)->offline_cnt = (motor->info)->offline_max_cnt;
		(motor->info)->work_state = DEV_OFFLINE;
	}
	else{
		if((motor->info)->work_state == DEV_OFFLINE){
			(motor->info)->work_state = DEV_ONLINE;		
		}
	}		
}
