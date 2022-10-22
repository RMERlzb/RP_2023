#include "Servo.h"

extern TIM_HandleTypeDef htim1;

void Cover_PwmOut(int16_t pwm);
void Cover_Awake(void);

void Servo_Init(void)
{
	Cover_Awake();
	
	Cover_PwmOut(0);
}

//168000000 1680 1000 100hz 占空比x/1000

//PE11
void Cover_PwmOut(int16_t pwm)
{
	Servo_PWM = pwm;
}


void Cover_Sleep(void)
{
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
}	

void Cover_Awake(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}



void Cover_Open(void)
{

	Cover_PwmOut(50);
	
}
void Cover_Close(void)
{
	
	Cover_PwmOut(225);
}

char Judge_Cover_State(void)
{
	//关着的，在IMU_Mode函数中会调用关闭盖子，此时标志位改变
	if( Servo_PWM == 225 )
	{
		rc_sensor_dial.Cover_OpenEnable = Flase;
		return Close;
	}
		
	
	else
		return Open;
}
