/**
 * @file        rp_math.h
 * @author      RobotPilots
 * @Version     v1.1
 * @brief       RobotPilots Robots' Math Libaray.
 * @update
 *              v1.0(11-September-2020)
 *              v1.1(13-November-2021)
 *                  1.增加位操作函数
 */

#ifndef __RP_MATH_H
#define __RP_MATH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "rc_sensor.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* 位操作函数 */
#define SET_EVENT(EVENT, FLAG)      ((EVENT) |= FLAG)     
#define CLEAR_EVENT(EVENT, FLAG)    ((EVENT) &= ~(FLAG))
#define GET_EVENT(EVENT, FLAG)      ((EVENT) & (FLAG))
/* 数值函数 */
#define constrain(x, min, max)	((x>max)?max:(x<min?min:x))
#define abs(x) 					((x)>0? (x):(-(x)))
/* 斜坡函数 */
//int16_t RampInt(int16_t final, int16_t now, int16_t ramp);
//float RampFloat(float final, float now, float ramp);

/* 死区函数 */
float DeathZoom(float input, float center, float death);



//浮点型数据限幅
void ConstrainFloat(float* input, float max, float min);

//整型数据限幅
void ConstrainInt(int16_t* input, int16_t max, int16_t min);

//过零点函数
void MyRampFloat(float* target, float measure, float ramp, float* err);
void MyRampInt(int16_t* target, int16_t measure, int16_t ramp, int16_t* err);

void RampFloat(float target, float measure, float ramp, float* err);
void RampInt(int16_t target, int16_t measure, int16_t ramp, int16_t* err);


//绝对值函数，不会强行改变传入的数字，但是宏定义的绝对值函数会强行改变
int16_t MyAbs_Int(int16_t num);
float MyAbs_Float(float num);

float SF(float t,float *slopeFilter,float res);
#endif

