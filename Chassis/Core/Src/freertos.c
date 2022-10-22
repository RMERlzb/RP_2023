/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId Chassis_TaskHandle;
osThreadId WorkStateTaskHandle;
osThreadId ImuTaskHandle;
osThreadId RC_HeartTaskHandle;
osThreadId RC_DialTaskHandle;
osThreadId Shoot_TaskHandle;
osThreadId Gimbal_TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartChassis_Task(void const * argument);
void StartWorkStateTask(void const * argument);
void StartImuTask(void const * argument);
void StartRC_HeartTask(void const * argument);
void StartRC_DialTask(void const * argument);
void StartShoot_Task(void const * argument);
void StartGimbal_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Chassis_Task */
  osThreadDef(Chassis_Task, StartChassis_Task, osPriorityNormal, 0, 128);
  Chassis_TaskHandle = osThreadCreate(osThread(Chassis_Task), NULL);

  /* definition and creation of WorkStateTask */
  osThreadDef(WorkStateTask, StartWorkStateTask, osPriorityNormal, 0, 128);
  WorkStateTaskHandle = osThreadCreate(osThread(WorkStateTask), NULL);

  /* definition and creation of ImuTask */
  osThreadDef(ImuTask, StartImuTask, osPriorityAboveNormal, 0, 128);
  ImuTaskHandle = osThreadCreate(osThread(ImuTask), NULL);

  /* definition and creation of RC_HeartTask */
  osThreadDef(RC_HeartTask, StartRC_HeartTask, osPriorityAboveNormal, 0, 128);
  RC_HeartTaskHandle = osThreadCreate(osThread(RC_HeartTask), NULL);

  /* definition and creation of RC_DialTask */
  osThreadDef(RC_DialTask, StartRC_DialTask, osPriorityAboveNormal, 0, 128);
  RC_DialTaskHandle = osThreadCreate(osThread(RC_DialTask), NULL);

  /* definition and creation of Shoot_Task */
  osThreadDef(Shoot_Task, StartShoot_Task, osPriorityNormal, 0, 128);
  Shoot_TaskHandle = osThreadCreate(osThread(Shoot_Task), NULL);

  /* definition and creation of Gimbal_Task */
  osThreadDef(Gimbal_Task, StartGimbal_Task, osPriorityNormal, 0, 128);
  Gimbal_TaskHandle = osThreadCreate(osThread(Gimbal_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartChassis_Task */
/**
* @brief Function implementing the Chassis_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartChassis_Task */
__weak void StartChassis_Task(void const * argument)
{
  /* USER CODE BEGIN StartChassis_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartChassis_Task */
}

/* USER CODE BEGIN Header_StartWorkStateTask */
/**
* @brief Function implementing the WorkStateTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartWorkStateTask */
__weak void StartWorkStateTask(void const * argument)
{
  /* USER CODE BEGIN StartWorkStateTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartWorkStateTask */
}

/* USER CODE BEGIN Header_StartImuTask */
/**
* @brief Function implementing the ImuTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartImuTask */
__weak void StartImuTask(void const * argument)
{
  /* USER CODE BEGIN StartImuTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartImuTask */
}

/* USER CODE BEGIN Header_StartRC_HeartTask */
/**
* @brief Function implementing the RC_HeartTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRC_HeartTask */
__weak void StartRC_HeartTask(void const * argument)
{
  /* USER CODE BEGIN StartRC_HeartTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartRC_HeartTask */
}

/* USER CODE BEGIN Header_StartRC_DialTask */
/**
* @brief Function implementing the RC_DialTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRC_DialTask */
__weak void StartRC_DialTask(void const * argument)
{
  /* USER CODE BEGIN StartRC_DialTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartRC_DialTask */
}

/* USER CODE BEGIN Header_StartShoot_Task */
/**
* @brief Function implementing the Shoot_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartShoot_Task */
__weak void StartShoot_Task(void const * argument)
{
  /* USER CODE BEGIN StartShoot_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartShoot_Task */
}

/* USER CODE BEGIN Header_StartGimbal_Task */
/**
* @brief Function implementing the Gimbal_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGimbal_Task */
__weak void StartGimbal_Task(void const * argument)
{
  /* USER CODE BEGIN StartGimbal_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartGimbal_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
