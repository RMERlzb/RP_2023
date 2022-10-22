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

//-----------------��ӵĺ���ָ��---------------------------------//
void rc_dial_reset(rc_sensor_t *rc);
void rc_dial_jugde(rc_sensor_t *rc);




/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void rc_sensor_check(rc_sensor_t *rc_sen);
static void rc_sensor_heart_beat(rc_sensor_t *rc_sen);
void rc_sensor_start_check(rc_sensor_t *rc);
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
// ң��������
drv_uart_t	rc_sensor_driver = {
	.id = DRV_UART2,
	.tx_byte = NULL,
};

// ң������Ϣ
rc_sensor_info_t 	rc_sensor_info = {
	.offline_max_cnt = 60,
};

//ң�����������Ӧ�Ĺ��ܱ�־λ

rc_sensor_dial_t rc_sensor_dial = {
	.This_LeftDial_Mode = 0,
	.Last_LeftDial_Mode = 0,
	
	.This_RightDial_Mode = 0,
	.Last_RightDial_Mode = 0,
	
	.Mechanical_Mode = 0,
	.Imu_Mode = 0,
	.This_Gryo_Mode = 0,
	.Last_Gryo_Mode = 0,
	
	.Continue_ShootEnable = 0,
	.FricWheel_WorkEnable = 0,
	
	.Single_ShootEnable = 0,
	
	.Var_Change_Enable = 0,
	
	.Continue_Shoot_BlockFlag = 0,
	.Single_Shoot_BlockFlag = 0,
	
	.ContinueShoot_Block_ToggleDir = 0,
	.SingleShoot_Block_ToggleDir = 0,
	
	.ContinueShoot_Cnt = 0,
	.SingleShoot_Cnt = 0,
};


// ң����������
rc_sensor_t	rc_sensor = {
	
	.info = &rc_sensor_info,
	.dial = &rc_sensor_dial,
	.init = rc_sensor_init,
	
	.update = rc_sensor_update,
	.check = rc_sensor_check,
	.heart_beat = rc_sensor_heart_beat,
	//��ӵ�
	.dial_reset = rc_dial_reset,
	.dial_jugde = rc_dial_jugde,
	
	.work_state = DEV_OFFLINE,
	.tran_state = DEV_OFF,
	.id = DEV_ID_RC,
	
};

/* Private functions ---------------------------------------------------------*/
/**
 *	@brief	ң�������ݼ��
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
		rc_info->s1 = RC_SW_MID;
		rc_info->s2 = RC_SW_MID;
		rc_info->thumbwheel = 0;
	}
	else
	{
		rc_sen->errno = NONE_ERR;
	}
}

/**
 *	@brief	ң����������
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
		/* ����->���� */
		if(rc_sen->work_state == DEV_OFFLINE)
			rc_sen->work_state = DEV_ONLINE;
	}
}

/* Exported functions --------------------------------------------------------*/
bool RC_IsChannelReset(void)
{
	if(  (DeathZoom(rc_sensor_info.ch0, 0, 50) == 0) && 
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
	// ͨ��ֵǿ�����ó��м�ֵ(������ҡ�˵�״̬)
	rc->info->ch0 = 0;
	rc->info->ch1 = 0;
	rc->info->ch2 = 0;
	rc->info->ch3 = 0;
	// ���ҿ���ѡ��ǿ�����ó��м�ֵ״̬
	rc->info->s1 = RC_SW_MID;
	rc->info->s2 = RC_SW_MID;
	// ���
	rc->info->mouse_vx = 0;
	rc->info->mouse_vy = 0;
	rc->info->mouse_vz = 0;
	rc->info->mouse_btn_l = 0;
	rc->info->mouse_btn_r = 0;
	// ����
	rc->info->key_v = 0;
	// ����
	rc->info->thumbwheel = 0;
}





void rc_dial_reset(rc_sensor_t *rc)
{
//	memset( &rc_sensor_dial, 0, sizeof(rc_sensor_dial) );
	rc->dial->This_LeftDial_Mode = Flase;
	rc->dial->Last_LeftDial_Mode = Flase;
	
	rc->dial->This_RightDial_Mode = Flase;
	rc->dial->Last_RightDial_Mode = Flase;
	
	rc->dial->Mechanical_Mode = Flase;
	rc->dial->Imu_Mode = Flase;
	rc->dial->This_Gryo_Mode = Flase;
	rc->dial->Last_Gryo_Mode = Flase;
	
	rc->dial->Continue_ShootEnable = Flase;
	rc->dial->FricWheel_WorkEnable = Flase;
	
	rc->dial->Single_ShootEnable = Flase;
	
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
	
	
	
	//�����ڻ�еģʽʱ
	if( rc->dial->This_LeftDial_Mode == mec_mode )
	{
		rc->dial->Mechanical_Mode = True;
		
		rc->dial->Imu_Mode = Flase;
		rc->dial->This_Gryo_Mode = Flase;
		
		rc->dial->Continue_ShootEnable = Flase;
		rc->dial->FricWheel_WorkEnable = True;	
		rc->dial->Single_ShootEnable = True;
		
		//�ж��Ƿ�Ҫ�������η���
		if( rc->dial->This_RightDial_Mode == single_shot_mode )
		{
			if( rc->dial->This_RightDial_Mode != rc->dial->Last_RightDial_Mode ) 		
				rc->dial->Var_Change_Enable = !rc->dial->Var_Change_Enable;
		}	
		
		//�ж��Ƿ񿪸�
		if( rc->dial->This_RightDial_Mode == cover_state_trans )
		{
			if( rc->dial->This_RightDial_Mode != rc->dial->Last_RightDial_Mode ) 		
				rc->dial->Cover_OpenEnable = !rc->dial->Cover_OpenEnable;
		}	
	}
	
	
	
	//������������ģʽʱ
	if( rc->dial->This_LeftDial_Mode == imu_mode )
	{		
		rc->dial->Mechanical_Mode = Flase;
		rc->dial->Imu_Mode = True;		
		
		rc->dial->Single_ShootEnable = Flase;
		
		//������С����ģʽ
		if( rc->info->thumbwheel >= 330 )	
			rc->dial->This_Gryo_Mode = True;
		else
			rc->dial->This_Gryo_Mode = Flase;
		
			
		//�ж��Ƿ���������
		if( rc->dial->This_RightDial_Mode == continue_shot_mode )
		{
			if( rc->dial->This_RightDial_Mode != rc->dial->Last_RightDial_Mode ) 				
				rc->dial->Continue_ShootEnable = !rc->dial->Continue_ShootEnable;
				
		}	
		
		//�ж��Ƿ���Ħ����
		if( rc->dial->This_RightDial_Mode == fricwheel_state_trans )
		{		
			if( rc->dial->This_RightDial_Mode != rc->dial->Last_RightDial_Mode ) 			
				rc->dial->FricWheel_WorkEnable = !rc->dial->FricWheel_WorkEnable;
			
		}
		
		//�ٴ��ж��Ƿ���
		if( rc->dial->FricWheel_WorkEnable == Flase )
			rc->dial->Continue_ShootEnable = Flase;
	}
	
	
	//�����ڼ���ģʽʱ
	if( rc->dial->This_LeftDial_Mode == keyboard_mode )
	{
		rc->dial->Mechanical_Mode = Flase;
		rc->dial->Imu_Mode = Flase;
		rc->dial->This_Gryo_Mode = Flase;
	}
	
	
}
