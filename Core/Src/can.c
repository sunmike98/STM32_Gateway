/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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

/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
#define BLDC_CONTROLLER_ID 	0xCF11E05
#define MY_EXT_ID 			23

CAN_FilterTypeDef CAN_Filter_Cfg;
CAN_TxHeaderTypeDef	TxHeader;
CAN_RxHeaderTypeDef	RxHeader;

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 10;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_10TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0|GPIO_PIN_1);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void CAN_Start(void)
{
	if(HAL_OK != HAL_CAN_Start(&hcan1))
	{
		Error_Handler();
	}
}
void CAN_ActivateNotification(void)
{
	if(HAL_OK != HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING))
	{
		Error_Handler();
	}
}
void CAN_Filter_Config(void)
{



	CAN_Filter_Cfg.FilterActivation = CAN_FILTER_ENABLE;
	CAN_Filter_Cfg.FilterIdHigh = (BLDC_CONTROLLER_ID >> 13) & 0xFFFF;
	CAN_Filter_Cfg.FilterIdLow = (BLDC_CONTROLLER_ID << 3) & 0xFFF8;
	CAN_Filter_Cfg.FilterMaskIdHigh = (BLDC_CONTROLLER_ID >> 13) & 0xFFFF;
	CAN_Filter_Cfg.FilterMaskIdLow = (BLDC_CONTROLLER_ID << 3) & 0xFFF8;
	/*CAN_Filter_Cfg.FilterIdHigh = (BLDC_CONTROLLER_ID & 0xFF00) >> 16;
	CAN_Filter_Cfg.FilterIdLow = (BLDC_CONTROLLER_ID & 0x00FF) << 3;
	CAN_Filter_Cfg.FilterMaskIdHigh = (BLDC_CONTROLLER_ID & 0xFF00) >> 16;
	CAN_Filter_Cfg.FilterMaskIdLow = (BLDC_CONTROLLER_ID & 0x00FF) << 3;*/
	CAN_Filter_Cfg.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN_Filter_Cfg.FilterBank = 8;
	CAN_Filter_Cfg.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_Filter_Cfg.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_Filter_Cfg.SlaveStartFilterBank = 0;

	if(HAL_OK != HAL_CAN_ConfigFilter(&hcan1, &CAN_Filter_Cfg))
	{
		Error_Handler();
	}
}
void CAN_NMEA2000_Tx_Header_Config(uint32_t NMEA2000_Msg_Id)
{
	CAN_NMEA2000_Header_ID_t CAN_Tx_Header_ExtId = {
			.Priority = 6,
			/*.Reserved = 0,*/
			.MsgId = 0,
			.MyId = MY_EXT_ID
	};
	CAN_Tx_Header_ExtId.MsgId = NMEA2000_Msg_Id;

	TxHeader.DLC = 8;
	TxHeader.ExtId = *(uint32_t*)&CAN_Tx_Header_ExtId;//MY_EXT_ID;
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.StdId = 0;
	TxHeader.TransmitGlobalTime = DISABLE;
}
status_t CAN_Transmit_Msg(uint8_t* TxData)
{
	status_t status = OK;
	uint32_t TxMailbox;
	if(HAL_OK != HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox))
	{
		status = Error;
	}
	return status;
}
status_t CAN_Receive_Msg(uint8_t* RxData)
{
	status_t status = OK;
	if(HAL_OK != HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData))
	{
		status = Error;
	}
	return status;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
