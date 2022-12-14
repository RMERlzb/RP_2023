/**
 * @file        rc_sensor.c
 * @author      RobotPilots@2020
 * @Version     V1.0
 * @date        9-September-2020
 * @brief       Device Rc.
 */
 
/* Includes ------------------------------------------------------------------*/
#include "rc_sensor.h"

#include "rp_math.h"
//#include "device.h"
#include "rc_protocol.h"

extern void rc_sensor_init(rc_sensor_t *rc_sen);
extern void rc_sensor_update(rc_sensor_t *rc_sen, uint8_t *rxBuf);

//-----------------添加的函数指针---------------------------------//
void rc_dial_reset(rc_sensor_t *rc);
void rc_dial_jugde(rc_sensor_t *rc);




/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void RC_ResetData(rc_sensor_t *rc);
static void rc_sensor_check(rc_sensor_t *rc_sen);
static void rc_sensor_heart_beat(rc_sensor_t *rc_sen);
void rc_sensor_start_check(rc_sensor_t *rc);
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

Keyboard_Info_t Keyboard;

// 遥控器驱动
drv_uart_t	rc_sensor_driver = {
	.type = DRV_UART2,
	.tx_byte = NULL,
};

/* 键盘信息 */
Keyboard_Info_t Keyboard;


// 遥控器信息
rc_sensor_info_t 	rc_sensor_info = {
	.offline_max_cnt = 60,
};

//遥控器拨码和相应的功能标志位

rc_sensor_dial_t rc_sensor_dial = {
	.This_LeftDial_Mode = 0,
	.Last_LeftDial_Mode = 0,
	
	.This_RightDial_Mode = 0,
	.Last_RightDial_Mode = 0,
	
	.Mechanical_Mode = 0,
	.Imu_Mode = 0,
	.Keyboard_Mode = 0,
	
	
	.This_Gryo_Mode = 0,
	.Last_Gryo_Mode = 0,
	
	.ThumbWheel_Up_Toggle = 0,
	.ThumbWheel_Down_Toggle = 0,
	
	.Continue_ShootEnable = 0,
	.FricWheel_WorkEnable = 0,
	
	.Single_ShootEnable = 0,
	
	.Version_Control_Enable = 0,
	
	.Var_Change_Enable = 0,
	
	.Continue_Shoot_BlockFlag = 0,
	.Single_Shoot_BlockFlag = 0,
	
	.ContinueShoot_Block_ToggleDir = 0,
	.SingleShoot_Block_ToggleDir = 0,
	
	.ContinueShoot_Cnt = 0,
	.SingleShoot_Cnt = 0,
};


// 遥控器传感器
rc_sensor_t	rc_sensor = {
	
	.info = &rc_sensor_info,
	.dial = &rc_sensor_dial,
	.init = rc_sensor_init,
	
	.update = rc_sensor_update,
	.check = rc_sensor_check,
	.heart_beat = rc_sensor_heart_beat,
	//添加的
	.dial_reset = rc_dial_reset,
	.dial_jugde = rc_dial_jugde,
	
	.work_state = DEV_OFFLINE,
	.tran_state = DEV_OFF,
	.id = DEV_ID_RC,
	
};




/* Private functions ---------------------------------------------------------*/
/**
 *	@brief	遥控器数据检查
 */
static void rc_sensor_check(rc_sensor_t *rc_sen)
{
	rc_sensor_info_t *rc_info = rc_sen->info;
	
	if(abs(rc_info->ch0) > 660 ||
	   abs(rc_info->ch1) > 660 ||
	   abs(rc_info->ch2) > 660 ||
	   abs(rc_info->ch3) > 660)
	{
		rc_sen->errno = DEV_DATA_ERR;
		rc_info->ch0 = 0;
		rc_info->ch1 = 0;
		rc_info->ch2 = 0;
		rc_info->ch3 = 0;		
//		rc_info->s1 = RC_SW_MID;
	//	rc_info->s2 = RC_SW_MID;
		rc_info->thumbwheel = 0;
	}
	else
	{
		rc_sen->errno = NONE_ERR;
	}
}

/**
 *	@brief	遥控器心跳包
 */
static void rc_sensor_heart_beat(rc_sensor_t *rc_sen)
{
	rc_sensor_info_t *rc_info = rc_sen->info;

	rc_info->offline_cnt++;
	if(rc_info->offline_cnt > rc_info->offline_max_cnt) {
		rc_info->offline_cnt = rc_info->offline_max_cnt;
		rc_sen->work_state = DEV_OFFLINE;
	} 
	else {
		/* 离线->在线 */
		if(rc_sen->work_state == DEV_OFFLINE)
		{
			rc_sen->work_state = DEV_ONLINE;
			RC_ResetData(rc_sen);  //重新连接后复位所有遥控数据
		}
			
	}
}

/* Exported functions --------------------------------------------------------*/
bool RC_IsChannelReset(void)
{
	if((DeathZoom(rc_sensor_info.ch0, 0, 50) == 0) && 
		 (DeathZoom(rc_sensor_info.ch1, 0, 50) == 0) && 
		 (DeathZoom(rc_sensor_info.ch2, 0, 50) == 0) && 
		 (DeathZoom(rc_sensor_info.ch3, 0, 50) == 0)   )	
	{
		return true;
	}
	return false;		
}



