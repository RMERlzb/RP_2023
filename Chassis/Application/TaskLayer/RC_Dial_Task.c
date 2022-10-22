#include "RC_Dial_Task.h"

void StartRC_DialTask(void const * argument)
{
  for(;;)
  {
		
		if( rc_sensor.work_state == DEV_ONLINE )
			rc_sensor.dial_jugde(&rc_sensor);
		
    osDelay(1);
  }

}
