/*! \file obdh.h
    \brief This file gather the main functions of the OBDH.

*/

#ifndef INCLUDE_OBDH_H_
#define INCLUDE_OBDH_H_


#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "../src/task_queues.h"
#include "../hal/obdh_hal.h"
#include "../driver/clocks.h"
#include "../driver/adc.h"
#include "../driver/uart.h"
#include "../interface/antenna.h"
#include "../interface/wdte.h"
#include "../src/communications_task.h"
#include "../src/debug_task.h"
#include "../src/eps_interface_task.h"
#include "../src/imu_interface_task.h"
#include "../src/read_internal_sensors_task.h"
#include "../src/store_data_task.h"
#include "../src/solar_panels_interface_task.h"
#include "../src/ttc_interface_task.h"
#include "../src/wdt_task.h"

/**
 * \fn create_tasks
 *
 * \brief Instantiation of the OS tasks
 * \return None
 */
void create_tasks( void );

/**
 * \fn setup_hardware
 *
 * \brief Initialization of peripheral modules: internal WDT, external WDT, Clocks, I2C, SPI, UART, ADC.
 * \return None
 */
void setup_hardware( void );


#endif

