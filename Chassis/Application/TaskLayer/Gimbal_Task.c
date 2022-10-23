#include "Gimbal_Task.h"

void StartGimbal_Task(void const * argument)
{
  Gimbal_Init();
	
  for(;;)
  {
		if( rc_sensor.work_state == DEV_ONLINE )
		{
		
			
			
			if ( Mec_Return_flag == True )
			{
							
				//陀螺仪模式
				if ( rc_sensor_dial.Imu_Mode == True )			
					IMU_Mode();		
								
				//机械模式
				else if ( rc_sensor_dial.Mechanical_Mode == True )
				{
					MEC_Mode();
				}
										
			
				
				Gimbal_Send();
						
			}
		}
		
    osDelay(1);
  }
 
}
