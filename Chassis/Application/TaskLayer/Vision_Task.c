#include "Vision_Task.h"
void StartVision_Task(void const * argument)
{
 
  for(;;)
  {
		vision_sensor.heart_beat(&vision_sensor);
		
    osDelay(1);
  }
  
}
