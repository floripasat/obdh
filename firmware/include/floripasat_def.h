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
#define  TRANSCEIVER_FLAG           BIT5

#define  ADC_SOLAR_PANELS_FLAG      BIT6
#define  MSP430_ADC_FLAG            BIT7
#define  BATTERY_MONITOR_FLAG       BIT8
#define  ADS1248_FLAG               BIT9
#define  TASK_SCHEDULER_FLAG        BITA

#define  PAYLOAD1_FLAG              BITB
#define  PAYLOAD2_FLAG              BITC
//! \} End of flags

/**
 * \struct request_packet_t
 *
 * \brief The struct to decode a request command
 */
typedef struct {
    uint16_t flags;         /**< modules flags to request just the wanted data*/
    uint8_t packages_count; /**< number of packages to read, from offset */
    uint8_t packages_origin;/**< position used as reference for the offset */
    int32_t packages_offset;/**< number of packages to offset from origin */
} request_data_packet_t;

typedef struct {
    uint8_t ID[6];
    uint16_t request_action; /**< the action (send data, shutdown..) */
    uint8_t arguments[8];
    uint8_t reserved[12];
} telecommand_t;

typedef struct {
    uint16_t package_flags;
    //obdh
    uint8_t system_status      [6];
    uint8_t imu                [14];
    uint8_t msp_sensors        [6];
    uint8_t systick            [4];
    uint8_t solar_panels       [12];
    uint8_t transceiver        [90];
    //eps
    uint8_t adc_solar_panels   [18];
    uint8_t msp430_adc         [8];
    uint8_t battery_monitor    [21];
    uint8_t ads1248            [21];
    uint8_t task_scheduler     [1];
    //payloads
    uint8_t payload1           [7];
    uint8_t payload2           [100]; 
} data_packet_t;

/**
 * \defgroup current_state_masks
 * \{
 */
#define OPERATION_MODE_MASK         0xF0    /**< to retrieve from current state register just the operation mode */
#define ENERGY_LEVEL_MASK           0x0F    /**< to retrieve from current state register just the energy level */
//! \} End of current_state_masks

/**
 * \defgroup operation_modes
 * \brief valid operation modes of the satellite
 * \{
 */
#define FULL_OPERATION_MODE         0x10    /**< Normal operation               */
#define TELECOMMAND_MODE            0x20    /**< While answering a telecommand  */
#define SHUTDOWN_MODE               0x30    /**< 24-hours shutdown              */
#define ANTENNA_DEPLOYMENT_MODE     0x40    /**< While waiting to do the deployment (45 minutes) */
//! \} End of operation_modes


/**
 * \defgroup energy_levels
 * \brief valid operation modes of the satellite
 * \{
 */
#define ENERGY_L1_MODE              0x01    /**< Energy level 1 - Tx on , Beacon 10s, TLM 60s , Payload on  */
#define ENERGY_L2_MODE              0x02    /**< Energy level 2 - Tx on , Beacon 10s, TLM 60s , Payload off */
#define ENERGY_L3_MODE              0x03    /**< Energy level 3 - Tx on , Beacon 20s, TLM 120s, Payload off */
#define ENERGY_L4_MODE              0x04    /**< Energy level 4 - Tx off, Beacon 30s, TLM off , Payload off */
//! \} End of energy_levels

/**
 * \defgroup uplink_commands
 * \brief valid commands to be sent from the earth segment
 * \{
 */
#define REQUEST_DATA_TELECOMMAND            0x7764    /**< uplink command to request whole data orbit - dw */
#define REQUEST_SHUTDOWN_TELECOMMAND        0x6473    /**< uplink command to request a 24 hours shutdown - sd */
#define REQUEST_PING_TELECOMMAND            0x6770    /**< ping request - pg */
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

