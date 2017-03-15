/*
 * wdt_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef SRC_WDT_TASK_H_
#define SRC_WDT_TASK_H_


#include "FreeRTOS.h"
#include "task.h"
#include "../driver/wdti.h"
#include "../interface/wdte.h"


#define WDT_TASK_PRIORITY          5        /**< Watchdog timer task priority */
#define WDT_TASK_PERIOD_MS         500      /**< Watchdog timer task period in miliseconds */
#define WDT_TASK_PERIOD_TICKS      ( WDT_TASK_PERIOD_MS / portTICK_PERIOD_MS ) /**< Watchdog timer task period in ticks */

/**
 * \var static xTaskHandle wdt_task_handle
 * \brief variable which holds the task reference, to allow it handling
 */
static xTaskHandle wdt_task_handle;

/**
 * \fn wdt_task( void *pvParameters )
 * That task reset the external and internal Watchdog Timers
 * \param pvParameters Not used
 * \return None
 */
void wdt_task( void *pvParameters );

#endif /* SRC_WDT_TASK_H_ */
