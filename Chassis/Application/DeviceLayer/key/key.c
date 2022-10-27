#include "key.h"


//*--------------------------ǰ������-----------------------*//
void Key_W(void)
{
  KEY_State_Judge(&Keyboard.W ,KEY_W , CHANGE_TIM , LONG_CHANGE_TIM_W);
  switch(Keyboard.W.State)
  {
    case UP:			
      break;
    case PRESS:
      break;
    case SHORT_DOWN:			
      break;
    case DOWN:
      break;
    case RELAX:
      break;
  }
}

void Key_S(void)
{
  KEY_State_Judge(&Keyboard.S ,KEY_S, CHANGE_TIM , LONG_CHANGE_TIM_S);
  switch(Keyboard.S.State)
  {
    case UP:
      break;
    case PRESS:
      break;
    case SHORT_DOWN:
      break;
    case DOWN:
      break;
    case RELAX:
      break;
  }
}

void Key_D(void)
{
  KEY_State_Judge(&Keyboard.D ,KEY_D, CHANGE_TIM , LONG_CHANGE_TIM_D);
  switch(Keyboard.D.State)
  {
    case UP:
      break;
    case PRESS:
      break;
    case SHORT_DOWN:
      break;
    case DOWN:
      break;
    case RELAX:		
      break;
  }
}

void Key_A(void)
{
  KEY_State_Judge(&Keyboard.A ,KEY_A, CHANGE_TIM , LONG_CHANGE_TIM_A);
  switch(Keyboard.A.State)
  {
    case UP:
      break;
    case PRESS:
      break;
    case SHORT_DOWN:
      break;
    case DOWN:
      break;
    case RELAX:	
      break;
  }
}

//----------------------Ѹ�ٵ�ͷ��������ģʽ��--------------------------------//
void Key_C(void)
{
	
  KEY_State_Judge(&Keyboard.C ,KEY_C, CHANGE_TIM , LONG_CHANGE_TIM_C);
  switch(Keyboard.C.State)
  {
 		case UP:		
      break;
    case PRESS:
      break;
		
    case SHORT_DOWN://------------������ģʽ�²���Ѹ�ٵ�ͷ
			if( rc_sensor_dial.Imu_Mode == True && rc_sensor_dial.This_Gryo_Mode == Flase )
			{
				if( imu_pid_Z.imu_angle_pid->target >= 0 )//----Ŀǰ����0
					imu_pid_Z.imu_angle_pid->target -= 180;
				
				else		//------Ŀǰ����0
					imu_pid_Z.imu_angle_pid->target += 180;
				
			}
				  
      break;
			
			
    case DOWN:
      break;
    case RELAX:
      break;
  }
}



//----------------------------��ס���-------------------------------//

void Key_SHIFT(void)
{
	
  KEY_State_Judge(&Keyboard.SHIFT ,KEY_SHIFT, CHANGE_TIM , LONG_CHANGE_TIM_SHIFT);
	
  switch(Keyboard.SHIFT.State)
  {
    case UP:			
      break;
		
    case PRESS:
      break;
		
    case SHORT_DOWN:		
      break;  
		
    case DOWN://--------------����
			if( rc_sensor.info->Ch[3] >= 0 )
				rc_sensor.info->Ch[3] = CH_MAX;
			else
				rc_sensor.info->Ch[3] = CH_MIN;
      break;
		
    case RELAX:
      break;
  }
}




//---------------------------����-------------------------------------//
uint32_t b_1,b_2;//ʱ�������־λ �������B�������µļ��ʱ��
void Key_B(void)
{
  KEY_State_Judge(&Keyboard.B ,KEY_B, CHANGE_TIM , LONG_CHANGE_TIM_B);
  switch(Keyboard.B.State)
  {
	
    case UP:				
      break;
		
		
    case PRESS:		
			B_FLAG++;			
		
			if(  B_FLAG%2 )b_1 = HAL_GetTick();
			if(!(B_FLAG%2))b_2 = HAL_GetTick();	
		
			if(abs(b_2 - b_1) < 500) //-----------�����ص���
				rc_sensor_dial.Cover_OpenEnable = Flase;
			
		  else  //------------------˫�������գ�����ͷ����ƽ
			{
				rc_sensor_dial.Cover_OpenEnable = True;
				imu_pid_Y.imu_angle_pid->target = 0;
				machinery_pid_Z.gimbal_angle_pid->target = mechanical_Y;			
			}
			
      break;
		
			
			
    case SHORT_DOWN:
      break;
    case DOWN:
      break;
    case RELAX:
      break;
  }
	
	//���մ򿪣��˶��ٶȼ���
	if( rc_sensor_dial.Cover_OpenEnable == True )
	{
		//���ڵ��̣�ԭ���������޷�Ϊ8000
		for (int i = 0; i < 4; i ++)
			chassis_motor[i].speed_pid->out_max = Chassis_OutMax * 0.5f;
		
		//������̨��ԭ���������޷�Ϊ20000
		imu_pid_Z.imu_speed_pid->out_max = Gimbal_OutMax * 0.5f;
			
		imu_pid_Y.imu_speed_pid->out_max = Gimbal_OutMax * 0.5f;

		machinery_pid_Z.imu_speed_pid->out_max = Gimbal_OutMax * 0.5f;
		
		machinery_pid_Y.imu_speed_pid->out_max = Gimbal_OutMax * 0.5f;
		
	}
	
	//������չرգ��ָ��˶��ٶ�
	else
	{
		//���ڵ��̣�ԭ���������޷�Ϊ8000
		for (int i = 0; i < 4; i ++)
			chassis_motor[i].speed_pid->out_max = Chassis_OutMax;
		
		//������̨��ԭ���������޷�Ϊ20000
		imu_pid_Z.imu_speed_pid->out_max = Gimbal_OutMax;
			
		imu_pid_Y.imu_speed_pid->out_max = Gimbal_OutMax;

		machinery_pid_Z.imu_speed_pid->out_max = Gimbal_OutMax;
		
		machinery_pid_Y.imu_speed_pid->out_max = Gimbal_OutMax;
	}	
}


