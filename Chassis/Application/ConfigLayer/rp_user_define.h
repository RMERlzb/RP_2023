#ifndef __RP_USER_DEFINE_H__
#define __RP_USER_DEFINE_H__

#include "stm32f4xx.h"                  // Device header

//��������yawֵ+     ����yawֵ��   
//��̨����pitchֵ-   ����pitchֵ+ 
//6020����������ֵ����ʱ��ת������������ֵ��˳ʱ��ת


//��������ٶȺͳ�ʼ�ٶ����ӱ���
#define Chassis_VMax  		8000
#define V_Rate					Chassis_VMax/660

//��̨��pitch����ֵ
#define gimbal_pitchmax  22.5f
#define gimbal_pitchmin  -38.5f

//�����������Z���Y��Ļ�е��ֵ
#define mechanical_Z  				2050
#define mechanical_Y  				6800
#define tail_mechanical				6050


//RM������Ƕ�
#define RMmotor_angle_max  		8192

//pitch�����6020����ļ��޵ĽǶ�
#define gimbal_angle_Ymax  		7300		//ͷ���ŵ�ʱ��
#define gimbal_angle_Ymin  		5900		//ͷ̧�ŵ�ʱ��

//CAN���͵ĵ��ָ��
#define Gimbal_Control_ID 		0x1ff
#define Chassis_Control_ID 		0x200
#define Shoot_Control_ID			0x200

//����λ�ö�Ӧ��ģʽֵ(�������� 1---3---2)
					//��
#define keyboard_mode		1
#define imu_mode 				3
#define mec_mode 				2


	//������ģʽ�µ��Ҳ���
#define continue_shot_mode			1
#define version_control_mode    3
#define fricwheel_state_trans		2	

	//��еģʽ�µ��Ҳ���
#define single_shot_mode				1
#define cover_state_trans				2

//С����ģʽ��speed_z�ĸ�ֵ
#define Gryo_Speed_Z    250

//�Ƕ�8192ת��Ϊ360��ı���
#define Mec_To_Angle_Rate		( RMmotor_angle_max / 360 )   //22.75���

//����ת������
#define Radian_Rate 57.f

//��ͨ�˲�����
#define	FilterRate  0.2f

//ǹ���ӵ����������ٶ�
#define Shoot_Vmax	2000	
#endif
