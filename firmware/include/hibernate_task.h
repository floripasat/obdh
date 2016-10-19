/*
 * hibernate_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_HIBERNATE_TASK_H_
#define INCLUDE_HIBERNATE_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define HIBERNATE_TASK_PRIORITY          5
#define HIBERNATE_TASK_PERIOD_MS         1000
#define HIBERNATE_TASK_PERIOD_TICKS      ( HIBERNATE_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xHibernateTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xHibernateTask;

/**
 * \fn void prvHibernateTask( void *pvParameters )
 * The task turn off (power down mode) the ODBH module for some time
 * \param pvParameters Not used
 */
void prvHibernateTask( void *pvParameters );

#endif /* INCLUDE_HIBERNATE_TASK_H_ */
