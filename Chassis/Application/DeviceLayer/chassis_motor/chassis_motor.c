#include "chassis_motor.h"

motor_info_t chassis_motor_info[CHAS_MOTOR_CNT];

pid_ctrl_t chassis_speed_pid[CHAS_MOTOR_CNT];

pid_ctrl_t chassis_angle_pid[CHAS_MOTOR_CNT];

pid_ctrl_t chassis_anglesum_pid[CHAS_MOTOR_CNT];

motor_t	chassis_motor[CHAS_MOTOR_CNT] = {
	[CHAS_LF] = {
		
		.info = &chassis_motor_info[CHAS_LF],
		.speed_pid = &chassis_speed_pid[CHAS_LF],
		.angle_pid = &chassis_angle_pid[CHAS_LF],
		.anglesum_pid = &chassis_anglesum_pid[CHAS_LF],
		
		.info_init = motor_info_init,
		.info_update = motor_info_update,
		.check = motor_check,
		.speed_pid_update = speed_pid_update,
		.angle_pid_update = angle_pid_update,
		.anglesum_pid_update = anglesum_pid_update,
		
	},
	[CHAS_RF] = {
		
		.info = &chassis_motor_info[CHAS_RF],
		.speed_pid	= &chassis_speed_pid[CHAS_RF],
		.angle_pid = &chassis_angle_pid[CHAS_RF],
		.anglesum_pid = &chassis_anglesum_pid[CHAS_RF],
		
		.info_init = motor_info_init,
		.info_update = motor_info_update,
		.check = motor_check,
		.speed_pid_update = speed_pid_update,
		.angle_pid_update = angle_pid_update,
		.anglesum_pid_update = anglesum_pid_update,
		
	},
	[CHAS_LB] = {
		
		.info = &chassis_motor_info[CHAS_LB],
		.speed_pid	= &chassis_speed_pid[CHAS_LB],
		.angle_pid = &chassis_angle_pid[CHAS_LB],
		.anglesum_pid = &chassis_anglesum_pid[CHAS_LB],
		
		.info_init = motor_info_init,
		.info_update = motor_info_update,
		.check = motor_check,
		.speed_pid_update = speed_pid_update,
		.angle_pid_update = angle_pid_update,
		.anglesum_pid_update = anglesum_pid_update,
		
	},
	[CHAS_RB] = {
		
		.info = &chassis_motor_info[CHAS_RB],
		.speed_pid	= &chassis_speed_pid[CHAS_RB],
		.angle_pid = &chassis_angle_pid[CHAS_RB],
		.anglesum_pid = &chassis_anglesum_pid[CHAS_RB],
		
		.info_init = motor_info_init,
		.info_update = motor_info_update,
		.check = motor_check,
		.speed_pid_update = speed_pid_update,
		.angle_pid_update = angle_pid_update,
		.anglesum_pid_update = anglesum_pid_update,
			
	},
};

