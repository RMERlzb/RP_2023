#include "Chassis_Task.h"
/*
	左拨码依次往下：1 3 2
	左拨码在中间：陀螺仪模式---->s1=3
	左拨码在下面：机械模式------>s1=2
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
//				//陀螺仪模式
//				if ( Imu_Mode )			
//					IMU_Mode();		
//								
//				//机械模式
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
