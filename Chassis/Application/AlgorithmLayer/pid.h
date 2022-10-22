#ifndef __PID_H
#define __PID_H
#include "rp_user_config.h"
#include "rp_math.h"
#include "rp_device_config.h"
#include "motor.h"

void pid_val_init(pid_ctrl_t *pid);
void single_pid_ctrl(pid_ctrl_t *pid,float err);
void single_speedpid_cal(motor_t* motor);
void single_anglepid_cal(motor_t* motor);
void single_anglesum_pid_cal(motor_t* motor);

#endif
