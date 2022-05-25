/**
  ******************************************************************************
  * @file    can.h
  * @brief   This file contains all the function prototypes for
  *          the can.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN Private defines */
typedef enum
{
	OK,
	Error
}status_t;

typedef struct
{
	uint8_t 	MyId : 8;
	uint32_t 	MsgId : 17;
	uint8_t 	/*Reserved*/ : 1;
	uint8_t 	Priority : 3;
	uint8_t				 : 3;

}CAN_NMEA2000_Header_ID_t;

typedef struct
{
	uint16_t 	RotorSpeed;
	uint8_t 	MotorCurrent;
	uint8_t 	BatteryVoltage;

}BLDC_Motor_Msg_1_Data_t;

typedef struct
{
	uint8_t 	ThrottleSignal;
	int8_t 		ControllerTemp;
	int8_t 		MotorTemp;

}BLDC_Motor_Msg_2_Data_t;

typedef struct
{
	uint32_t MsgId;

	BLDC_Motor_Msg_1_Data_t BLDC_Motor_Msg_1_Data;
	BLDC_Motor_Msg_2_Data_t BLDC_Motor_Msg_2_Data;

}BLDC_Motor_Msg_t;


/* USER CODE END Private defines */

void MX_CAN1_Init(void);

/* USER CODE BEGIN Prototypes */
void CAN_Start(void);
void CAN_ActivateNotification(void);
void CAN_Filter_Config(void);
status_t CAN_Transmit_Msg(uint8_t TxData[]);
status_t CAN_Receive_Msg(uint8_t RxData[]);
void BLDC_Set_Received_Values(uint8_t CAN_Rx_Data[]);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
