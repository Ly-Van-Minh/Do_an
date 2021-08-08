/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm_log.h"
#include "misc.h"
#include "lora.h"
#include "light-sensor.h"
#include "data-format.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SENDER
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
USART_CLI_HandleTypedef_t uartCliHandle;
IWDG_HandleTypeDef hiwdg;
extern u8 ucSendData[PAYLOAD_LENGTH];
extern u8 ucReceivedData[PAYLOAD_LENGTH];
MainAppTypeDef_t mInfo = {
    .adcLightSensor = 0,
    .isRxDone = false,
    .isInit = false,
    .pTxData = ucSendData,
    .pRxData = ucReceivedData,
    .loraCurMode = UNKNOWN,
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  STM_LOGD("MainInit", "Reset cause: {%s}", resetCauseGetName(resetCauseGet()));
  // STM_LOGD("MainInit", "Watchdog Init {%ums}", iwdgInit(&hiwdg, WATCHDOG_TIME));
  vLoraInit();
  STM_LOGD("MainInit", "------START APPLICATION------");
  LORA_GET_REGISTER(RegSyncWord);
  LORA_GET_REGISTER(RegPayloadLength);

#ifndef SENDER
  /* Receiver */
  vModeInit(STDBY_MODE);
  vSpi1Write(RegFifoAddrPtr, FIFO_RX_BASE_ADDR);
  vModeInit(RXCONTINUOUS_MODE);
#endif /* !SENSOR */
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
// ADC_READ_LIGHTSENSOR();
#ifdef SENDER
    printf("\r\n");
    LoRaTransmit(mInfo.pTxData , PAYLOAD_LENGTH, 5000);
    HAL_Delay(1000);
#else  /* !SENDER */
    if (mInfo.isRxDone)
    {
      mInfo.isRxDone = false;
      if ((ucSpi1Read(RegIrqFlags) & RX_DONE_Msk) >> RX_DONE_MskPos)
      {
        /* PAYLOAD_CRC CHECK */
        u8 temp = ucSpi1Read(RegIrqFlags);
        if ((temp & PAYLOAD_CRC_ERROR_Msk) >> PAYLOAD_CRC_ERROR_MskPos == 1)
        {
          STM_LOGE("ReceiveErr", "Payload CRC Failed");
        }
        else
        {
          vSpi1Write(RegFifoAddrPtr, ucSpi1Read(RegFifoRxCurrentAddr));
          for (size_t i = 0u; i < PAYLOAD_LENGTH; i++)
          {
            LORA_GET_REGISTER(RegFifoAddrPtr);
            *(mInfo.pRxData + i) = ucSpi1Read(RegFifo);
            STM_LOGV("Transmit", "data receive[%d]: 0x%x", i, *(mInfo.pRxData + i));
          }
        }
      }
      /* CLEAR RX_DONE FLAG */
      vSpi1Write(RegIrqFlags, RX_DONE_Msk | PAYLOAD_CRC_ERROR_Msk);
    }
#endif /* !SENDER */
    HAL_IWDG_Refresh(&hiwdg);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void _Error_Handler(char *file, int line)
{
  __disable_irq();

  while (1)
  {
    STM_LOGE("ERROR", "Error file %s line %d", file, line);
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
    STM_LOGE("ERROR", "");
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
        ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
