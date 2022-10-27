#include "key.h"


//*--------------------------前后左右-----------------------*//
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

//----------------------迅速掉头，陀螺仪模式下--------------------------------//
void Key_C(void)
{
	
  KEY_State_Judge(&Keyboard.C ,KEY_C, CHANGE_TIM , LONG_CHANGE_TIM_C);
  switch(Keyboard.C.State)
  {
 		case UP:		
      break;
    case PRESS:
      break;
		
    case SHORT_DOWN://------------陀螺仪模式下才能迅速掉头
			if( rc_sensor_dial.Imu_Mode == True && rc_sensor_dial.This_Gryo_Mode == Flase )
			{
				if( imu_pid_Z.imu_angle_pid->target >= 0 )//----目前大于0
					imu_pid_Z.imu_angle_pid->target -= 180;
				
				else		//------目前大于0
					imu_pid_Z.imu_angle_pid->target += 180;
				
			}
				  
      break;
			
			
    case DOWN:
      break;
    case RELAX:
      break;
  }
}



//----------------------------按住冲刺-------------------------------//

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
		
    case DOWN://--------------长按
			if( rc_sensor.info->Ch[3] >= 0 )
				rc_sensor.info->Ch[3] = CH_MAX;
			else
				rc_sensor.info->Ch[3] = CH_MIN;
      break;
		
    case RELAX:
      break;
  }
}




//---------------------------弹舱-------------------------------------//
uint32_t b_1,b_2;//时间计数标志位 负责计算B键按两下的间隔时间
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
		
			if(abs(b_2 - b_1) < 500) //-----------单击关弹舱
				rc_sensor_dial.Cover_OpenEnable = Flase;
			
		  else  //------------------双击开弹舱，并且头部持平
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
	
	//弹舱打开，运动速度减慢
	if( rc_sensor_dial.Cover_OpenEnable == True )
	{
		//对于底盘，原本输出最大限幅为8000
		for (int i = 0; i < 4; i ++)
			chassis_motor[i].speed_pid->out_max = Chassis_OutMax * 0.5f;
		
		//对于云台，原本输出最大限幅为20000
		imu_pid_Z.imu_speed_pid->out_max = Gimbal_OutMax * 0.5f;
			
		imu_pid_Y.imu_speed_pid->out_max = Gimbal_OutMax * 0.5f;

		machinery_pid_Z.imu_speed_pid->out_max = Gimbal_OutMax * 0.5f;
		
		machinery_pid_Y.imu_speed_pid->out_max = Gimbal_OutMax * 0.5f;
		
	}
	
	//如果弹舱关闭，恢复运动速度
	else
	{
		//对于底盘，原本输出最大限幅为8000
		for (int i = 0; i < 4; i ++)
			chassis_motor[i].speed_pid->out_max = Chassis_OutMax;
		
		//对于云台，原本输出最大限幅为20000
		imu_pid_Z.imu_speed_pid->out_max = Gimbal_OutMax;
			
		imu_pid_Y.imu_speed_pid->out_max = Gimbal_OutMax;

		machinery_pid_Z.imu_speed_pid->out_max = Gimbal_OutMax;
		
		machinery_pid_Y.imu_speed_pid->out_max = Gimbal_OutMax;
	}	
}


//----------------------打开关闭摩擦轮---------------------//
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
		
    case DOWN:  //--------------------长按下跳变
			rc_sensor_dial.FricWheel_WorkEnable = !rc_sensor_dial.FricWheel_WorkEnable;
      break;
		
    case RELAX:
      break;
  }
}

//--------------------------小陀螺模式------------------------------//
float Keyboard_SpeedZ = 0;
uint16_t PressCnt = 0, time_cnt = 0;//辅助变速实验
void Key_F(void)
{
	time_cnt++;
	time_cnt %= 21;//完成一个正弦输出
	
  KEY_State_Judge(&Keyboard.F ,KEY_F, CHANGE_TIM , LONG_CHANGE_TIM_F);
  switch(Keyboard.F.State)
  {
    case UP: //抬起状态
      break;
		
    case PRESS: //按下瞬间
      break;
		
    case SHORT_DOWN: //短按
			rc_sensor_dial.This_Gryo_Mode = True;
			PressCnt ++;
			PressCnt %= 3;				
      break;
		
    case DOWN: //长按
      break;
		
    case RELAX: //松开瞬间
      break;
  }
	
	if( rc_sensor_dial.This_Gryo_Mode == True )
	{
		if( PressCnt == 0 )//---------低速
			Keyboard_SpeedZ = Gryo_Speed_ZMin;
				
		else if( PressCnt == 1 ) //-------快速
			Keyboard_SpeedZ = Gryo_Speed_ZMax;
				
		else	if( PressCnt == 2 )	//------------变速
			Keyboard_SpeedZ = 200 * sin( time_cnt * 3.1415926f / 10 );	
	}
	
	else
		time_cnt = PressCnt = 0;
	
}



//-----------------长按：机械模式，短按：陀螺仪模式-------------------------//
void Key_ctrl(void)
{
  KEY_State_Judge(&Keyboard.CTRL ,KEY_CTRL, CHANGE_TIM , LONG_CHANGE_TIM_CTRL);
  switch(Keyboard.CTRL.State)
  {
    case UP:
      break;
    case PRESS:
      break;
		
		
		
    case SHORT_DOWN://-------------短按开启陀螺仪模式，关闭小陀螺，关闭弹仓
			rc_sensor_dial.Imu_Mode = True;
			rc_sensor_dial.Mechanical_Mode = Flase;
			rc_sensor_dial.Cover_OpenEnable = Flase;
			break;
		
		
    case DOWN: //------------长按开启机械模式，关闭陀螺仪和小陀螺
			rc_sensor_dial.Mechanical_Mode = True;
			rc_sensor_dial.Imu_Mode = Flase;
			rc_sensor_dial.This_Gryo_Mode = Flase;
      break;
		
		
    case RELAX:			
      break;
  }
}





//------------------------方向按键
void Dir_Key(void)
{
	Key_W();
	Key_S();
	Key_D();
	Key_A();
	

	Key_Channel_Update();//更新模拟通道值
}


//--------------------------所有功能按键
void Func_Key(void)
{
	//----------------机械模式，陀螺仪模式切换---------------------//
	Key_ctrl();
	
	//----------------------打开关闭摩擦轮---------------------//
	Key_R();
	
	//---------------------按住冲刺--------------------------//
	Key_SHIFT();
	
	//--------------------开关弹舱------------------------//
	Key_B();
	
	//---------------------切换小陀螺------------------------//
	Key_F();
	
	//---------------------陀螺仪模式下迅速摆头--------------------//
	Key_C();
}

void KB_CTRL(void)
{
	
	Dir_Key();
	 	
	Func_Key();
	
}
