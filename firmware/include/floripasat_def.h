/*
 * floripasat_def.h
 *
 *  Created on: 8 de set de 2016
 *      Author: elder
 */

#ifndef FLORIPASATDEF_H_
#define FLORIPASATDEF_H_

#include "stdint.h"

typedef struct {
    unsigned system_status_flag:1;
    unsigned imu_flag:1;
    unsigned msp_sensors_flag:1;
    unsigned systick_flag:1;
    unsigned solar_panels_flag:1;
    unsigned rtc_flag:1;
    unsigned radio_flag:1;

    unsigned adc_solar_panels_flag:1;
    unsigned msp430_adc_flag:1;
    unsigned battery_monitor_flag:1;
    unsigned ads1248_flag:1;
    unsigned task_scheduler_flag:1;

    unsigned beacon_flag:1;
    unsigned transceiver_flag:1;

    unsigned payload1_flag:1;
    unsigned payload2_flag:1;
} module_flags_t;

typedef struct {
    union {
        module_flags_t flags;
        uint32_t flags_byte;
    };
    uint8_t packages_count;
    uint8_t packages_origin;
    int32_t packages_offset;
} request_data_packet_t;

typedef struct {
    uint8_t request_action;
    uint8_t request_data[9];
} request_packet_t;


typedef struct {
    uint8_t package_flags      [4];
    //obdh
    uint8_t system_status      [5];
    uint8_t imu                [20];
    uint8_t msp_sensors        [6];
    uint8_t systick            [4];
    uint8_t solar_panels       [42];
    uint8_t rtc                [8];
    uint8_t radio              [100];
    //eps
    uint8_t adc_solar_panels   [18];
    uint8_t msp430_adc         [8];
    uint8_t battery_monitor    [21];
    uint8_t ads1248            [21];
    uint8_t task_scheduler     [1];
    //tt&c
    uint8_t beacon             [3];
    uint8_t transceiver        [4];
    //payloads
    uint8_t payload1           [7];
    uint8_t payload2           [100]; 
} data_packet_t;


//UPLINK COMMANDS
#define SEND_DATA_REQUEST           0x44
#define SHUTDOWN_REQUEST            0x53

#define OLDER_PACKAGES_ORIGIN       0
#define NEWER_PACKAGES_ORIGIN       1

//INTERFACES
#define IMU1    0x01
#define IMU2    0x02
#define EPS     0x03
#define TTC     0x04
#define SD      0x05

volatile data_packet_t satellite_data;


#endif /* FLORIPASATDEF_H_ */

