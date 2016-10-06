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

#define EPS_DATA_LENGTH 30
#define IMU_DATA_LENGTH 30
#define TTC_DATA_LENGTH 30
#define TEMP_SENS_DATA_LENGTH 70

#define UART_PACKAGE_LENGTH 160 //soma dos anteriores **** mudar posteriormente

char eps_data[EPS_DATA_LENGTH];
char imu_data[IMU_DATA_LENGTH];
char ttc_data[TTC_DATA_LENGTH];
char temp_sens_data[TEMP_SENS_DATA_LENGTH];


#endif /* GLOBALS_H_ */

