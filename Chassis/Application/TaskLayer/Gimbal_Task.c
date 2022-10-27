#include "Gimbal_Task.h"

void StartGimbal_Task(void const * argument)
{
  Gimbal_Init();
	
  for(;;)
  {
		if( RC_ONLINE )
		{
					
			
			if ( Mec_Return_flag == True )
			{
							
				//陀螺仪模式
				if ( rc_sensor_dial.Imu_Mode == True )			
					IMU_Mode();		
								
				//机械模式
				if ( rc_sensor_dial.Mechanical_Mode == True )
				{
					MEC_Mode();
				}
									
				//键盘模式下不需要在机械模式才开盖子		
				if ( rc_sensor_dial.Keyboard_Mode == True )
					Cover_State_Judge();
				
				Gimbal_Send();
						
			}
		}
		
    osDelay(1);
  }
 
}
