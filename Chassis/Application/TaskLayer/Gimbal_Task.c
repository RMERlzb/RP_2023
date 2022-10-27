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
							
				//������ģʽ
				if ( rc_sensor_dial.Imu_Mode == True )			
					IMU_Mode();		
								
				//��еģʽ
				if ( rc_sensor_dial.Mechanical_Mode == True )
				{
					MEC_Mode();
				}
									
				//����ģʽ�²���Ҫ�ڻ�еģʽ�ſ�����		
				if ( rc_sensor_dial.Keyboard_Mode == True )
					Cover_State_Judge();
				
				Gimbal_Send();
						
			}
		}
		
    osDelay(1);
  }
 
}
