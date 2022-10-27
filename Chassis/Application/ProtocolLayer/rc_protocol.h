/**
 * @file        rc_protocol.c
 * @author      RobotPilots
 * @Version     v1.1
 * @brief       DT7&DR16 Rc Protocol.
 * @update
 *              v1.0(9-September-2020)
 *              v1.1(24-October-2021)
 *                  1.ÐÞ¸Ärc_potocol.c/.h->rc_protocol.c/.h 
 */
 
#ifndef __RC_PROTOCOL_H
#define __RC_PROTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "rp_math.h"
#include "rc_sensor.h"
#include "rp_user_define.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void rc_sensor_init(rc_sensor_t *rc_sen);
void FirstGetInto_KEY_PRESS(KEY_Info_t *str);
void KEY_State_Judge(KEY_Info_t *str , uint8_t KEY_PRESS , int change_tim ,int long_change_tim);
void Key_Channel_Update(void);
void USART2_rxDataHandler(uint8_t *rxBuf);
	
#endif
