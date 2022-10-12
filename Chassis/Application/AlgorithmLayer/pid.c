#include "pid.h"
void pid_val_init(pid_ctrl_t *pid)
{
	pid->target = 0;
//	pid->measure = 0;
	pid->kp=0;
	pid->ki=0;
	pid->kd=0;
	pid->err=0;
	pid->last_err=0;
	pid->integral=0;
	pid->pout=0;
	pid->iout=0;
	pid->dout=0;
	pid->out=0;
	pid->integral_max=1000;
	pid->out_max=8000;
}

void single_pid_ctrl(pid_ctrl_t *pid,float err)
{
	// 保存误差值(需要在外面自行计算误差)
	pid->err = err;
	// 积分
	pid->integral += pid->err;
	pid->integral = constrain(pid->integral, -pid->integral_max, +pid->integral_max);
	// p i d 输出项计算
	pid->pout = pid->kp * pid->err;
	pid->iout = pid->ki * pid->integral;
	pid->dout = pid->kd * (pid->err - pid->last_err);
	// 累加pid输出值
	pid->out = pid->pout + pid->iout + pid->dout;
	pid->out = constrain(pid->out, -pid->out_max, pid->out_max);
	// 记录上次误差值
	pid->last_err = pid->err;
}
