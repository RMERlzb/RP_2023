/**
 * @file        rc_protocol.c
 * @author      RobotPilots
 * @Version     v1.1
 * @brief       DT7&DR16 Rc Protocol.
 * @update
 *              v1.0(9-September-2020)
 *              v1.1(24-October-2021)
 *                  1.修改rc_potocol.c/.h->rc_protocol.c/.h 
 */
 
/* Includes ------------------------------------------------------------------*/
#include "rc_protocol.h"


/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Mouse_Updata(void);
void Mouse_FS(void);
void rc_sensor_init(rc_sensor_t *rc_sen)
{
	// 初始化为离线状态
	rc_sen->info->offline_cnt = rc_sen->info->offline_max_cnt + 1;
	rc_sen->work_state = DEV_OFFLINE;
	RC_ResetData(rc_sen);
	if(rc_sen->id == DEV_ID_RC)
		rc_sen->errno = NONE_ERR;
	else
		rc_sen->errno = DEV_ID_ERR;
}




/**
 *	@brief	遥控器数据解析协议
 */
void rc_sensor_update(rc_sensor_t *rc_sen, uint8_t *rxBuf)
{
	
	rc_sensor_info_t *rc_info = rc_sen->info;
	
	rc_info->ch0 = (rxBuf[0] | rxBuf[1] << 8) & 0x07FF;
	rc_info->ch0 -= 1024;
	rc_info->ch1 = (rxBuf[1] >> 3 | rxBuf[2] << 5) & 0x07FF;
	rc_info->ch1 -= 1024;
	rc_info->ch2 = (rxBuf[2] >> 6 | rxBuf[3] << 2 | rxBuf[4] << 10) & 0x07FF;
	rc_info->ch2 -= 1024;
	rc_info->ch3 = (rxBuf[4] >> 1 | rxBuf[5] << 7) & 0x07FF;
	rc_info->ch3 -= 1024;

	rc_info->s1 = ((rxBuf[5] >> 4) & 0x000C) >> 2;
	rc_info->s2 = (rxBuf[5] >> 4) & 0x0003;	
	
	rc_info->mouse.x = rxBuf[6]  | (rxBuf[7 ] << 8);
	rc_info->mouse.y = rxBuf[8]  | (rxBuf[9 ] << 8);
	rc_info->mouse.z = rxBuf[10] | (rxBuf[11] << 8);
	rc_info->mouse.press_l = rxBuf[12];
	rc_info->mouse.press_r = rxBuf[13];
	
	rc_info->kb.key_code= rxBuf[14] | (rxBuf[15] << 8);	
	
	rc_info->thumbwheel = ((int16_t)rxBuf[16] | ((int16_t)rxBuf[17] << 8)) & 0x07ff;
	rc_info->thumbwheel -= 1024;
	
	Mouse_FS();
	
	rc_info->offline_cnt = 0;
}













/*------------------------按键按下的状态开始------------------------*/
void FirstGetInto_KEY_PRESS(KEY_Info_t *str)
{
  if(str->prev_KEY_PRESS != str->KEY_PRESS)
  {
    str->state_cnt = 0;
    str->prev_KEY_PRESS = str->KEY_PRESS;
  }
}

void KEY_State_Judge(KEY_Info_t *str , uint8_t KEY_PRESS , int change_tim ,int long_change_tim)
{
  str->KEY_PRESS = KEY_PRESS;
  FirstGetInto_KEY_PRESS(str);
  switch(KEY_PRESS)
  {
		//这次松手
    case KEY_UP:  {
			//如果上一次是按下
      if(str->prev_State != UP) 
      {
        str->state_cnt++;
				//达到一定条件才算1-->0
        if(str->state_cnt >= change_tim)  
        {
          str->State = RELAX;
          str->prev_State = RELAX;
					//达到一定条件才算0
          if(str->state_cnt >= change_tim + 1)  //抬起不分长短抬
          {
            str->State = UP;
            str->prev_State = UP;
          }
        }		
      }
			//如果上一次是松手
			else{str->state_cnt = 0;}
    }break;
    
    case KEY_DOWN:    {
      if(str->prev_State != DOWN) 
      {
        str->state_cnt++;
        if(str->state_cnt >= change_tim)  
        {
          str->State = PRESS;
          str->prev_State = PRESS;
          if(str->state_cnt >= change_tim + 1)
          {
            str->State = SHORT_DOWN;
            str->prev_State = SHORT_DOWN;
            if(str->state_cnt >= long_change_tim)  
            {
              str->State = DOWN;
              str->prev_State = DOWN;
            }
          }
        }
      }else{str->state_cnt = 0;}
    }break;
  }
}




/*-------------------------按键按下的状态结束---------------------------*/



/**
 * @brief 鼠标卡尔曼
 * @note  已加滑动滤波
 * @param 
 */

float Mouse_X_UPDATA, Mouse_Y_UPDATA, Mouse_Z_UPDATA;
float Mouse_X_Last, Mouse_Y_Last, Mouse_Z_Last;
//6.46

float Mouse_X_Speed(void)
{
  float res;
	
	Mouse_X_Last = rc_sensor.info->Ch[0];
	
  if(MyAbs_Float(MOUSE_X_MOVE_SPEED > Xmax))res = 0;
  else 
		res = SF(MOUSE_X_MOVE_SPEED,Keyboard.MouseSF.SFX,0);
	
	rc_sensor.info->Ch[0] = (0.7f * MOUSE_X_RATE * res + 0.3f * Mouse_X_Last);
	
	if(rc_sensor.info->Ch[0]> 660)rc_sensor.info->Ch[0]= 660;
	if(rc_sensor.info->Ch[0]<-660)rc_sensor.info->Ch[0]=-660;
	
  return res;
}


