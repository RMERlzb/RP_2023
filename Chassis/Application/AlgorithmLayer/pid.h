#ifndef __PID_H
#define __PID_H
#include "rp_user_config.h"
#include "rp_math.h"


void pid_val_init(pid_ctrl_t *pid);
void single_pid_ctrl(pid_ctrl_t *pid,float err);
//void cascade_pid_ctrl(pid_ctrl_t *pid);

#endif
