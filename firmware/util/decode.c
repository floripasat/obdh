/*
 * decode.c
 *
 *  Created on: 13 de fev de 2017
 *      Author: elder
 */

#include "decode.h"

void decode(uint8_t *package) {
    if(authenticate(package[0]) == true) {
        switch(package[1]) {
        case SEND_DATA_REQUEST: 
            pack_data(package + 2)
            break;
        case SHUTDOWN_REQUEST:
            shutdown(package + 2);
            break;
    }
}



bool authenticate(uint8_t authentication) {

    return true;
}

void pack_data(uint8_t *package) {
    switch(package[OBDH_BYTE)) {
    


    }

    switch(package[EPS_BYTE)) {
    


    }
}

#define OBDH_BYTE           0
#define EPS_BYTE            1
#define TTC_BYTE            2
#define PAYLOADS_BYTE       3

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

void shutdown(uint8_t *package) {
    uint32_t seconds_to_wake_up = (uint32_t* package)[0];

}
