#include "Shoot_Task.h"

void StartShoot_Task(void const * argument)
{
  
	Shoot_Init();
	
  for(;;)
  {
		if( RC_ONLINE )
		{
			
			Shoot();	
		
			
			Shoot_Send();
			
		}
		
    osDelay(1);
  }
 
}
