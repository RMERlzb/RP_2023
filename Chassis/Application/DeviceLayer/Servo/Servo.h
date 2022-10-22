#ifndef __Servo_H
#define __Servo_H

#include "stm32f4xx.h" 
#include "main.h"
#include "drv_io.h"
#include "tim.h"
#include "rc_sensor.h"
#include "rp_device_config.h"

#define Servo_PWM	  TIM1->CCR2

typedef enum {
	Close,
	Open,
} Servo_State_t;

void Servo_Init(void);
void Cover_PwmOut(int16_t pwm);
void Cover_Sleep(void);
void Cover_Awake(void);
char Judge_Cover_State(void);
void Cover_Open(void);
void Cover_Close(void);

#endif
