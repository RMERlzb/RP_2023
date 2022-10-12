#ifndef __CHASSIS_MOTOR_H
#define __CHASSIS_MOTOR_H

#include "main.h"
#include "stm32f4xx.h" 
#include "rp_device_config.h"
#include "rp_user_config.h"
#include "pid.h"
/*
	chassis_motor_info_t�ṹ��
		���������Ϣ
		
	chassis_motor_t�ṹ��
		���������Ϣ�ṹ�壬����ָ�룬ID����һ��3508�ٶȵ�PID
		
	chassis_motor_init����
		�Ե����Ϣ���и�ֵ0
		
	chassis_motor_update����
		���յ������Ӧ�����Ϣ���б���
		
	chassis_motor_check����
		������Ƿ�������
		
	ע�⣺
		����һ��3508�ٶ�PID������
		����һ��3508�����Ϣ����
		����һ��3508�������
	
	�����������Ϊ���������ֵ		
	typedef enum {
		CHAS_LF,
		CHAS_RF,
		CHAS_LB,
		CHAS_RB,
		CHAS_MOTOR_CNT,
	} chassis_motor_cnt_t;

	���������ʱ��֪����ɶ�ã������ڸ�������ĳ�ʼ���л�������и�ֵ--->	dev_id_t	 id;
	typedef enum {
		DEV_ID_RC = 0,
		DEV_ID_IMU = 1,
		DEV_ID_CHASSIS_LF = 2,
		DEV_ID_CHASSIS_RF = 3,
		DEV_ID_CHASSIS_LB = 4,
		DEV_ID_CHASSIS_RB = 5,
		DEV_ID_CNT = 6,
	} dev_id_t;
	ע�⣺
		������Ϣָ�롢PIDָ�롢����ָ��ĸ�ֵ
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
