#include "Chassis_Task.h"
/*
	�����������£�1 3 2
	�������м䣺������ģʽ---->s1=3
	���������棺��еģʽ------>s1=2
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
