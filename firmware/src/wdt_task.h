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

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define WDT_TASK_PRIORITY          5
#define WDT_TASK_PERIOD_MS         500
#define WDT_TASK_PERIOD_TICKS      ( WDT_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xWdtTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xWdtTask;

/**
 * \fn void prvWdtTask( void *pvParameters )
 * That task reset the external and internal WatchDog Timers
 * \param pvParameters Not used
 */
void wdtTask( void *pvParameters );

#endif /* SRC_WDT_TASK_H_ */
