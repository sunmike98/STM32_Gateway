/*
 * nmea2000.h
 *
 *  Created on: 13 kwi 2022
 *      Author: Michal
 */

#ifndef INC_NMEA2000_H_
#define INC_NMEA2000_H_

#include <stdint.h>

/* Message Engine_Param_Rapid_update */
typedef struct
{
	uint32_t Msg_ID;
	uint8_t MsgData[5];

}NMEA_2000_Msg_EPRU_t;


#endif /* INC_NMEA2000_H_ */
