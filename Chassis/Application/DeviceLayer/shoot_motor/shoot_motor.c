#include "shoot_motor.h"
motor_info_t shoot_motor_info[SHOOT_MOTOR_CNT]; 

pid_ctrl_t shoot_speed_pid[SHOOT_MOTOR_CNT];

pid_ctrl_t shoot_angle_pid[SHOOT_MOTOR_CNT];

pid_ctrl_t shoot_anglesum_pid[SHOOT_MOTOR_CNT];

motor_t shoot_motor[SHOOT_MOTOR_CNT] = {
	[SHOOT_LRUB] = {
		
		.info = &shoot_motor_info[SHOOT_LRUB],
		.speed_pid  = &shoot_speed_pid[SHOOT_LRUB],
		.angle_pid = &shoot_angle_pid[SHOOT_LRUB],
		.anglesum_pid = &shoot_anglesum_pid[SHOOT_LRUB],
	
		.info_init = motor_info_init,
		.info_update = motor_info_update,
		.check = motor_check,
		.speed_pid_update = speed_pid_update,
		.angle_pid_update = angle_pid_update,
		.anglesum_pid_update = anglesum_pid_update,
		
	},
	[SHOOT_RRUB] = {
		
		.info = &shoot_motor_info[SHOOT_RRUB],
		.speed_pid  = &shoot_speed_pid[SHOOT_RRUB],
		.angle_pid = &shoot_angle_pid[SHOOT_RRUB],
		.anglesum_pid = &shoot_anglesum_pid[SHOOT_RRUB],
		
		.info_init = motor_info_init,
		.info_update = motor_info_update,
		.check = motor_check,
		.speed_pid_update = speed_pid_update,
		.angle_pid_update = angle_pid_update,
		.anglesum_pid_update = anglesum_pid_update,
		
	},
	[SHOOT_DIAL] = {
		
		.info = &shoot_motor_info[SHOOT_DIAL],
		.speed_pid  = &shoot_speed_pid[SHOOT_DIAL],
		.angle_pid = &shoot_angle_pid[SHOOT_DIAL],
		.anglesum_pid = &shoot_anglesum_pid[SHOOT_DIAL],
			
		.info_init = motor_info_init,
		.info_update = motor_info_update,	
		.check = motor_check,
		.speed_pid_update = speed_pid_update,
		.angle_pid_update = angle_pid_update,
		.anglesum_pid_update = anglesum_pid_update,
		
	},
	[SHOOT_GUN] = {
		
		.info = &shoot_motor_info[SHOOT_GUN],
		.speed_pid  = &shoot_speed_pid[SHOOT_GUN],
		.angle_pid = &shoot_angle_pid[SHOOT_GUN],
		.anglesum_pid = &shoot_anglesum_pid[SHOOT_GUN],
				
		.info_init = motor_info_init,
		.info_update = motor_info_update,
		.check = motor_check,
		.speed_pid_update = speed_pid_update,
		.angle_pid_update = angle_pid_update,
		.anglesum_pid_update = anglesum_pid_update,
		
	},
};
