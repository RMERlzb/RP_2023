#include "gimbal_motor.h"

motor_info_t gimbal_motor_info[GIMBAL_MOTOR_CNT]; 

pid_ctrl_t gimbal_speed_pid[GIMBAL_MOTOR_CNT];

pid_ctrl_t gimbal_angle_pid[GIMBAL_MOTOR_CNT];

pid_ctrl_t gimbal_anglesum_pid[GIMBAL_MOTOR_CNT];

motor_t gimbal_motor[GIMBAL_MOTOR_CNT] = {
	[GIMBAL_LOW] = {
		
		.info = &gimbal_motor_info[GIMBAL_LOW],
		.speed_pid = &gimbal_speed_pid[GIMBAL_LOW],
		.angle_pid = &gimbal_angle_pid[GIMBAL_LOW],
		.anglesum_pid = &gimbal_anglesum_pid[GIMBAL_LOW],
		
		.info_init = motor_info_init,
		.info_update = motor_info_update,
		.check = motor_check,
		.speed_pid_update = speed_pid_update,
		.angle_pid_update = angle_pid_update,
		.anglesum_pid_update = anglesum_pid_update,
		
		.dir = Dir_Header,
	},
	[GIMBAL_HIGH] = {
		
		.info = &gimbal_motor_info[GIMBAL_HIGH],
		.speed_pid  = &gimbal_speed_pid[GIMBAL_HIGH],
		.angle_pid = &gimbal_angle_pid[GIMBAL_HIGH],
		.anglesum_pid = &gimbal_anglesum_pid[GIMBAL_HIGH],
		
		.info_init = motor_info_init,
		.info_update = motor_info_update,
		.check = motor_check,
		.speed_pid_update = speed_pid_update,
		.angle_pid_update = angle_pid_update,
		.anglesum_pid_update = anglesum_pid_update,
		
		.dir = Dir_Header,
	},
};