//----------------------�򿪹ر�Ħ����---------------------//
void Key_R(void)
{
  KEY_State_Judge(&Keyboard.R ,KEY_R, CHANGE_TIM , LONG_CHANGE_TIM_R);
	
	
  switch(Keyboard.R.State)
  {
    case UP:			
      break;
		
    case PRESS:	
      break;
		
    case SHORT_DOWN:		
      break;
		
    case DOWN:  //--------------------����������
			rc_sensor_dial.FricWheel_WorkEnable = !rc_sensor_dial.FricWheel_WorkEnable;
      break;
		
    case RELAX:
      break;
  }
}

//--------------------------С����ģʽ------------------------------//
float Keyboard_SpeedZ = 0;
uint16_t PressCnt = 0, time_cnt = 0;//��������ʵ��
void Key_F(void)
{
	time_cnt++;
	time_cnt %= 21;//���һ���������
	
  KEY_State_Judge(&Keyboard.F ,KEY_F, CHANGE_TIM , LONG_CHANGE_TIM_F);
  switch(Keyboard.F.State)
  {
    case UP: //̧��״̬
      break;
		
    case PRESS: //����˲��
      break;
		
    case SHORT_DOWN: //�̰�
			rc_sensor_dial.This_Gryo_Mode = True;
			PressCnt ++;
			PressCnt %= 3;				
      break;
		
    case DOWN: //����
      break;
		
    case RELAX: //�ɿ�˲��
      break;
  }
	
	if( rc_sensor_dial.This_Gryo_Mode == True )
	{
		if( PressCnt == 0 )//---------����
			Keyboard_SpeedZ = Gryo_Speed_ZMin;
				
		else if( PressCnt == 1 ) //-------����
			Keyboard_SpeedZ = Gryo_Speed_ZMax;
				
		else	if( PressCnt == 2 )	//------------����
			Keyboard_SpeedZ = 200 * sin( time_cnt * 3.1415926f / 10 );	
	}
	
	else
		time_cnt = PressCnt = 0;
	
}



//-----------------��������еģʽ���̰���������ģʽ-------------------------//
void Key_ctrl(void)
{
  KEY_State_Judge(&Keyboard.CTRL ,KEY_CTRL, CHANGE_TIM , LONG_CHANGE_TIM_CTRL);
  switch(Keyboard.CTRL.State)
  {
    case UP:
      break;
    case PRESS:
      break;
		
		
		
    case SHORT_DOWN://-------------�̰�����������ģʽ���ر�С���ݣ��رյ���
			rc_sensor_dial.Imu_Mode = True;
			rc_sensor_dial.Mechanical_Mode = Flase;
			rc_sensor_dial.Cover_OpenEnable = Flase;
			break;
		
		
    case DOWN: //------------����������еģʽ���ر������Ǻ�С����
			rc_sensor_dial.Mechanical_Mode = True;
			rc_sensor_dial.Imu_Mode = Flase;
			rc_sensor_dial.This_Gryo_Mode = Flase;
      break;
		
		
    case RELAX:			
      break;
  }
}





//------------------------���򰴼�
void Dir_Key(void)
{
	Key_W();
	Key_S();
	Key_D();
	Key_A();
	

	Key_Channel_Update();//����ģ��ͨ��ֵ
}


//--------------------------���й��ܰ���
void Func_Key(void)
{
	//----------------��еģʽ��������ģʽ�л�---------------------//
	Key_ctrl();
	
	//----------------------�򿪹ر�Ħ����---------------------//
	Key_R();
	
	//---------------------��ס���--------------------------//
	Key_SHIFT();
	
	//--------------------���ص���------------------------//
	Key_B();
	
	//---------------------�л�С����------------------------//
	Key_F();
	
	//---------------------������ģʽ��Ѹ�ٰ�ͷ--------------------//
	Key_C();
}

void KB_CTRL(void)
{
	
	Dir_Key();
	 	
	Func_Key();
	
}
