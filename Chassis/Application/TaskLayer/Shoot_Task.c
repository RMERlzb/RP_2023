#include "Shoot_Task.h"
int16_t b = 0;
void StartShootTask(void const * argument)
{
  
  for(;;)
  {
	b++;
	b %= 1000;
		
		shoot_txbuf[0] = b>>8;
		shoot_txbuf[1] = b;
		
		shoot_txbuf[2] = -b>>8;
		shoot_txbuf[3] = -b;
		
		shoot_txbuf[4] = -b>>8;
		shoot_txbuf[5] = -b;

		CAN_Send_Msg (&CAN2_TxMsg , Shoot_Control_ID , shoot_txbuf);
	
		memset (shoot_txbuf, 0, sizeof(shoot_txbuf) );
		
    osDelay(1);
  }
 
}
