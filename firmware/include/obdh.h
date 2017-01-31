/*
 * obdh.h
 *
 *  Created on: 8 de set de 2016
 *      Author: Fsat
 */

#ifndef INCLUDE_OBDH_H_
#define INCLUDE_OBDH_H_


#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "../hal/obdh_hal.h"
#include "../driver/clocks.h"
#include "../driver/adc.h"
#include "../interface/antenna.h"
#include "../interface/wdte.h"
#include "../src/communications_task.h"
#include "../src/debug_task.h"
#include "../src/eps_interface_task.h"
#include "../src/imu_interface_task.h"
#include "../src/read_internal_sensors_task.h"
#include "../src/save_data_on_flash_memory_task.h"
#include "../src/solar_panels_interface_task.h"
#include "../src/ttc_interface_task.h"
#include "../src/wdt_task.h"


//void vDeployAntenna();
void create_tasks();
void setup_hardware();


#endif

