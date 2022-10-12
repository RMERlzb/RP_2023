#include "Imu_Task.h"
short ggx, ggy, ggz;
short aax, aay, aaz;
float pitch, roll, yaw;

//������ٶȵ��˲�
float filterout1, filterout2;

void StartImuTask(void const * argument)
{
  BMI_Init();
	
  for(;;)
  {
		BMI_Get_RawData (&ggx, &ggy, &ggz, &aax, &aay, &aaz);
		BMI_Get_EulerAngle (&pitch, &roll, &yaw, &ggx, &ggy, &ggz, &aax, &aay, &aaz);
	
		//�˲����ٶ�Z
		filterout1 = FilterRate * ggz + (1 - FilterRate) * filterout1;
		ggz = filterout1;
		
		//�˲����ٶ�Y
		filterout2 = FilterRate * ggy + (1 - FilterRate) * filterout2;
		ggy = filterout2;
		
    osDelay(1);
  }
 
}
