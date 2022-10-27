#include "RC_Dial_Task.h"

void StartRC_DialTask(void const * argument)
{
  for(;;)
  {
		
		if( RC_ONLINE )
		{
				
			if( rc_sensor.dial->Keyboard_Mode == True )
				KB_CTRL();
				
			rc_sensor.dial_jugde(&rc_sensor);
			
		}
		

    osDelay(1);
  }

}
