#include "vision_sensor.h"

extern void vision_update(vision_sensor_t *vision, uint8_t *rxBuf);
extern void vision_init(vision_sensor_t *vision);
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void vision_check(vision_sensor_t *vision);
static void vision_heart_beat(vision_sensor_t *vision);

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
// 视觉驱动
drv_uart_t	vision_sensor_driver = {
	.type = DRV_UART3,
//	.tx_byte =  UART1_SendData,
};

// 视觉信息
vision_info_t 	vision_sensor_info = {
	.State.offline_max_cnt = 200,
};

// 视觉传感器
vision_sensor_t	vision_sensor = {
	.info = &vision_sensor_info,
	.init = vision_init,
	.update = vision_update,
	.check = vision_check,
	.heart_beat = vision_heart_beat,
	.work_state = DEV_OFFLINE,
	.id = DEV_ID_VISION	,
};

/* Private functions ---------------------------------------------------------*/
extern Vision_Cmd_Id_t AUTO_CMD_MODE;

static void vision_check(vision_sensor_t *vision)
{

	Vision_Rx_Data_t *Pack = &vision_sensor.info->RxPacket.RxData;

	memcpy(&vision_sensor.info->RxPacket,vision_sensor.info->RxPacket.RxData.data, LEN_VISION_RX_PACKET);
	
	if(Pack->pitch_angle == NULL)Pack->pitch_angle = (pitch + 180) * 22.7555f;  
	if(Pack->yaw_angle   == NULL)Pack->yaw_angle   = (yaw + 180) * 22.7555f;
	

	if(Pack->pitch_angle <= 8191 || Pack->pitch_angle >= 0)
		Pack->pitch_angle = Pack->pitch_angle;
	else                                                   	
		Pack->pitch_angle = (pitch + 180) * 22.7555f;
	
	
	if(Pack->yaw_angle   <= 8191 || Pack->yaw_angle   >= 0)
		Pack->yaw_angle   = Pack->yaw_angle ;
	else                                                   	 
		Pack->yaw_angle   = (yaw + 180) * 22.7555f;
	
	
	
	if(!Pack->is_find_Target && !Pack->is_find_Dafu)
	{
		Pack->pitch_angle = (pitch + 180) * 22.7555f; 
		Pack->yaw_angle   = (yaw + 180) * 22.7555f;
	}
	
	if(VISION_OFFLINE)
	{
		Pack->pitch_angle = (pitch + 180) * 22.7555f; 
		Pack->yaw_angle   = (yaw + 180) * 22.7555f;
	}
}






static void vision_heart_beat(vision_sensor_t *vision_sen)
{
	vision_info_t *vision_info = vision_sen->info;

	vision_info->State.offline_cnt++;
	if(vision_info->State.offline_cnt > vision_info->State.offline_max_cnt) 
	{
		vision_info->State.offline_cnt = vision_info->State.offline_max_cnt;
		vision_sen->work_state = DEV_OFFLINE;
	} 
	else 
	{
		/* 离线->在线 */
		if(vision_sen->work_state == DEV_OFFLINE)
			vision_sen->work_state = DEV_ONLINE;
	}	
	
}
/* Exported functions --------------------------------------------------------*/
