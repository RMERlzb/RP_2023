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
							
				//������ģʽ
				if ( rc_sensor_dial.Imu_Mode == True )			
					IMU_Mode();		
								
				//��еģʽ
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
