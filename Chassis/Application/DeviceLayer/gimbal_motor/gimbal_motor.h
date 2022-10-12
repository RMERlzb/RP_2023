#ifndef __gimbal_motor_H__
#define __gimbal_motor_H__
#include "stm32f4xx.h"                  // Device header
#include "rp_device_config.h"
#include "pid.h"
/*
	gimbal_motor_info_t�ṹ��
		���������Ϣ
		
	gimbal_motor_t�ṹ��
		���������Ϣ�ṹ�壬����ָ�룬ID����һ���Ƕ�λ�õ�PID
		
	gimbal_motor_init����
		�Ե����Ϣ���и�ֵ0
		
	gimbal_motor_update����
		���յ������Ӧ�����Ϣ���б���
		
	gimbal_motor_check����
		������Ƿ�������
		
	ע�⣺
		����һ���Ƕ�λ��PID����
		����һ��6020�����Ϣ����
		����һ��6020�������
		
	�����������Ϊ���������ֵ	
	typedef enum {
		GIMBAL_LOW, 
		GIMBAL_HIGH, 
		GIMBAL_MOTOR_CNT,
	} gimbal_motor_cnt_t;
	
	���������ʱ��֪����ɶ�ã������ڸ�������ĳ�ʼ���л�������и�ֵ--->	dev_gimbal_id_t	 id;
	typedef enum {
		DEV_ID_GIMBAL_LOW=1,
		DEV_ID_GIMBAL_HIGH=2,
	} dev_gimbal_id_t;

	ע�⣺
		������Ϣָ�롢PIDָ�롢����ָ��ĸ�ֵ
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
