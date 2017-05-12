/*
 * floripasat_def.h
 *
 *  Created on: 8 de set de 2016
 *      Author: elder
 */

#ifndef FLORIPASATDEF_H_
#define FLORIPASATDEF_H_

#include "stdint.h"

#define has_flag(x,y)   (x & y)

/**
 * \defgroup flags
 * \brief submodules flags of the data packet
 * \{
 */
#define  SYSTEM_STATUS_FLAG         BIT0
#define  IMU_FLAG                   BIT1
#define  MSP_SENSORS_FLAG           BIT2
#define  SYSTICK_FLAG               BIT3
#define  SOLAR_PANELS_FLAG          BIT4
#define  RTC_FLAG                   BIT5
#define  RADIO_FLAG                 BIT6

#define  ADC_SOLAR_PANELS_FLAG      BIT7
#define  MSP430_ADC_FLAG            BIT8
#define  BATTERY_MONITOR_FLAG       BIT9
#define  ADS1248_FLAG               BITA
#define  TASK_SCHEDULER_FLAG        BITB

#define  BEACON_FLAG                BITC
#define  TRANSCEIVER_FLAG           BITD

#define  PAYLOAD1_FLAG              BITE
#define  PAYLOAD2_FLAG              BITF
//! \} End of flags

/**
 * \struct request_packet_t
 *
 * \brief The struct to decode a request command
 */
typedef struct {
    uint8_t request_action; /**< the action (send data, shutdown..) */
    uint16_t flags;         /**< modules flags to request just the wanted data*/
    uint8_t packages_count; /**< number of packages to read, from offset */
    uint8_t packages_origin;/**< position used as reference for the offset */
    int32_t packages_offset;/**< number of packages to offset from origin */
} request_packet_t;



typedef struct {
    uint16_t package_flags;
    //obdh
    uint8_t system_status      [6];
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



/**
 * \defgroup uplink_commands
 * \brief valid commands to be sent from the earth segment
 * \{
 */
#define SEND_DATA_REQUEST           0x44    /**< uplink command to request data */
#define SHUTDOWN_REQUEST            0x53    /**< uplink command to request a 24 hours shutdown */
//! \} End of uplink_commands


/**
 * \defgroup packet_origin
 * \brief used to refers to a position in the memory where are packet to read
 * \{
 */
#define OLDER_PACKAGES_ORIGIN       0   /**< refers to the older packets in the memory */
#define NEWER_PACKAGES_ORIGIN       1   /**< refers to the newer packets in the memory */
//! \} End of packet_origin

//INTERFACES
#define IMU1    0x01
#define IMU2    0x02
#define EPS     0x03
#define TTC     0x04
#define SD      0x05


#define _DEBUG_AS_LINK 1



#define BOOTING_MSG  "FSAT booting...\n Firmware v 0.1 - 15/03/2017\n\n"
#define CLOCK_INFO_MSG  " CLOCKS:\n  Master = 16MHz\n  Subsystem master = 16MHz\n  Auxiliary = 32768kHz \n\n"
#define CLOCK_FAIL_MSG  " *CLOCKS SETUP FAIL* \n\n"
#define UART_INFO_MSG  " UART:\n  Baudrate = 9600\n  Data bits = 8\n  Parity = None\n  Stop bits = 1\n\n"
#define I2C_INFO_MSG " I2C:\n  Freq =~ 100kHz\n\n"
#define SPI_INF_MSG  " SPI:\n  Freq =~ ---Hz\n\n"
#define ADC_INFO_MSG  " ADC:\n  Vref+ = 3.0V\n  Vref- = GND\n\n"

volatile data_packet_t satellite_data;


#endif /* FLORIPASATDEF_H_ */

