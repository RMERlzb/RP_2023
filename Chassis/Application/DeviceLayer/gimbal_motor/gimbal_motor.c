#include "gimbal_motor.h"
void gimbal_motor_init(gimbal_motor_t *motor);
void gimbal_motor_update(gimbal_motor_t *motor, uint8_t *rxbuf);
void gimbal_motor_check(gimbal_motor_t *motor);

gimbal_motor_info_t gimbal_motor_info[GIMBAL_MOTOR_CNT]; 

pid_ctrl_t gimbal_pid[GIMBAL_MOTOR_CNT];

gimbal_motor_t gimbal_motor[GIMBAL_MOTOR_CNT] = {
	[GIMBAL_LOW] = {
		.info = &gimbal_motor_info[GIMBAL_LOW],
		.pid  = &gimbal_pid[GIMBAL_LOW],
		.id   = DEV_ID_GIMBAL_LOW,
		.init = gimbal_motor_init,
		.update = gimbal_motor_update,
		.check = gimbal_motor_check,
	},
	[GIMBAL_HIGH] = {
		.info = &gimbal_motor_info[GIMBAL_HIGH],
		.pid  = &gimbal_pid[GIMBAL_HIGH],
		.id   = DEV_ID_GIMBAL_HIGH,
		.init = gimbal_motor_init,
		.update = gimbal_motor_update,
		.check = gimbal_motor_check,
	},
};

void gimbal_motor_init(gimbal_motor_t *motor){
	(motor->info)->angle = 0;
	(motor->info)->current = 0;
	(motor->info)->speed = 0;
	(motor->info)->temperature = 0;
	(motor->info)->offline_max_cnt = 50;
	(motor->info)->offline_cnt = 0;
	(motor->info)->work_state = DEV_ONLINE;
}
void gimbal_motor_update(gimbal_motor_t *motor,uint8_t *rxbuf){
	(motor->info)->angle = (int16_t)rxbuf[0]<<8 | rxbuf[1];
	(motor->info)->speed = (int16_t)rxbuf[2]<<8 | rxbuf[3];
	(motor->info)->current = (int16_t)rxbuf[4]<<8 | rxbuf[5];
	(motor->info)->temperature = rxbuf[6];
	(motor->info)->offline_cnt = 0;
}	
void gimbal_motor_check(gimbal_motor_t *motor){
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
