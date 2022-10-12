#ifndef __RC_Heart_Task_H__
#define __RC_Heart_Task_H__

#include "stm32f4xx.h"                  // Device header
#include "cmsis_os.h"
#include "rc_sensor.h"
#include "chassis.h"
#include "rp_user_define.h"
#include "gimbal.h"  
#include "RC_Dial_Task.h"
/*
	�������ȼ�������ͨ
	tick������500ms��һֱ�����̺���̨����0����
	һֱ���ң�����Ƿ�ϵ磬����ϵ��ˣ���Chassis_Task�����л�ֹͣ���̺���̨�Ŀ���
	ң�����ϵ�����̶�ȡϵͳʱ��ֵ�������õ���Ŀ��ֵΪ0���м��٣���̨��Ŀ��ֵ��0
	500ms��һֱ��������0
*/
extern char Mec_Return_flag;
void StartRC_HeartTask(void const * argument);

#endif
