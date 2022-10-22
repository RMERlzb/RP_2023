#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx.h" 
#include "rp_device_config.h"
#include "rp_math.h"
#include "rp_user_define.h"

void motor_info_init(motor_t *motor);
void motor_info_update(motor_t *motor, uint8_t *rxbuf);
void motor_check(motor_t *motor);

void speed_pid_update(motor_t *motor);
void angle_pid_update(motor_t *motor);
void anglesum_pid_update(motor_t *motor);

#endif
