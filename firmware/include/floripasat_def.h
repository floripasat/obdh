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

// UPLINK DATA PACKET
// | AUTHENTICATION |   CMD  |  DATA  |
// |      1 BYTE    | 1 BYTE | 4 BYTES|

//UPLINK COMMANDS
#define SEND_DATA_REQUEST               0x01
#define SHUTDOWN_REQUEST                0x02

#define OBDH_BYTE                       0
#define EPS_BYTE                        1
#define TTC_BYTE                        2
#define PAYLOADS_BYTE                   3

//DATA REQUEST PACKAGE
//OBDH
#define PACKAGE_REQUEST                 (0x01)
#define SYSTEM_STATUS_REQUEST           (0x02)
#define IMU_REQUEST                     (0x04)
#define MSP_SENSORS_REQUEST             (0x08)
#define SYSTICK_REQUEST                 (0x10)
#define SOLAR_PANELS_REQUEST            (0x20)
#define RTC_REQUEST                     (0x40)
#define RADIO_REQUEST                   (0x80)
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

#define HEADER_SIZE                     6
#define PACKAGE_FLAGS_SIZE              4

//OBDH
#define SYSTEM_STATUS_SIZE              4
#define IMU_SIZE                        20
#define MSP_SENSORS_SIZE                6
#define SYSTICK_SIZE                    4
#define SOLAR_PANELS_SIZE               42
#define RTC_SIZE                        8
#define RADIO_SIZE                      100

//EPS
#define ADC_SOLAR_PANELS_SIZE           18
#define MSP430_ADC_SIZE                 8
#define BATTERY_MONITOR_SIZE            21
#define ADS1248_SIZE                    21
#define TASK_SCHEDULER_SIZE             1
//TT&C
#define BEACON_SIZE                     3
#define TRANSCEIVER_SIZE                4
//PAYLOADS
#define PAYLOAD1_SIZE                   7
#define PAYLOAD2_SIZE                   0 //TODO: fix

#define PACKAGE_FLAGS_OFFSET           HEADER_SIZE
#define SYSTEM_STATUS_OFFSET           PACKAGE_FLAGS_OFFSET + PACKAGE_FLAGS_SIZE
#define IMU_OFFSET                     SYSTEM_STATUS_OFFSET + SYSTEM_STATUS_SIZE
#define MSP_SENSORS_OFFSET             IMU_OFFSET + IMU_SIZE
#define SYSTICK_OFFSET                 MSP_SENSORS_OFFSET + MSP_SENSORS_SIZE
#define SOLAR_PANELS_OFFSET            SYSTICK_OFFSET + SYSTICK_SIZE
#define RTC_OFFSET                     SOLAR_PANELS_OFFSET + SOLAR_PANELS_SIZE
#define RADIO_OFFSET                   RTC_OFFSET + RTC_SIZE

//EPS
#define ADC_SOLAR_PANELS_OFFSET        RADIO_OFFSET + RADIO_SIZE
#define MSP430_ADC_OFFSET              ADC_SOLAR_PANELS_OFFSET + ADC_SOLAR_PANELS_SIZE
#define BATTERY_MONITOR_OFFSET         MSP430_ADC_OFFSET + MSP430_ADC_SIZE
#define ADS1248_OFFSET                 BATTERY_MONITOR_OFFSET + BATTERY_MONITOR_SIZE
#define TASK_SCHEDULER_OFFSET          ADS1248_OFFSET + ADS1248_SIZE
//TT&C
#define BEACON_OFFSET                  TASK_SCHEDULER_OFFSET + TASK_SCHEDULER_SIZE
#define TRANSCEIVER_OFFSET             BEACON_OFFSET + BEACON_SIZE
//PAYLOADS
#define PAYLOAD1_OFFSET                TRANSCEIVER_OFFSET + TRANSCEIVER_SIZE
#define PAYLOAD2_OFFSET                PAYLOAD1_OFFSET + PAYLOAD1_SIZE

#define PACKAGE_TOTAL_SIZE             PAYLOAD2_OFFSET + PAYLOAD2_SIZE

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

