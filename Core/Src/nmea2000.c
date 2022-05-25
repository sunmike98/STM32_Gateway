/*
 * nmea2000.c
 *
 *  Created on: 13 kwi 2022
 *      Author: Michal
 */
#include "nmea2000.h"
#include "can.h"

#define MY_EXT_ID 	23

#define NMEA2000_BATTERY_STATUS_MSG_ID	127508U
#define NMEA2000_EPRU_MSG_ID			127488U
#define NMEA2000_EPD_MSG_ID				127489U

#define NMEA2000_BATTERY_STATUS_MSG_DLC	8U
#define NMEA2000_EPRU_MSG_DLC			8U
#define NMEA2000_EPD_MSG_DLC			22U

CAN_HandleTypeDef hcan1;
CAN_TxHeaderTypeDef	TxHeader;

extern BLDC_Motor_Msg_t	BLDC_Motor_Msg_1;
extern BLDC_Motor_Msg_t	BLDC_Motor_Msg_2;

NMEA2000_Msg_t NMEA_2000_Msg_EPRU = {.Msg_ID = NMEA2000_EPRU_MSG_ID, .Msg_DLC = NMEA2000_EPRU_MSG_DLC};
NMEA2000_Msg_t NMEA_2000_Msg_BatStatus = {.Msg_ID = NMEA2000_BATTERY_STATUS_MSG_ID, .Msg_DLC = NMEA2000_BATTERY_STATUS_MSG_DLC};
NMEA2000_Msg_t NMEA_2000_Msg_EPD = {.Msg_ID = NMEA2000_EPD_MSG_ID, .Msg_DLC = NMEA2000_EPD_MSG_DLC};

void CAN_NMEA2000_Tx_Header_Config(uint32_t NMEA2000_Msg_Id, uint8_t NMEA2000_Msg_DLC)
{
	CAN_NMEA2000_Header_ID_t CAN_Tx_Header_ExtId = {
			.Priority = 6,
			/*.Reserved = 0,*/
			.MsgId = 0,
			.MyId = MY_EXT_ID
	};
	CAN_Tx_Header_ExtId.MsgId = NMEA2000_Msg_Id;

	TxHeader.DLC = NMEA2000_Msg_DLC;
	TxHeader.ExtId = *(uint32_t*)&CAN_Tx_Header_ExtId;//MY_EXT_ID;
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.StdId = 0;
	TxHeader.TransmitGlobalTime = DISABLE;
}
status_t NMEA2000_Transmit_Msg(NMEA2000_Msg_t* NMEA200_MsgType)
{
	uint32_t TxMailbox;
	status_t status = OK;

	CAN_NMEA2000_Tx_Header_Config(NMEA200_MsgType->Msg_ID, NMEA200_MsgType->Msg_DLC);

	NMEA_Set_Values(NMEA200_MsgType);

	if(HAL_OK != HAL_CAN_AddTxMessage(&hcan1, &TxHeader, (uint8_t*)&NMEA200_MsgType->NMEA2000_Msg_Data, &TxMailbox))
	{
		status = Error;
	}
	return status;
}
void NMEA_Set_Values(NMEA2000_Msg_t* NMEA2000_MsgType)
{
	if(NMEA2000_MsgType == &NMEA_2000_Msg_EPRU)
	{
		NMEA_2000_Msg_EPRU.NMEA2000_Msg_Data.NMEA2000_Msg_EPRU_Data.EngineInstance = 0x0;
		NMEA_2000_Msg_EPRU.NMEA2000_Msg_Data.NMEA2000_Msg_EPRU_Data.EngineSpeed = (110*BLDC_Motor_Msg_1.BLDC_Motor_Msg_1_Data.RotorSpeed/7000);
		NMEA_2000_Msg_EPRU.NMEA2000_Msg_Data.NMEA2000_Msg_EPRU_Data.EngineBoostPressure = 0xff;
		NMEA_2000_Msg_EPRU.NMEA2000_Msg_Data.NMEA2000_Msg_EPRU_Data.EngineTilt_Trim = 0xf;
	}
	else if(NMEA2000_MsgType == &NMEA_2000_Msg_BatStatus)
	{
		NMEA_2000_Msg_BatStatus.NMEA2000_Msg_Data.NMEA2000_Msg_BatStatus_Data.BatteryInstance = 0x01;
		NMEA_2000_Msg_BatStatus.NMEA2000_Msg_Data.NMEA2000_Msg_BatStatus_Data.BatteryVoltage = BLDC_Motor_Msg_1.BLDC_Motor_Msg_1_Data.BatteryVoltage;
		NMEA_2000_Msg_BatStatus.NMEA2000_Msg_Data.NMEA2000_Msg_BatStatus_Data.BatteryCurrent = BLDC_Motor_Msg_1.BLDC_Motor_Msg_1_Data.MotorCurrent;
		NMEA_2000_Msg_BatStatus.NMEA2000_Msg_Data.NMEA2000_Msg_BatStatus_Data.BatteryCaseTemperature = 0xff;
	}
	else if(NMEA2000_MsgType == &NMEA_2000_Msg_EPD)
	{
		NMEA_2000_Msg_EPD.NMEA2000_Msg_Data.NMEA2000_Msg_EPD_Data.EngineInstance = 0x0;
		NMEA_2000_Msg_EPD.NMEA2000_Msg_Data.NMEA2000_Msg_EPD_Data.EngineOilPressure = 0x01;
		NMEA_2000_Msg_EPD.NMEA2000_Msg_Data.NMEA2000_Msg_EPD_Data.EngineOilTemp = 0x01;
		NMEA_2000_Msg_EPD.NMEA2000_Msg_Data.NMEA2000_Msg_EPD_Data.EngineTemp = 0x01;
	}
}
