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
#include "../driver/obdh_hal.h"
#include "../include/wdt_task.h"
#include "../include/antenna_task.h"
#include "../include/read_internal_sensors_task.h"
#include "../include/eps_interface_task.h"
#include "../include/imu_interface_task.h"
//#include "../include/ttc_interface_task.h"
#include "../include/communications_task.h"
#include "../include/solar_panels_interface_task.h"
#include "../include/save_data_on_flash_memory_task.h"

#include "../include/debug_task.h"

//#include "../driver/obdh_hal.h"


//void vDeployAntenna();
void vCreateTasks();
void vSetupHardware();
void vWDTsConfiguration();
//void vPrintEvent (char *string);


#endif

