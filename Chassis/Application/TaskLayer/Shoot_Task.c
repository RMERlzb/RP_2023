#include "Shoot_Task.h"

void StartShoot_Task(void const * argument)
{
  
	Shoot_Init();
	
  for(;;)
  {
		if( rc_sensor.work_state == DEV_ONLINE )
		{
			
			Shoot();		
			
			Block_Manage();		
			
			Shoot_Send();
			
		}
		
    osDelay(1);
  }
 
}
