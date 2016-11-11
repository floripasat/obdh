/*
 * save_data_on_flash_memory_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef INCLUDE_SAVE_DATA_ON_FLASH_MEMORY_TASK_H_
#define INCLUDE_SAVE_DATA_ON_FLASH_MEMORY_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define SAVE_DATA_ON_FLASH_MEMORY_TASK_PRIORITY          5
#define SAVE_DATA_ON_FLASH_MEMORY_TASK_PERIOD_MS         100
#define SAVE_DATA_ON_FLASH_MEMORY_TASK_PERIOD_TICKS      ( SAVE_DATA_ON_FLASH_MEMORY_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle xSaveDataOnFlashMemoryTask
 * \brief variable which holds the task reference
 */
static xTaskHandle xSaveDataOnFlashMemoryTask;

/**
 * \fn void prvSaveDataOnFlashMemoryTask( void *pvParameters )
 * The task witch do the SaveDataOnFlashMemory module main function
 * \param pvParameters Not used
 */
void saveDataOnFlashMemoryTask( void *pvParameters );

#endif /* INCLUDE_SAVE_DATA_ON_FLASH_MEMORY_TASK_H_ */