float Mouse_Y_Speed(void)
{
  float res;
	
	Mouse_Y_Last = rc_sensor.info->Ch[1];
	
  if(MyAbs_Float(MOUSE_Y_MOVE_SPEED > Ymax))res = 0;
  else
		res = -SF(MOUSE_Y_MOVE_SPEED,Keyboard.MouseSF.SFY,0);

	rc_sensor.info->Ch[1] = (0.7f * MOUSE_Y_RATE * res + 0.3f * Mouse_Y_Last);
	
	if(rc_sensor.info->Ch[1]> 660)rc_sensor.info->Ch[1]= 660;
	if(rc_sensor.info->Ch[1]<-660)rc_sensor.info->Ch[1]=-660;
	
	if(MyAbs_Float(rc_sensor_info.Ch[1]) < 1)rc_sensor.info->Ch[1] = 0;
	
  return res;
}


float Mouse_Z_Speed(void)
{
  float res;
	
	Mouse_Z_Last = Mouse_Z_UPDATA;
	 
	res = SF(MOUSE_Z_MOVE_SPEED,Keyboard.MouseSF.SFZ,0);
	
	res = (0.8f * res + 0.2f * Mouse_Z_Last);
	
	//rc_sensor.info->Mouse_ROLL = res;
	
  return res;
}



void Mouse_Updata(void)
{
	Mouse_X_UPDATA = Mouse_X_Speed();
	Mouse_Y_UPDATA = Mouse_Y_Speed();
	Mouse_Z_UPDATA = Mouse_Z_Speed();
}

void Mouse_FS(void)
{
	
	Mouse_Updata();
	
	Mouse_X_Last = rc_sensor.info->Ch[0];
	Mouse_Y_Last = rc_sensor.info->Ch[1];
	
	rc_sensor.info->Ch[0] = (0.7f * MOUSE_X_RATE * Mouse_X_UPDATA + 0.3f * Mouse_X_Last);
  rc_sensor.info->Ch[1] = (0.7f * MOUSE_Y_RATE * Mouse_Y_UPDATA + 0.3f * Mouse_Y_Last);
	
  rc_sensor.info->Mouse_ROLL = Mouse_Z_UPDATA;
	
	if(rc_sensor.info->Ch[0]> 660)rc_sensor.info->Ch[0]= 660;
	if(rc_sensor.info->Ch[0]<-660)rc_sensor.info->Ch[0]=-660;
	
	if(rc_sensor.info->Ch[1]> 660)rc_sensor.info->Ch[1]= 660;
	if(rc_sensor.info->Ch[1]<-660)rc_sensor.info->Ch[1]=-660;
	
	
}






/*--------------------键盘值转为遥控通道值 begin---------------------*/

//wsad中间通道值，最后计算结果赋予rc中模拟通道值Ch[]
float W_Chan,
      S_Chan,
      A_Chan,
      D_Chan;


void Sim_Channel(float *CH, char dir,float num)//1up 0down
{
	if( dir)*CH+=num;
	if(!dir)*CH-=num;
	
	if(*CH > CH_MAX)     *CH = CH_MAX;
	if(*CH < CH_OFFSET  )*CH = CH_OFFSET;
}

/////////////////////////////////////
void WS_Channel(void) 
{
	int16_t CH;
	CH = W_Chan - S_Chan;	
	if(MyAbs_Int(CH) > CH_MAX)
	{
		if(CH > 0)CH = CH_MAX;
		if(CH < 0)CH = CH_MIN;
	}
	rc_sensor.info->Ch[3] = CH;
}

void AD_Channel(void) 
{
	int16_t CH;
	CH = D_Chan - A_Chan;
	if(MyAbs_Int(CH) > CH_MAX)
	{
		if(CH > 0)CH = CH_MAX;
		if(CH < 0)CH = CH_MIN;
	}
	rc_sensor.info->Ch[2] = CH;
}




/*-缓慢减速-*/
float CH_UP   = 1.f;
float CH_DOWN = 3.f;
void Key_Channel_Update(void)
{
	if(KEY_W)Sim_Channel(&W_Chan, 1, CH_UP);
	else     Sim_Channel(&W_Chan, 0, CH_DOWN);
	
	if(KEY_S)Sim_Channel(&S_Chan, 1, CH_UP);
	else		 Sim_Channel(&S_Chan, 0, CH_DOWN);

	if(KEY_A)Sim_Channel(&A_Chan, 1, CH_UP);
	else     Sim_Channel(&A_Chan, 0, CH_DOWN);

	if(KEY_D)Sim_Channel(&D_Chan, 1, CH_UP);
	else     Sim_Channel(&D_Chan, 0, CH_DOWN);
	
	WS_Channel();
	AD_Channel();
}



/*-----------------------键盘值转为遥控通道值结束---------------------*/









/**
 *	@brief	在串口2中解析遥控数据协议
*/


void USART2_rxDataHandler(uint8_t *rxBuf)
{	
	rc_sensor.update(&rc_sensor, rxBuf);
	rc_sensor.check(&rc_sensor);	
	
}
