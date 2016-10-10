/*
 * fast_tasks.h
 *
 *  Created on: 8 de set de 2016
 *      Author: Fsat
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdio.h>
#include <stdint.h>

#define TTC_DATA_LENGTH     30
#define OBDH_DATA_LENGTH     7  //  7 B of payload
#define EPS_DATA_LENGTH     23  // 17 B of payload + 2 * 3 Bytes of SOF and EOF
#define IMU_DATA_LENGTH     14  // 6 B to Acc + 6 B to Gyr
#define RADIO_DATA_LENGTH    4  // 2 B to counter + 2 B fo signal dB
#define TEMP_SENS_DATA_LENGTH 80

char     obdhData[OBDH_DATA_LENGTH];
char      imuData[IMU_DATA_LENGTH];
char      epsData[EPS_DATA_LENGTH];
char    radioData[RADIO_DATA_LENGTH];
char      ttcData[TTC_DATA_LENGTH];
char tempSensData[TEMP_SENS_DATA_LENGTH];


#endif /* GLOBALS_H_ */

