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


//INTERFACES
#define IMU1    0x01
#define IMU2    0x02
#define EPS     0x03
#define TTC     0x04
#define SD      0x05


#define TTC_DATA_LENGTH     30
#define OBDH_DATA_LENGTH     7  //  7 B of payload
#define EPS_DATA_LENGTH     23  // 17 B of payload + 2 * 3 Bytes of SOF and EOF

#define RADIO_DATA_LENGTH    4  // 2 B to counter + 2 B fo signal dB
#define MSP_INTERNAL_DATA_LENGTH 60

char eps_data[EPS_DATA_LENGTH];

char ttc_data[TTC_DATA_LENGTH];
char msp_internal_data[MSP_INTERNAL_DATA_LENGTH];

#define     USE_IMU_1   1

#if USE_IMU_1 == 1
    #define IMU_DATA_LENGTH     70  // 6 B to Acc + 2 B to Temp + 6 B to Gyr
    char imu_data[IMU_DATA_LENGTH];
#endif
// TODO: FAZER UM ARQUIVO QUE INCLUA SOMENTE OS MODULOS DE ACORDO COM A UM CABEÇALHO DE #DEFINES


#endif /* GLOBALS_H_ */

