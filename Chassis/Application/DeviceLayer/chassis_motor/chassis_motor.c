#include "chassis_motor.h"
void chassis_motor_init(chassis_motor_t *motor);
void chassis_motor_update(chassis_motor_t *motor,uint8_t *rxbuf);
void chassis_motor_check(chassis_motor_t *motor);

chassis_motor_info_t chassis_motor_info[CHAS_MOTOR_CNT];

pid_ctrl_t chassis_pid[CHAS_MOTOR_CNT];

chassis_motor_t	chassis_motor[CHAS_MOTOR_CNT] = {
	[CHAS_LF] = {
		.info = &chassis_motor_info[CHAS_LF],
		.pid	= &chassis_pid[CHAS_LF],
		.init = chassis_motor_init,
		.update = chassis_motor_update,
		.check = chassis_motor_check,
		.id = DEV_ID_CHASSIS_LF,
	},
	[CHAS_RF] = {
		.info = &chassis_motor_info[CHAS_RF],
		.pid	= &chassis_pid[CHAS_RF],
		.init = chassis_motor_init,
		.update = chassis_motor_update,
		.check = chassis_motor_check,
		.id = DEV_ID_CHASSIS_RF,
	},
	[CHAS_LB] = {
		.info = &chassis_motor_info[CHAS_LB],
		.pid	= &chassis_pid[CHAS_LB],
		.init = chassis_motor_init,
		.update = chassis_motor_update,
		.check = chassis_motor_check,
		.id = DEV_ID_CHASSIS_LB,
	},
	[CHAS_RB] = {
		.info = &chassis_motor_info[CHAS_RB],
		.pid	= &chassis_pid[CHAS_RB],
		.init = chassis_motor_init,
		.update = chassis_motor_update,
		.check = chassis_motor_check,
		.id = DEV_ID_CHASSIS_RB,
	},
};
void chassis_motor_init(chassis_motor_t *motor){
	(motor->info)->angle = 0;
	(motor->info)->current = 0;
	(motor->info)->speed = 0;
	(motor->info)->temperature = 0;
	(motor->info)->offline_max_cnt = 50;
	(motor->info)->offline_cnt = 0;
	(motor->info)->work_state = DEV_ONLINE;
}
void chassis_motor_update(chassis_motor_t *motor, uint8_t *rxbuf){
	(motor->info)->angle = (int16_t)rxbuf[0]<<8 | rxbuf[1];
	(motor->info)->speed = (int16_t)rxbuf[2]<<8 | rxbuf[3];
	(motor->info)->current = (int16_t)rxbuf[4]<<8 | rxbuf[5];
	(motor->info)->temperature = rxbuf[6];
	(motor->info)->offline_cnt = 0;
}	
void chassis_motor_check(chassis_motor_t *motor){
	(motor->info)->offline_cnt++;
	
	if( (motor->info)->offline_cnt > (motor->info)->offline_max_cnt ){
		(motor->info)->offline_cnt = (motor->info)->offline_max_cnt;
		(motor->info)->work_state = DEV_OFFLINE;
	}
	else{
		if( (motor->info)->work_state == DEV_OFFLINE ){
			(motor->info)->work_state = DEV_ONLINE;		
		}
	}		
}
