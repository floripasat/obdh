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


// | AUTHENTICATION |   CMD  |  DATA  |
// |      1 BYTE    | 1 BYTE | 4 BYTES|

//UPLINK COMMANDS
#define     SEND_DATA_REQUEST   0x01
#define     SHUTDOWN_REQUEST    0x02

#define OBDH_BYTE           0
#define EPS_BYTE            1
#define TTC_BYTE            2
#define PAYLOADS_BYTE       3

//DATA REQUEST PACKAGE
//OBDH
#define SYSTEM_STATUS_REQUEST           (0x01)
#define IMU_REQUEST                     (0x02)
#define MSP_SENSORS_REQUEST             (0x04)
#define SYSTICK_REQUEST                 (0x08)
#define SOLAR_PANELS_REQUEST            (0x10)
#define RTC_REQUEST                     (0x20)
#define RADIO_REQUEST                   (0x40)
#define PACKAGE_REQUEST                 (0x80)
//EPS
#define ADC_SOLAR_PANELS_REQUEST        (0x01)
#define MSP430_ADC_REQUEST              (0x02)
#define BATTERY_MONITOR_REQUEST         (0x04)
#define ADS1248_REQUEST                 (0x08)
#define TASK_SCHEDULER_REQUEST          (0x10)
//TT&C
#define BEACON_REQUEST                  (0x01)
#define TRANSCEIVER_REQUEST             (0x02)
//PAYLOADS
#define PAYLOAD1_REQUEST                (0x01)
#define PAYLOAD2_REQUEST                (0x02)


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

