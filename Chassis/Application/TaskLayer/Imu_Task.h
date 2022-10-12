#ifndef __Imu_Task_H__
#define __Imu_Task_H__

#include "stm32f4xx.h"                  // Device header
#include "string.h"
#include "cmsis_os.h"
#include "pid.h"
#include "bmi.h"
#include "gimbal.h"
#include "rp_user_define.h"
/*
	�������ȼ�������ͨ����
	
	���ʼ�����������豸
	��Z��Y�Ľ��ٶȽ������˲�
	���������һֱ�õ�ŷ���Ǻ��������ٶ�
	����6020����и��Ե�imu_pid_t���͵�PID��������PID�Ĳ�����ֵ
	
	ͨ�������ǵĽǶȺͽ��ٶȽ����𴮼�PID�����������������Ӧ��6020���
	�ܹ�������̨����ָ����yaw�ǶȺ�pitch�Ƕ�
*/
extern short ggx, ggy, ggz;
extern short aax, aay, aaz;
extern float pitch, roll, yaw;

extern void StartImuTask(void const * argument);
	
#endif
