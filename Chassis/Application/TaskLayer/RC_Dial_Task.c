#include "RC_Dial_Task.h"

char Mechanical_Mode = 0, Imu_Mode = 0, Gryo_Mode = 0;

void StartRC_DialTask(void const * argument)
{
  
  for(;;)
  {
		//�����ڻ�еģʽʱ
		if(rc_sensor.info->s1 == mec_mode)
		{
			Mechanical_Mode = 1;
			Imu_Mode = 0;
			Gryo_Mode = 0;
		}
		
		//������������ģʽʱ
		if(rc_sensor.info->s1 == imu_mode)
		{			
			//������С����ģʽ
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
