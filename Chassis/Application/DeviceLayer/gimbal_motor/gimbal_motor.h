#ifndef __gimbal_motor_H__
#define __gimbal_motor_H__
#include "stm32f4xx.h"                  // Device header
#include "rp_device_config.h"
#include "pid.h"
/*
	gimbal_motor_info_t结构体
		包含电机信息
		
	gimbal_motor_t结构体
		包含电机信息结构体，函数指针，ID，和一个角度位置的PID
		
	gimbal_motor_init函数
		对电机信息进行赋值0
		
	gimbal_motor_update函数
		对收到的相对应电机信息进行保存
		
	gimbal_motor_check函数
		检查电机是否工作正常
		
	注意：
		会有一个角度位置PID数组
		会有一个6020电机信息数组
		会有一个6020电机数组
		
	下面这个是作为数组的索引值	
	typedef enum {
		GIMBAL_LOW, 
		GIMBAL_HIGH, 
		GIMBAL_MOTOR_CNT,
	} gimbal_motor_cnt_t;
	
	下面这个暂时不知道有啥用，但是在各个电机的初始化中会对它进行赋值--->	dev_gimbal_id_t	 id;
	typedef enum {
		DEV_ID_GIMBAL_LOW=1,
		DEV_ID_GIMBAL_HIGH=2,
	} dev_gimbal_id_t;

	注意：
		进行信息指针、PID指针、函数指针的赋值
*/
typedef struct gimbal_motor_info_struct{
	uint16_t		angle;
	int16_t	  	speed;
	int16_t 		current;
  uint8_t   	temperature;
	uint16_t		angle_prev;
	int32_t			angle_sum;
	uint8_t  		offline_cnt;
	uint8_t			offline_max_cnt;	
	dev_work_state_t   		work_state;
}gimbal_motor_info_t;

typedef struct gimbal_motor_struct{
	gimbal_motor_info_t 	*info;
	pid_ctrl_t*       		pid;
	dev_gimbal_id_t				id;
	void 	(*init)(struct gimbal_motor_struct *motor);
	void  (*update)(struct gimbal_motor_struct *motor, uint8_t *rxBuf);
	void  (*check)(struct gimbal_motor_struct *motor);
}gimbal_motor_t;

extern gimbal_motor_t gimbal_motor[GIMBAL_MOTOR_CNT];
#endif
