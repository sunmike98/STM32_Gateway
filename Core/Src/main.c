/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "can.h"
#include "usart.h"
#include "gpio.h"
#include "nmea2000.h"

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

/* USER CODE BEGIN PV */

extern BLDC_Motor_Msg_t	BLDC_Motor_Msg_1;
extern BLDC_Motor_Msg_t	BLDC_Motor_Msg_2;
extern NMEA2000_Msg_t NMEA_2000_Msg_ZIOM;
extern NMEA2000_Msg_t NMEA_2000_Msg_EPRU;
extern NMEA2000_Msg_t NMEA_2000_Msg_EPD;
extern NMEA2000_Msg_t NMEA_2000_Msg_BatStatus;

uint8_t TxData[8];
uint8_t RxData[8];

CAN_RxHeaderTypeDef RxHeader;
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
  MX_USART3_UART_Init();
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */
  CAN_Start();
  CAN_ActivateNotification();
  CAN_Filter_Config();

  /* USER CODE END 2 */
  /*RxHeader.ExtId = 0xCF11E05;
  RxData[0] = 1;
  RxData[1] = 2;
  RxData[2] = 3;
  RxData[3] = 4;
  RxData[4] = 5;
  RxData[5] = 6;
  RxData[6] = 7;
  RxData[7] = 8;

  //BLDC_Set_Received_Values(RxData);
  BLDC_Motor_Msg_1.BLDC_Motor_Msg_1_Data.RotorSpeed = 1000;
  NMEA2000_Transmit_Msg1(&NMEA_2000_Msg_EPRU);
  //RxHeader.ExtId = 0xCF11E04;
  //BLDC_Set_Received_Values(RxData);
  BLDC_Motor_Msg_1.BLDC_Motor_Msg_1_Data.BatteryVoltage = 53;
  NMEA2000_Transmit_Msg1(&NMEA_2000_Msg_BatStatus);
  RxHeader.ExtId = 0xCF11E05;
  BLDC_Set_Received_Values(RxData);
  NMEA2000_Transmit_Msg1(&NMEA_2000_Msg_EPRU);
  RxHeader.ExtId = 0xCF11E04;
  BLDC_Set_Received_Values(RxData);
  NMEA2000_Transmit_Msg1(&NMEA_2000_Msg_BatStatus);
  */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 13;
  RCC_OscInitStruct.PLL.PLLN = 195;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_Receive_Msg(RxData);
	BLDC_Set_Received_Values(RxData);

	/*TxData[0] = 1;
		TxData[1] = 1;
		TxData[2] = (110 * (BLDC_Motor_Msg_1.BLDC_Motor_Msg_1_Data.RotorSpeed / 7000));
		TxData[3] = 1;
		TxData[4] = 1;*/

		NMEA2000_Transmit_Msg(&NMEA_2000_Msg_EPRU);
		NMEA2000_Transmit_Msg(&NMEA_2000_Msg_BatStatus);
		NMEA2000_Transmit_Msg(&NMEA_2000_Msg_EPD);

		//CAN_Transmit_Msg(TxData);

	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
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
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
