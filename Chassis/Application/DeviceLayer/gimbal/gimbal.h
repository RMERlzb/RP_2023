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
#include "vision_sensor.h"
#include "rp_user_define.h"
#include "rc_sensor.h"
#include "Servo.h"

typedef struct imu_pid_struct{
	pid_ctrl_t* 	imu_angle_pid;
	pid_ctrl_t* 	imu_speed_pid;
} imu_pid_t;

typedef struct machinery_pid_struct{
	pid_ctrl_t*  gimbal_angle_pid;
	pid_ctrl_t*	 imu_speed_pid;
} machinery_pid_t;

extern machinery_pid_t  machinery_pid_Z;
extern machinery_pid_t  machinery_pid_Y;

extern imu_pid_t imu_pid_Y;
extern imu_pid_t imu_pid_Z;

extern float chassis_input;


//ʧ����λ
void Gimbal_Reset(void);

//������ģʽ
void imu_pid_init(void);
void imu_pid_cal(void);
void Chassis_Input(void);
void IMU_Mode(void);

//��еģʽ
void machinery_pid_init(void);
void mec_pid_cal(void);
void MEC_Mode(void);

//��ʼ��
void gimbal_self_init(void);
void Gimbal_Init(void);

//��������
void Gimbal_Send(void);

char Mechanical_Return(void);

void Cover_State_Judge(void);	
#endif
