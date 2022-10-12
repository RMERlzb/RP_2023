#ifndef __RP_USER_DEFINE_H__
#define __RP_USER_DEFINE_H__

#include "stm32f4xx.h"                  // Device header

//��������yawֵ+     ����yawֵ��   
//��̨����pitchֵ-   ����pitchֵ+ 
//6020����������ֵ����ʱ��ת������������ֵ��˳ʱ��ת


//������ģʽ�½Ƕ������仯�ʣ�ע���Ǹ���
#define imu_angle_target_change_rate  -0.0005f

//��еģʽ��pitch�������ĽǶ�Ŀ�������仯��
#define mec_angle_target_change_rate  -0.01f

//��������ٶȺͳ�ʼ�ٶ����ӱ���
#define V_Max  					8000
#define V_Rate					V_Max/660

//��̨��pitch����ֵ
#define gimbal_pitchmax  22.5f
#define gimbal_pitchmin  -38.5f

//�����������Z���Y��Ļ�е��ֵ
#define mechanical_Z  				2050
#define mechanical_Y  				6800

//6020������ĽǶ�
#define gimbal_angle_max  		8192

//pitch�����6020����ļ��޵ĽǶ�
#define gimbal_angle_Ymax  		7300		//ͷ���ŵ�ʱ��
#define gimbal_angle_Ymin  		5900		//ͷ̧�ŵ�ʱ��

//CAN���͵ĵ��ָ��
#define Gimbal_Control_ID 		0x1ff
#define Chassis_Control_ID 		0x200
#define Shoot_Control_ID			0x200

//����λ�ö�Ӧ��ģʽֵ
					//��
#define imu_mode 				3
#define mec_mode 				2
#define keyboard_mode		1
	
//С����ģʽ��speed_z�ĸ�ֵ
#define Gryo_Speed_Z    300

//����̨�Ƕ�8192ת��Ϊ360��ı���
#define Mec_To_Angle_Rate		( gimbal_angle_max / 360 )

//����ת������
#define Radian_Rate 57.f

//��ͨ�˲�����
#define	FilterRate  0.2f

#endif
