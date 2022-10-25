#include "Judge_Task.h"
void StartJudge_Task(void const * argument)
{

  for(;;)
  {
		judge_sensor.heart_beat(&judge_sensor);
		
		slave_heart_beat();
		
		if( JUDGE_ONLINE && Dashboard_Position == Dashboard_Low )
			Down_Send(&judge_sensor);
		
    osDelay(1);
  }
 
}
