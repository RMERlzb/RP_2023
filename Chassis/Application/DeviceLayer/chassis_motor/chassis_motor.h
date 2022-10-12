#ifndef __CHASSIS_MOTOR_H
#define __CHASSIS_MOTOR_H

#include "main.h"
#include "stm32f4xx.h" 
#include "rp_device_config.h"
#include "rp_user_config.h"
#include "pid.h"
/*
	chassis_motor_info_t结构体
		包含电机信息
		
	chassis_motor_t结构体
		包含电机信息结构体，函数指针，ID，和一个3508速度的PID
		
	chassis_motor_init函数
		对电机信息进行赋值0
		
	chassis_motor_update函数
		对收到的相对应电机信息进行保存
		
	chassis_motor_check函数
		检查电机是否工作正常
		
	注意：
		会有一个3508速度PID的数组
		会有一个3508电机信息数组
		会有一个3508电机数组
	
	下面这个是作为数组的索引值		
	typedef enum {
		CHAS_LF,
		CHAS_RF,
		CHAS_LB,
		CHAS_RB,
		CHAS_MOTOR_CNT,
	} chassis_motor_cnt_t;

	下面这个暂时不知道有啥用，但是在各个电机的初始化中会对它进行赋值--->	dev_id_t	 id;
	typedef enum {
		DEV_ID_RC = 0,
		DEV_ID_IMU = 1,
		DEV_ID_CHASSIS_LF = 2,
		DEV_ID_CHASSIS_RF = 3,
		DEV_ID_CHASSIS_LB = 4,
		DEV_ID_CHASSIS_RB = 5,
		DEV_ID_CNT = 6,
	} dev_id_t;
	注意：
		进行信息指针、PID指针、函数指针的赋值
*/
typedef struct chassis_motor_info_struct{
	uint16_t		angle;
	int16_t	  	speed;
	int16_t 		current;
  uint8_t   	temperature;
	uint16_t		angle_prev;
	int32_t			angle_sum;
	uint8_t  		offline_cnt;
	uint8_t			offline_max_cnt;	
	dev_work_state_t   		work_state;
} chassis_motor_info_t;

typedef struct chassis_motor_struct{
	chassis_motor_info_t   *info;
	pid_ctrl_t*       			pid;
	dev_errno_t							errno;
	dev_id_t								id;
	void 	(*init)(struct chassis_motor_struct *motor);
	void  (*update)(struct chassis_motor_struct *motor,uint8_t *rxBuf);
	void  (*check)(struct chassis_motor_struct *motor);
} chassis_motor_t;
extern chassis_motor_t chassis_motor[CHAS_MOTOR_CNT];
#endif
