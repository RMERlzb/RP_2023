#ifndef __gimbal_H__
#define __gimbal_H__
#include "stm32f4xx.h"                  // Device header
#include "gimbal_motor.h"
#include "pid.h"
#include "can_protocol.h"
#include "drv_can.h"
#include "string.h"
#include "rp_math.h"
#include "Imu_Task.h"
#include "chassis.h"
#include "rp_user_define.h"


typedef struct imu_pid_struct{
	pid_ctrl_t* 	imu_pid_angle;
	pid_ctrl_t* 	imu_pid_velocity;
}imu_pid_t;

typedef struct machinery_pid_struct{
	pid_ctrl_t*  gimbal_pid_angle;
	pid_ctrl_t*	 imu_pid_velocity;
}machinery_pid_t;


extern machinery_pid_t  machinery_pid_Y;
extern imu_pid_t imu_pid_Z;

void imu_pid_init(void);
void machinery_pid_init(void);
void Gimbal_Init(void);

void Gimbal_Send(void);

char Mechanical_Return(void);
void IMU_Mode(void);
void MEC_Mode(void);
void GYRO_Mode(void);
#endif
