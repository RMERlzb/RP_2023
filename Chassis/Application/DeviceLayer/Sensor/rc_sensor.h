#ifndef __RC_SENSOR_H
#define __RC_SENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "string.h"
#include "rp_user_define.h"
#define    RC_ONLINE             (rc_sensor.work_state == DEV_ONLINE)
#define    RC_OFFLINE            (rc_sensor.work_state == DEV_OFFLINE)
#define    RC_CH_VALUE_MIN       ((uint16_t)364 )
#define    RC_CH_VALUE_OFFSET    ((uint16_t)1024)
#define	   RC_CH_VALUE_MAX       ((uint16_t)1684)
#define	   RC_CH_VALUE_SIDE_WIDTH	((RC_CH_VALUE_MAX-RC_CH_VALUE_MIN)/2)

#define    CH_MIN       ((int16_t)-660)
#define    CH_OFFSET    ((int16_t)   0)
#define	   CH_MAX       ((int16_t) 660)

/* ----------------------- RC Switch Definition-------------------------------*/

#define    SW_UP              ((uint16_t)1)
#define    SW_MID             ((uint16_t)3)
#define    SW_DOWN            ((uint16_t)2)

/* ----------------------- Function Definition-------------------------------- */
/* ң��ҡ��ͨ��ƫ��ֵ */
#define			SW1						(rc_info.s1)
#define			SW2						(rc_info.s2)
#define			CH2_VALUE			(rc_info.ch2)
#define			CH3_VALUE			(rc_info.ch3)
#define			CH0_VALUE			(rc_info.ch0)
#define			CH1_VALUE			(rc_info.ch1)
#define			TW_VALUE			(rc_info.thumbwheel)
#define			TW_CNT			  (rc_info.TW)
/*����ֵ��Ӧͨ��*/
#define			CH2_VALUE_K		(rc_sensor.info->Ch[2])
#define			CH3_VALUE_K		(rc_sensor.info->Ch[3])
#define			CH0_VALUE_K		(rc_sensor.info->Ch[0])
#define			CH1_VALUE_K		(rc_sensor.info->Ch[1])

/* ���ң��������״̬ */
#define     SW1_UP      (rc_info.s1 == SW_UP)
#define     SW1_MID     (rc_info.s1 == SW_MID)
#define     SW1_DOWN    (rc_info.s1 == SW_DOWN)
#define     SW2_UP      (rc_info.s2 == SW_UP)
#define     SW2_MID     (rc_info.s2 == SW_MID)
#define     SW2_DOWN    (rc_info.s2 == SW_DOWN)

/* ���� */

#define KEY_W                rc_sensor.info->kb.bit.W		
#define KEY_S                rc_sensor.info->kb.bit.S		
#define KEY_A                rc_sensor.info->kb.bit.A		
#define KEY_D                rc_sensor.info->kb.bit.D	
#define KEY_SHIFT            rc_sensor.info->kb.bit.SHIFT	
#define KEY_CTRL             rc_sensor.info->kb.bit.CTRL		
#define KEY_Q                rc_sensor.info->kb.bit.Q		
#define KEY_E                rc_sensor.info->kb.bit.E		
#define KEY_R                rc_sensor.info->kb.bit.R		
#define KEY_F                rc_sensor.info->kb.bit.F		
#define KEY_G                rc_sensor.info->kb.bit.G		
#define KEY_Z                rc_sensor.info->kb.bit.Z		
#define KEY_X                rc_sensor.info->kb.bit.X		
#define KEY_C                rc_sensor.info->kb.bit.C		
#define KEY_V                rc_sensor.info->kb.bit.V		
#define KEY_B                rc_sensor.info->kb.bit.B		

#define KEY_ALL_CODE         rc.info->kb.key_code

/*������־λ*/

