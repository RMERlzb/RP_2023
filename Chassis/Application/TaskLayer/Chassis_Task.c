#include "Chassis_Task.h"
/*
	左拨码依次往下：1 3 2
	左拨码在中间：陀螺仪模式---->s1=3
	左拨码在下面：机械模式------>s1=2
*/
void StartChassis_Task(void const * argument)
{	
	Chassis_Init();
	

  for(;;)
  {
		
		if( RC_ONLINE )	
		{
			
				Chassis_MEC_Input();	
							
				Chassis_send();							

			
		}
			
		osDelay(1);
		
	}	
	
}

void StartWorkStateTask(void const * argument)
{
  for(;;)
  {
    osDelay(1);
  }
}
