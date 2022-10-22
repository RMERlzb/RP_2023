#include "motor.h"

void motor_info_init(motor_t *motor)
{
	(motor->info)->angle = 0;
	(motor->info)->current = 0;
	(motor->info)->speed = 0;
	(motor->info)->temperature = 0;
	(motor->info)->offline_max_cnt = 50;
	(motor->info)->offline_cnt = 0;
	motor->work_state = DEV_ONLINE;
}

void motor_info_update(motor_t *motor, uint8_t *rxbuf)
{
	(motor->info)->angle = (int16_t)rxbuf[0]<<8 | rxbuf[1];
	(motor->info)->speed = (int16_t)rxbuf[2]<<8 | rxbuf[3];
	(motor->info)->current = (int16_t)rxbuf[4]<<8 | rxbuf[5];
	(motor->info)->temperature = rxbuf[6];
	(motor->info)->offline_cnt = 0;
}	


void motor_check(motor_t *motor)
{
	(motor->info)->offline_cnt++;
	
	if( (motor->info)->offline_cnt > (motor->info)->offline_max_cnt )
	{
		(motor->info)->offline_cnt = (motor->info)->offline_max_cnt;
		motor->work_state = DEV_OFFLINE;
	}
	else{
		if( motor->work_state == DEV_OFFLINE )
		{
			motor->work_state = DEV_ONLINE;		
		}
	}		
}

void speed_pid_update(motor_t *motor)
{
	//速度目标值在别的地方赋值
	
	motor->speed_pid->measure = motor->info->speed;
	
	motor->speed_pid->err = motor->speed_pid->target - motor->speed_pid->measure;
}

void angle_pid_update(motor_t *motor)
{
	
	motor->angle_pid->measure = motor->info->angle;
	
	motor->info->angle_prev = motor->info->angle;
	
	//过零点处理
	RampFloat( motor->angle_pid->target, motor->angle_pid->measure, \
						RMmotor_angle_max / 2, &motor->angle_pid->err );
	
}

void anglesum_pid_update(motor_t *motor)
{
	float temp_err = 0;
	
	//先对前后两次角度误差进行保存和过零点处理	
	RampFloat( motor->info->angle, motor->info->angle_prev, \
						RMmotor_angle_max / 2, &temp_err );
	
	//加到测量值上
	motor->info->angle_sum += temp_err;
	
	motor->info->angle_prev = motor->info->angle;		
	
	motor->anglesum_pid->measure = motor->info->angle_sum;
	
	//角度总和PID的误差就是目标总角度和当前总角度之间的差值
	motor->anglesum_pid->err = motor->anglesum_pid->target - motor->anglesum_pid->measure;
																					
}