#define W_FLAG       rc_sensor.info->Key_Flg[W]
#define S_FLAG       rc_sensor.info->Key_Flg[S]
#define A_FLAG       rc_sensor.info->Key_Flg[A]
#define D_FLAG       rc_sensor.info->Key_Flg[D]
#define SHIFT_FLAG   rc_sensor.info->Key_Flg[SHIFT]
#define CTRL_FLAG    rc_sensor.info->Key_Flg[CTRL]
#define Q_FLAG       rc_sensor.info->Key_Flg[Q]
#define E_FLAG       rc_sensor.info->Key_Flg[E]
#define R_FLAG       rc_sensor.info->Key_Flg[R]
#define F_FLAG       rc_sensor.info->Key_Flg[F]
#define G_FLAG       rc_sensor.info->Key_Flg[G]
#define Z_FLAG       rc_sensor.info->Key_Flg[Z]
#define X_FLAG       rc_sensor.info->Key_Flg[X]
#define C_FLAG       rc_sensor.info->Key_Flg[C]
#define V_FLAG       rc_sensor.info->Key_Flg[V]
#define B_FLAG       rc_sensor.info->Key_Flg[B]
/*����ʱ�� ���̰����ж�*/
#define CHANGE_TIM                      5    //ms
#define LONG_CHANGE_TIM_W               1000   //ms
#define LONG_CHANGE_TIM_S               1000   //ms
#define LONG_CHANGE_TIM_A               1000   //ms
#define LONG_CHANGE_TIM_D               1000   //ms
#define LONG_CHANGE_TIM_Q               500    //ms
#define LONG_CHANGE_TIM_E               500    //ms
#define LONG_CHANGE_TIM_R               500    //ms
#define LONG_CHANGE_TIM_F               500    //ms
#define LONG_CHANGE_TIM_G               500    //ms
#define LONG_CHANGE_TIM_Z               500    //ms
#define LONG_CHANGE_TIM_X               500    //ms
#define LONG_CHANGE_TIM_C               500    //ms
#define LONG_CHANGE_TIM_V               500    //ms
#define LONG_CHANGE_TIM_B               500    //ms
#define LONG_CHANGE_TIM_CTRL            500   //ms
#define LONG_CHANGE_TIM_SHIFT           500    //ms
#define LONG_CHANGE_TIM_MOUSE_L         230    //ms
#define LONG_CHANGE_TIM_MOUSE_R         500    //ms

/*����ٶ����ֵ����*/
#define Xmax    300
#define Ymax    300
/*��껬���˲�����*/
#define SF_LENGTH 15

/*���������ƶ��ٶ�*/
#define    MOUSE_X_MOVE_SPEED    (rc_sensor.info->mouse.x)
#define    MOUSE_Y_MOVE_SPEED    (rc_sensor.info->mouse.y)
#define    MOUSE_Z_MOVE_SPEED    (rc_sensor.info->mouse.z)

/* �����갴��״̬ */
#define    MOUSE_LEFT    (rc_sensor.info->mouse.press_l )
#define    MOUSE_RIGH    (rc_sensor.info->mouse.press_r )

/* �����̰���״̬ */
#define KEY_UP                    0x00
#define KEY_DOWN                  0x01

/* ��ⰴ��������Ƿ��а��� */
#define KEY_CTRL_STATE (KEY_ALL_CODE || MOUSE_LEFT || MOUSE_RIGH)


/* ���̱�־λö�� */
typedef enum{
	W,
	S,
	A,
	D,
	Q,       //��ת90��
	E,       //��ת90��
	R,       //�򿪵���
	F,       //С����
	G,       //����̧ͷ
	Z,
	X,
	C,       //�Ӿ�ģʽ�л�
	V,       //���Һ�ת180��
	B,       //��λģʽ��
	CTRL,    //��еģʽ�л�
	SHIFT,   //����ģʽ�л�
	Mouse_L,
	Mouse_R,  	
	KEY_CNT,
}key_t;

/*
*  ������Ϣ�ṹ��
*
*
*/
typedef enum 
{
  UP = 0,   //0
  PRESS,    //0->1
  SHORT_DOWN,//1�̰�
  DOWN,      //1����
  RELAX,    //1->0
}KEY_State_t;

typedef struct 
{
  int         state_cnt ;
  KEY_State_t State;
  KEY_State_t prev_State;
  uint8_t     prev_KEY_PRESS;
  uint8_t     KEY_PRESS;
}KEY_Info_t;

typedef struct
{
  float Slope;
  float MoveData;
  float K;
}KEY_DirInfo_t;

typedef struct
{
  KEY_DirInfo_t FB_dir;
  KEY_DirInfo_t LR_dir;
}KEY_MoveInfo_t;

typedef struct
{
  float SFX[SF_LENGTH]; 
  float SFY[SF_LENGTH]; 
  float SFZ[SF_LENGTH];	
}Mouse_SF_t;

