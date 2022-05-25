
/*
 * nmea2000.h
 *
 *  Created on: 13 kwi 2022
 *      Author: Michal
 */

#ifndef INC_NMEA2000_H_
#define INC_NMEA2000_H_

#include <stdint.h>

/*typedef enum
{
	OK,
	Error
}status_t;*/

typedef struct
{
	uint8_t d;
	uint8_t x;

}NMEA2000_Msg_ZIOM_Data_t;

/* Message Battery_Status */
typedef struct
{
	uint8_t BatteryInstance;
	uint16_t BatteryVoltage;
	uint16_t BatteryCurrent;
	uint16_t BatteryCaseTemperature;
	uint8_t Reserved;

}NMEA2000_Msg_BatStatus_Data_t;

/* Message Engine_Param_Rapid_update */
typedef struct
{
	uint8_t  EngineInstance;
	uint16_t EngineSpeed;
	uint16_t EngineBoostPressure;
	uint8_t  EngineTilt_Trim;
	uint16_t Reserved;

}NMEA2000_Msg_EPRU_Data_t;

/* Message Engine_Param_Dynamic */
typedef struct
{
	uint8_t EngineInstance;
	uint16_t EngineOilPressure;
	uint16_t EngineOilTemp;
	uint16_t EngineTemp;
	uint16_t AlternatorPotential;
	uint16_t FuelRate;
	uint32_t TotalEngineHours;
	uint16_t EngineCoolantPressure;
	uint16_t FuelPressure;
	uint8_t Reserved;
	uint16_t EngineDiscreteStatus;

}NMEA2000_Msg_EPD_Data_t;

typedef union
{
	NMEA2000_Msg_EPRU_Data_t		NMEA2000_Msg_EPRU_Data;
	NMEA2000_Msg_ZIOM_Data_t		NMEA2000_Msg_ZIOM_Data;
	NMEA2000_Msg_BatStatus_Data_t	NMEA2000_Msg_BatStatus_Data;
	NMEA2000_Msg_EPD_Data_t			NMEA2000_Msg_EPD_Data;

}NMEA2000_Msg_Data_t;

typedef struct
{
	uint32_t Msg_ID;
	uint8_t Msg_DLC;

	NMEA2000_Msg_Data_t	NMEA2000_Msg_Data;

}NMEA2000_Msg_t;


void CAN_NMEA2000_Tx_Header_Config(uint32_t NMEA2000_Msg_Id, uint8_t NMEA2000_Msg_DLC);
void NMEA2000_Transmit_Msg(NMEA2000_Msg_t* NMEA200_MsgType);
void NMEA_Set_Values(NMEA2000_Msg_t* NMEA2000_MsgType);

#endif /* INC_NMEA2000_H_ */