void RC_ResetData(rc_sensor_t *rc)
{
	rc->info->ch0 = 0;           // 通道值强行设置成中间值(不拨动摇杆的状态)
	rc->info->ch1 = 0;
	rc->info->ch2 = 0;
	rc->info->ch3 = 0;	
	rc->info->s1  = 4;      
	rc->info->s2  = 4;
	
	rc->info->mouse.x        = 0;// 鼠标
	rc->info->mouse.y        = 0;
	rc->info->mouse.z        = 0;
	rc->info->mouse.press_l  = 0;
	rc->info->mouse.press_r  = 0;	
	rc->info->kb.key_code    = 0;// 键盘	
	rc->info->thumbwheel     = 0;// 左拨轮
	
	for(char i=0; i<KEY_CNT; i++)//复位所有键盘标志位
		rc->info->Key_Flg[i] = 0;	
	
//	State.init();
}

/*

	State
	状态切换初始化

*/
void Rc_Init(void)
{
	rc_sensor.info->ch0 = 0;           // 通道值强行设置成中间值(不拨动摇杆的状态)
	rc_sensor.info->ch1 = 0;
	rc_sensor.info->ch2 = 0;
	rc_sensor.info->ch3 = 0;	
	rc_sensor.info->thumbwheel     = 0;// 左拨轮
//	rc_sensor.info->TW             = 0;
//	State.SW2_LOCK = 0;
	
	rc_sensor.info->mouse.x        = 0;// 鼠标
	rc_sensor.info->mouse.y        = 0;
	rc_sensor.info->mouse.z        = 0;
	rc_sensor.info->mouse.press_l  = 0;
	rc_sensor.info->mouse.press_r  = 0;	
	rc_sensor.info->kb.key_code    = 0;// 键盘	
	rc_sensor.info->thumbwheel     = 0;// 左拨轮

	for(char i=0; i<KEY_CNT; i++)//复位所有键盘标志位
	rc_sensor.info->Key_Flg[i] = 0;	
	
//	State.CH_LOCK = 0;
}

void Key_Init(void)
{
	rc_sensor.info->mouse.x        = 0;// 鼠标
	rc_sensor.info->mouse.y        = 0;
	rc_sensor.info->mouse.z        = 0;
	rc_sensor.info->mouse.press_l  = 0;
	rc_sensor.info->mouse.press_r  = 0;	
	rc_sensor.info->kb.key_code    = 0;// 键盘	
	rc_sensor.info->thumbwheel     = 0;// 左拨轮
//	rc_sensor.info->TW             = 0;
	
	for(char i=0; i<KEY_CNT; i++)//复位所有键盘标志位
	rc_sensor.info->Key_Flg[i] = 0;	
	
//	State.CH_LOCK = 0;
}
/*状态切换初始化end*/




void rc_dial_reset(rc_sensor_t *rc)
{
	//memset( &rc_sensor_dial, Flase, sizeof(rc_sensor_dial) );
	
	rc->dial->This_LeftDial_Mode = Flase;
	rc->dial->Last_LeftDial_Mode = Flase;
	
	rc->dial->This_RightDial_Mode = Flase;
	rc->dial->Last_RightDial_Mode = Flase;
	
	rc->dial->Mechanical_Mode = Flase;
	rc->dial->Imu_Mode = Flase;
	rc->dial->Keyboard_Mode = Flase;
	
	
	rc->dial->This_Gryo_Mode = Flase;
	rc->dial->Last_Gryo_Mode = Flase;
	
	rc->dial->ThumbWheel_Up_Toggle = Flase;
	rc->dial->ThumbWheel_Down_Toggle = Flase;
	
	rc->dial->Continue_ShootEnable = Flase;
	rc->dial->FricWheel_WorkEnable = Flase;
	
	rc->dial->Single_ShootEnable = Flase;
	
	rc->dial->Version_Control_Enable = Flase;
	
	rc->dial->Var_Change_Enable = Flase;
	
	rc->dial->Continue_Shoot_BlockFlag = Flase;
	rc->dial->Single_Shoot_BlockFlag = Flase;	
	
	rc->dial->ContinueShoot_Block_ToggleDir = Flase;
	rc->dial->SingleShoot_Block_ToggleDir = Flase;
	
	rc->dial->Cover_OpenEnable = Flase;
	
	rc->dial->ContinueShoot_Cnt = Flase;
	rc->dial->SingleShoot_Cnt = Flase;
}



