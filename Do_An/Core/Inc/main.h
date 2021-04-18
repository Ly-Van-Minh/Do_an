/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

  /* Private includes ----------------------------------------------------------*/
  /* USER CODE BEGIN Includes */
#include "stm_log.h"
#include "misc.h"
#include "stdbool.h"
  /* USER CODE END Includes */

  /* Exported types ------------------------------------------------------------*/
  /* USER CODE BEGIN ET */

  typedef uint32_t u32;
  typedef uint16_t u16;
  typedef uint8_t u8;

  typedef struct MainAppType
  {
    u16 adcLightSensor;
    bool isInit;
  } MainAppTypeDef;

  /* USER CODE END ET */

  /* Exported constants --------------------------------------------------------*/
  /* USER CODE BEGIN EC */

  /* USER CODE END EC */

  /* Exported macro ------------------------------------------------------------*/
  /* USER CODE BEGIN EM */

  /* USER CODE END EM */

  /* Exported functions prototypes ---------------------------------------------*/
  void Error_Handler(void);

  /* USER CODE BEGIN EFP */
  void _Error_Handler(char *file, int line);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LIGHT_SENSOR_OUTPUT_Pin GPIO_PIN_0
#define LIGHT_SENSOR_OUTPUT_GPIO_Port GPIOA
#define RELAY_OUTPUT_Pin GPIO_PIN_1
#define RELAY_OUTPUT_GPIO_Port GPIOA
#define BUTTON_INPUT_Pin GPIO_PIN_2
#define BUTTON_INPUT_GPIO_Port GPIOA
#define INTERRUPT_LORA_Pin GPIO_PIN_3
#define INTERRUPT_LORA_GPIO_Port GPIOA
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define LED_OUTPUT_Pin GPIO_PIN_0
#define LED_OUTPUT_GPIO_Port GPIOB

  /* USER CODE BEGIN Private defines */

#define NODE1_ADDRESS 0x11u   /* Address node 1 */
#define NODE2_ADDRESS 0x22u   /* Address node 2 */
#define NODE3_ADDRESS 0x33u   /* Address node 3 */
#define GATEWAY_ADDRESS 0x99u /* Adress Gateway */

#define RELAY_ON 0xAAu
#define RELAY_OFF 0xBBu

#define WHICH_ERROR(err) (err == HAL_ERROR) ? "HAL_ERROR" : ((err == HAL_BUSY) ? "HAL_BUSY" : "HAL_TIMEOUT")
#define TOGGLE_LED() HAL_GPIO_TogglePin(LED_OUTPUT_GPIO_Port, LED_OUTPUT_Pin)

#define ERROR_CHECK(ret)                                           \
  do                                                               \
  {                                                                \
    if (ret != HAL_OK)                                             \
    {                                                              \
      STM_LOGE("ERROR_CHECK_TAG", "[Error] %s", WHICH_ERROR(ret)); \
    }                                                              \
  } while (0)

  /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
