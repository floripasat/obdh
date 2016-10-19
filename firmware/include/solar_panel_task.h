/*
 * solar_panel_task.h
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_SOLAR_PANEL_TASK_H_
#define INCLUDE_SOLAR_PANEL_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define SOLAR_PANEL_TASK_PRIORITY          5
#define SOLAR_PANEL_TASK_PERIOD_MS         1000
#define SOLAR_PANEL_TASK_PERIOD_TICKS      ( SOLAR_PANEL_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xSolarPanelTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xSolarPanelTask;

/**
 * \fn void prvSolarPanelTask( void *pvParameters )
 * That task manage the solar panels sensors
 * \param pvParameters Not used
 */
void prvSolarPanelTask( void *pvParameters );

#endif /* INCLUDE_SOLAR_PANEL_TASK_H_ */
