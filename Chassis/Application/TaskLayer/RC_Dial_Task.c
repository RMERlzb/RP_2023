#include "RC_Dial_Task.h"

char Mechanical_Mode = 0, Imu_Mode = 0, Gryo_Mode = 0;

void StartRC_DialTask(void const * argument)
{
  
  for(;;)
  {
		//拨码在机械模式时
		if(rc_sensor.info->s1 == mec_mode)
		{
			Mechanical_Mode = 1;
			Imu_Mode = 0;
			Gryo_Mode = 0;
		}
		
		//拨码在陀螺仪模式时
		if(rc_sensor.info->s1 == imu_mode)
		{			
			//拨码在小陀螺模式
			if( rc_sensor.info->thumbwheel >= 330 )	
				Gryo_Mode = 1;
			else
				Gryo_Mode = 0;
			
			Mechanical_Mode = 0;
			Imu_Mode = 1;	
	
		}
		
		if(rc_sensor.info->s1 == keyboard_mode)
		{
			Mechanical_Mode = 0;
			Imu_Mode = 0;
			Gryo_Mode = 0;
		}
		
    osDelay(1);
  }

}