void rc_dial_jugde(rc_sensor_t *rc)
{
	
	rc->dial->Last_LeftDial_Mode = rc->dial->This_LeftDial_Mode;
	rc->dial->Last_RightDial_Mode = rc->dial->This_RightDial_Mode;	

	rc->dial->Last_Gryo_Mode = rc->dial->This_Gryo_Mode;	
	
	rc->dial->This_LeftDial_Mode = rc->info->s1;
	rc->dial->This_RightDial_Mode = rc->info->s2;
	
	
	
	
	
	//拨码在机械模式时
	if( rc->dial->This_LeftDial_Mode == mec_mode )
	{
		rc->dial->Mechanical_Mode = True;
		
		rc->dial->Imu_Mode = Flase;
		rc->dial->This_Gryo_Mode = Flase;
		rc->dial->Keyboard_Mode = Flase;
		
		rc->dial->Continue_ShootEnable = Flase;
		rc->dial->FricWheel_WorkEnable = True;	
		rc->dial->Single_ShootEnable = True;
		
		//判断是否要开启单次发射
		if( rc->dial->This_RightDial_Mode == single_shot_mode )
		{
			if( rc->dial->This_RightDial_Mode != rc->dial->Last_RightDial_Mode ) 		
				rc->dial->Var_Change_Enable = !rc->dial->Var_Change_Enable;
		}	
		

		//判断是否开盖
		if( rc->dial->This_RightDial_Mode == cover_state_trans )
		{
			if( rc->dial->This_RightDial_Mode != rc->dial->Last_RightDial_Mode ) 		
				rc->dial->Cover_OpenEnable = !rc->dial->Cover_OpenEnable;
		}	
	}
	
	
	
	//拨码在陀螺仪模式时
	if( rc->dial->This_LeftDial_Mode == imu_mode )
	{			
		rc->dial->Imu_Mode = True;		
		
		rc->dial->Mechanical_Mode = Flase;
		rc->dial->Keyboard_Mode = Flase;
		
		rc->dial->Single_ShootEnable = Flase;
		
		//左拨码在小陀螺模式
		if( rc->info->thumbwheel >= 330 )	
			rc->dial->This_Gryo_Mode = True;
		else
			rc->dial->This_Gryo_Mode = Flase;
		
		//判断滚轮状态
		if( rc->info->thumbwheel <= -330 )	
			rc->dial->ThumbWheel_Up_Toggle = !rc->dial->ThumbWheel_Up_Toggle;
		
		
		//判断是否连续发射
		if( rc->dial->This_RightDial_Mode == continue_shot_mode )
		{
			if( rc->dial->This_RightDial_Mode != rc->dial->Last_RightDial_Mode ) 				
				rc->dial->Continue_ShootEnable = !rc->dial->Continue_ShootEnable;
				
		}	
		
		
		
		//判断是否要开启视觉控制，有两个条件-----一个左滚轮跳变往上开启，一个陀螺仪模式下拨杆在中间
		if( rc->dial->This_RightDial_Mode == version_control_mode && \
				rc->dial->ThumbWheel_Up_Toggle == True )
			rc->dial->Version_Control_Enable = True;
		
		else
			rc->dial->Version_Control_Enable = Flase;
		
		//判断是否开启摩擦轮
		if( rc->dial->This_RightDial_Mode == fricwheel_state_trans )
		{		
			if( rc->dial->This_RightDial_Mode != rc->dial->Last_RightDial_Mode ) 			
				rc->dial->FricWheel_WorkEnable = !rc->dial->FricWheel_WorkEnable;
			
		}
		
		//再次判断是否发射
		if( rc->dial->FricWheel_WorkEnable == Flase )
			rc->dial->Continue_ShootEnable = Flase;
	}
	
	
	
	//跳跃到键盘模式时，全部清空，默认手动开启机械模式
	if( rc->dial->This_LeftDial_Mode == keyboard_mode && \
			rc->dial->Last_LeftDial_Mode != keyboard_mode )
	{
		char lastmode_temp = 0, thismode_temp = 0;
		
		lastmode_temp = rc->dial->This_LeftDial_Mode;
		thismode_temp = rc->dial->Last_LeftDial_Mode;
		
		rc_dial_reset(&rc_sensor);
		
		rc->dial->This_LeftDial_Mode = lastmode_temp;
		rc->dial->Last_LeftDial_Mode = thismode_temp;
		
		rc->dial->Mechanical_Mode = True;
	}
		
	//跳出键盘模式时，全清空，模式会自动识别
	if( rc->dial->This_LeftDial_Mode != keyboard_mode && \
			rc->dial->Last_LeftDial_Mode == keyboard_mode )
	{
		char lastmode_temp = 0, thismode_temp = 0;
		
		lastmode_temp = rc->dial->This_LeftDial_Mode;
		thismode_temp = rc->dial->Last_LeftDial_Mode;
		
		rc_dial_reset(&rc_sensor);
		
		rc->dial->This_LeftDial_Mode = lastmode_temp;
		rc->dial->Last_LeftDial_Mode = thismode_temp;
	}
	
	
	//左拨码在键盘模式时，通过键盘可以开启机械或者陀螺仪
	if( rc->dial->This_LeftDial_Mode == keyboard_mode )
	{
		
		rc->dial->Keyboard_Mode = True;
		
		//摩擦轮开启的情况下才能打开拨弹轮
		if( rc->dial->FricWheel_WorkEnable == Flase )
			rc->dial->Continue_ShootEnable = Flase;
		
		
		
	}
	
	
}