typedef struct Keyboard_Info_struct {
  KEY_Info_t     W;
  KEY_Info_t     S;
  KEY_Info_t     A;
  KEY_Info_t     D;
  KEY_Info_t     Q;       //��ת90��
  KEY_Info_t     E;       //��ת90��
  KEY_Info_t     R;       //�򿪵���
  KEY_Info_t     F;       //С����
  KEY_Info_t     G;       //����̧ͷ
  KEY_Info_t     Z;
  KEY_Info_t     X;
  KEY_Info_t     C;       //�Ӿ�ģʽ�л�
  KEY_Info_t     V;       //���Һ�ת180��
  KEY_Info_t     B;       //��λģʽ��
  KEY_Info_t     CTRL;    //��еģʽ�л�
  KEY_Info_t     SHIFT;   //����ģʽ�л�
  KEY_Info_t     Mouse_L;
  KEY_Info_t     Mouse_R;  
  KEY_MoveInfo_t MoveInfo;
  Mouse_SF_t     MouseSF; 
}Keyboard_Info_t;


typedef struct rc_sensor_info_struct {
	
	int16_t 	ch0;
	int16_t 	ch1;
	int16_t 	ch2;
	int16_t 	ch3;
	
	int16_t 	thumbwheel;	
	
	uint8_t  	s1;
	uint8_t  	s2;
	
	struct
  {
      int16_t x;
      int16_t y;
      int16_t z;
      uint8_t press_l;
      uint8_t press_r;
  }mouse;

	union 
	{
		uint16_t key_code;
		struct
		{
			uint16_t W : 1;//ð�ű�ʾλ��
			uint16_t S : 1;
			uint16_t A : 1;
			uint16_t D : 1;
			uint16_t SHIFT : 1;
			uint16_t CTRL : 1;
			uint16_t Q : 1;
			uint16_t E : 1;
			uint16_t R : 1;
			uint16_t F : 1;
			uint16_t G : 1;
			uint16_t Z : 1;
			uint16_t X : 1;
			uint16_t C : 1;
			uint16_t V : 1;
			uint16_t B : 1;
		} bit;
	} kb;	

	float Ch[4];	           /*����ģ��ͨ��*/
	float	Mouse_ROLL;
	
	char  Key_Flg[KEY_CNT];  /*���̱�־λ*/

	
	int16_t		offline_cnt;
	int16_t		offline_max_cnt;

} rc_sensor_info_t;

typedef struct rc_sensor_dial_struct {
	
	char 	This_LeftDial_Mode;
	char 	Last_LeftDial_Mode;
	
	char 	This_RightDial_Mode;
	char 	Last_RightDial_Mode;
	
	char  Mechanical_Mode;
	char  Imu_Mode;
	
	char  Keyboard_Mode;
	
	char	This_Gryo_Mode;
	char	Last_Gryo_Mode;
	
	char  ThumbWheel_Up_Toggle;
	char  ThumbWheel_Down_Toggle;
	
	char  Continue_ShootEnable;
	char  FricWheel_WorkEnable;
	char 	Single_ShootEnable;
	
	char  Version_Control_Enable; 
	
	char	Var_Change_Enable;
	
	char  Continue_Shoot_BlockFlag;
	char  Single_Shoot_BlockFlag;	
	
	char  ContinueShoot_Block_ToggleDir;
	char  SingleShoot_Block_ToggleDir;
	
	char  Cover_OpenEnable;
	
	uint32_t  ContinueShoot_Cnt;
	uint32_t  SingleShoot_Cnt;
	
} rc_sensor_dial_t;


typedef struct rc_sensor_struct {
	
	rc_sensor_info_t	*info;
	rc_sensor_dial_t  *dial;
	drv_uart_t			  *driver;
	
	void				(*init)(struct rc_sensor_struct *self);
	void				(*update)(struct rc_sensor_struct *self, uint8_t *rxBuf);
	void				(*check)(struct rc_sensor_struct *self);	
	void				(*heart_beat)(struct rc_sensor_struct *self);
	void				(*dial_reset)(struct rc_sensor_struct *self);
	void				(*dial_jugde)(struct rc_sensor_struct *self);
	
	dev_work_state_t	work_state; 
	dev_state_tran_t  tran_state;///////////�ӵģ���������ϵ���е��λ
	dev_errno_t			errno;
	dev_id_t			id;
	
	
} rc_sensor_t;



extern Keyboard_Info_t Keyboard;
extern rc_sensor_info_t rc_sensor_info;
extern rc_sensor_dial_t rc_sensor_dial;
extern rc_sensor_t 		rc_sensor;
/* Exported functions --------------------------------------------------------*/

bool RC_IsChannelReset(void);
void RC_ResetData(rc_sensor_t *rc);
#endif
