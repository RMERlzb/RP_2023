/**
 * @file        rp_device_config.h
 * @author      RobotPilots
 * @Version     v1.0
 * @brief       RobotPilots Robots' Device Configuration.
 * @update
 *              v1.0(7-November-2021)  
 */
#ifndef __RP_DEVICE_CONFIG_H
#define __RP_DEVICE_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "rp_user_config.h"
#include "rp_driver_config.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* �豸�� --------------------------------------------------------------------*/
/**
 *	@brief	�豸id�б�
 *	@class	device
 */
typedef enum {
	DEV_ID_NONE,
    DEV_ID_RC,   
    DEV_ID_IMU, 

		DEV_ID_JUDGE,
	  DEV_ID_VISION,
	
    DEV_ID_POWER_0,
    DEV_ID_POWER_1,
    DEV_ID_POWER_2,
    DEV_ID_POWER_3,
		
    DEV_ID_TURN_0,
    DEV_ID_TURN_1,
    DEV_ID_TURN_2,
    DEV_ID_TURN_3,		

    DEV_ID_FRIC_L,
    DEV_ID_FRIC_R,
    DEV_ID_BOX,
    DEV_ID_BARREL,
		
    DEV_ID_GIMB_Y,
    DEV_ID_GIMB_P,
		DEV_ID_CNT,
} dev_id_t;

/**
 *	@brief	���̵���豸����
 *	@class	device
 */
typedef enum {
	CHAS_LF,
	CHAS_RF,
	CHAS_LB,
	CHAS_RB,
	CHAS_MOTOR_CNT,
} chassis_motor_cnt_t;

/**
 *	@brief	��̨���
 *	@class	device
 */
typedef enum {
	GIMBAL_LOW, 
	GIMBAL_HIGH, 
	GIMBAL_MOTOR_CNT,
} gimbal_motor_cnt_t;
/**
 *	@brief	��̨�豸ID
 *	@class	device
 */
typedef enum {
	DEV_ID_GIMBAL_LOW = 1,
	DEV_ID_GIMBAL_HIGH = 2,
} dev_gimbal_id_t;

/**
 *	@brief	ǹ�ܵ��
 *	@class	device
 */
typedef enum {
	SHOOT_LRUB, 
	SHOOT_RRUB, 
	SHOOT_DIAL,
	SHOOT_GUN,
	SHOOT_MOTOR_CNT,
} shoot_motor_cnt_t;
/**
 *	@brief	ǹ�ܵ��
 *	@class	device
 */
typedef enum {
	DEV_ID_SHOOT_RRUB = 1,
	DEV_ID_SHOOT_LRUB = 2,
	DEV_ID_SHOOT_DIAL = 3,
	DEV_ID_SHOOT_GUN = 4,
} dev_shoot_id_t;

/**
 *	@brief	�豸����״̬(ͨ��)
 *	@class	device
 */
typedef enum {
	DEV_ONLINE,
	DEV_OFFLINE,
} dev_work_state_t;

typedef enum {
	DEV_BOOT,
	DEV_OFF,
} dev_state_tran_t;

typedef enum {
	Dir_Header,
	Dir_Tail,
} dev_dir_t;


typedef enum {
	Flase,
	True,
} dev_mode_state_t;


/**
 *	@brief	�������(ͨ��)
 *  @note   ���Զ����豸����������Ͳ��滻����errno�ı������ͣ���
 *          typedef enum {
 *              IMU_NONE_ERR,
 *              IMU_ID_ERR,
 *              IMU_COM_FAILED,
 *              IMU_DEV_NOT_FOUND,
 *              ...
 *          } imu_errno_t;
 *          
 *          typedef struct imu_sensor_struct {
 *              ...
 *	            imu_errno_t errno;
 *              ...	
 *          } imu_sensor_t;
 *	@class	device
 */
typedef enum {
	NONE_ERR,		// ����(�޴���)
	DEV_ID_ERR,		// �豸ID����
	DEV_INIT_ERR,	// �豸��ʼ������
	DEV_DATA_ERR,	// �豸���ݴ���
} dev_errno_t;

/**
 *	@brief	�豸�ṹ�嶨��ģ��
 *	@class	device
 */
typedef struct device {
	void				        *info;		// �Զ�������豸��Ϣ�ṹ��
	void				        *driver;	// �Զ�������豸�����ṹ��
	void				        (*init)(struct device *self);	// �豸��ʼ������
	void				        (*update)(struct device *self, uint8_t *rxBuf);	// �豸���ݸ��º���
	void				        (*check)(struct device *self);	// �豸���ݼ�麯��
	void				        (*heart_beat)(struct device *self);	// �豸������
	volatile dev_work_state_t	work_state;	// �豸����״̬
	volatile dev_errno_t		errno;		// ���Զ�������豸�������
	const    dev_id_t			id;			// �豸id
} device_t;

/**
 *	@brief	RM��׼�����Ϣ��ṹ��
 *	@class	device
 */
typedef struct motor_info_struct {
	uint16_t		angle;
	int16_t    	speed;
	int16_t			current;
  uint8_t    	temperature;
	uint16_t		angle_prev;
	int32_t			angle_sum;
	uint8_t			offline_cnt;
	uint8_t			offline_max_cnt;	
} motor_info_t;

typedef struct motor_struct {
	motor_info_t 	  		*info;
	drv_can_t				    *driver;
	
	pid_ctrl_t      	  *speed_pid;
	pid_ctrl_t      	  *angle_pid;
	pid_ctrl_t				  *anglesum_pid;
	

	void					    	(*info_init)(struct motor_struct *self);	
	void					    	(*info_update)(struct motor_struct *self, uint8_t *rxBuf);
	void					    	(*check)(struct motor_struct *self);	
	void					    	(*heart_beat)(struct motor_struct *self);////////////////////��û���
	void								(*speed_pid_update)(struct motor_struct *self);
	void								(*angle_pid_update)(struct motor_struct *self);
	void								(*anglesum_pid_update)(struct motor_struct *self);
	
	dev_dir_t           dir;/////////////��̨���õ�����ʼ��Ϊ��ǰ��ֻ��Imu_Mode_Chassis_Input��������
	dev_work_state_t   	work_state;
	dev_errno_t	    		errno;
} motor_t;

#endif
