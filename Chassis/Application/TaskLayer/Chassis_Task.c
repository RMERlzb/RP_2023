#include "Chassis_Task.h"
/*
	�����������£�1 3 2
	�������м䣺������ģʽ---->s1=3
	���������棺��еģʽ------>s1=2
*/
void StartRC_CtlTask(void const * argument)
{	
	Chassis_Init();
	Gimbal_Init();
	imu_pid_init();
	machinery_pid_init();
	
  for(;;)
  {
		
//		if( rc_sensor.work_state == DEV_ONLINE )	
//		{
//			
//			if ( Mec_Return_flag )
//			{
//							
//				//������ģʽ
//				if ( Imu_Mode )			
//					IMU_Mode();		
//								
//				//��еģʽ
//				else if ( Mechanical_Mode )
//					MEC_Mode();										
//		
//				Chassis_MEC_Input();	
//							
//				Chassis_send();				
//				
//				Gimbal_Send();
//						
//			}
			
//		}
			
		osDelay(1);
		
	}	
	
}

void StartWorkStateTask(void const * argument)
{
  for(;;)
  {
    osDelay(1);
  }
}
