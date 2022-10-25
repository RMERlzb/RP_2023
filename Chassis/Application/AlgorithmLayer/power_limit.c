#include "Power_Limit.h"



/*------------------------------功率算法---------------------------------*/




/*-祖传功率-*/
void Chassis_Motor_Power_Limit(int16_t *data)
{
	float buffer = judge_info.power_heat_data.chassis_power_buffer;
	float heat_rate,Limit_k, CHAS_LimitOutput, CHAS_TotalOutput;
	uint16_t OUT_MAX;	

	
	OUT_MAX = Chassis_OutMax * 4; //////////////--------------这里修改了
	
	if(buffer > 60)buffer = 60;//防止飞坡之后缓冲250J变为正增益系数
	
	Limit_k = buffer / 60;
	
	if(buffer < 25)
		Limit_k = Limit_k * Limit_k ;// * Limit_k; //3方
	else
		Limit_k = Limit_k;// * str->Limit_k; //平方
	
	if(buffer < 60)
		CHAS_LimitOutput = Limit_k * OUT_MAX;
	else 
		CHAS_LimitOutput = OUT_MAX;    
	
	//----------------绝对值修改了
	CHAS_TotalOutput = MyAbs_Int(data[0]) + MyAbs_Int(data[1])  \
									 + MyAbs_Int(data[2]) + MyAbs_Int(data[3]) ;
	
	heat_rate = CHAS_LimitOutput / CHAS_TotalOutput;
	
  if(CHAS_TotalOutput >= CHAS_LimitOutput)
  {
		for(char i = 0 ; i < 4 ; i++)
		{	
			data[i] = (int16_t)(data[i] * heat_rate);	
		}
	}
}


char Shoot_Power_Judge(void)
{
	
	float now_heat = judge_sensor.info->power_heat_data.shooter_id1_17mm_cooling_heat;
	
	float max_heat = judge_sensor.info->game_robot_status.shooter_id1_17mm_cooling_limit;
	
	if( MyAbs_Int (now_heat - max_heat) <= 10 )
		return 0;
	
	else
		return 1;
	
}

/*-2022电容-*/

void Chassis_2022_CAP_Power_Limit(int16_t *data)
{
	float temp = judge_sensor.info->power_heat_data.chassis_power_buffer;
	float res;
	
	if((temp / 55) < 1)
	{
		res = 0.01f + (temp / 55) * 0.99f;
	}
	else
		res = 1;
		
	for(char i = 0 ; i < 4 ; i++)
	{
		data[i] = (int16_t)(data[i] * res);
	}
}




void Chassis_Turn_Power_Limit(int16_t *data)
{
	float temp = judge_sensor.info->power_heat_data.chassis_power_buffer;
	float res;
		
	if(temp < 50)res = 0.3f + 0.7f * (temp / 40);
	else res = 1;	
	
	for(char i = 0 ; i < 4 ; i++)
	{
		data[i] = (int16_t)(data[i] * res);
	}
}

void Judge_Offline_Power_Limit(int16_t *data)
{
	float all,rate;

	all = abs(data[0]) + abs(data[1]) + abs(data[2]) + abs(data[3]) ;

	if(all > 10000)
		rate = 10000 / all;
	else 
		rate = 1;
	
	for(char i = 0 ; i < 4 ; i++)
	{	
		data[i] = (int16_t)(data[i] * rate);	
	}

}
/*-----------------------------------功率算法-------------------------------*/



