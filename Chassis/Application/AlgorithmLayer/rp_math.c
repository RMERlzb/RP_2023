/**
 * @file        rp_math.c
 * @author      RobotPilots
 * @Version     v1.1
 * @brief       RobotPilots Robots' Math Libaray.
 * @update
 *              v1.0(11-September-2020)
 *              v1.1(13-November-2021)
 *                  1.增加位操作函数
 */
 
/* Includes ------------------------------------------------------------------*/
#include "rp_math.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
//int16_t RampInt(int16_t final, int16_t now, int16_t ramp)
//{
//	int32_t buffer = 0;
//	
//	buffer = final - now;
//	if (buffer > 0)
//	{
//		if (buffer > ramp)
//			now += ramp;
//		else
//			now += buffer;
//	}
//	else
//	{
//		if (buffer < -ramp)
//			now += -ramp;
//		else
//			now += buffer;
//	}

//	return now;
//}

//float RampFloat(float final, float now, float ramp)
//{
//	float buffer = 0;
//	
//	buffer = final - now;
//	if (buffer > 0)
//	{
//		if (buffer > ramp)
//			now += ramp;
//		else
//			now += buffer;
//	}
//	else
//	{
//		if (buffer < -ramp)
//			now += -ramp;
//		else
//			now += buffer;
//	}

//	return now;	
//}

float DeathZoom(float input, float center, float death)
{
	if(abs(input - center) < death)
		return center;
	return input;
}

void ConstrainFloat(float* input, float max, float min)
{
	if(*input >= max)
		*input = max;
	
	else if(*input <= min)
		*input = min;	
}

void ConstrainInt(int16_t* input, int16_t max, int16_t min)
{
	if(*input >= max)
		*input = max;
	
	else if(*input <= min)
		*input = min;	
}

//不仅会让目标值和测量值的范围一致，并且还会对误差进行突变情况处理
void MyRampFloat(float* target, float measure, float ramp, float* err)
{

		if(*target >= ramp)
			*target -= ramp * 2;
		else if(*target <= -ramp)
			*target += ramp * 2;

		if(*target - measure >= ramp)
			*err = *target - measure - ramp * 2;
		else if(*target -measure <= -ramp)
			*err = *target - measure + ramp * 2;
		else
			*err = *target - measure;
	
}

//不仅会让目标值和测量值的范围一致，并且还会对误差进行突变情况处理
void MyRampInt(int16_t* target, int16_t measure, int16_t ramp, int16_t* err)
{
 
		if(*target >= ramp)
			*target -= ramp * 2;
		else if(*target <= -ramp)
			*target += ramp * 2;

		if(*target - measure >= ramp)
			*err = *target - measure - ramp * 2;
		else if(*target -measure <= -ramp)
			*err = *target - measure + ramp * 2;
		else
			*err = *target - measure;
	
}
//不需要改变目标值，只需要对误差进行突变情况处理
void RampFloat(float target, float measure, float ramp, float* err)
{
	*err = target - measure;
	
	if(target - measure >= ramp)
		*err -= ramp * 2;
	else if(target - measure <= -ramp)
		*err += ramp * 2;
		
}
//不需要改变目标值，只需要对误差进行突变情况处理
void RampInt(int16_t target, int16_t measure, int16_t ramp, int16_t* err)
{
	*err = target - measure;
	
	if(target - measure >= ramp)
		*err -= ramp * 2;
	else if(target - measure <= -ramp)
		*err += ramp * 2;	
}

int16_t MyAbs_Int(int16_t num)
{
	if(num >= 0)
		return num;
	else
		return (-num);
}

float MyAbs_Float(float num)
{
	if(num >= 0)
		return num;
	else
		return (-num);
}

float SF(float t,float *slopeFilter,float res)
{
  for(int i = SF_LENGTH-1;i>0;i--)
  {
    slopeFilter[i] = slopeFilter[i-1];
  }slopeFilter[0] = t;
  for(int i = 0;i<SF_LENGTH;i++)
  {
    res += slopeFilter[i];
  }
	return (res/SF_LENGTH);
}
