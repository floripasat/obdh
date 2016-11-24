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
#include "../MMC_lib/mmc.h"
#include "../MMC_lib/hal_MMC_hardware_board.h"

/* TODO: remove the global variables in obdh_def*/
#include "../include/obdh_def.h"


#define FLASH_PACKAGE_LENGTH    512
#define STATUS_PACKAGE_LENGTH    512
/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define SAVE_DATA_ON_FLASH_MEMORY_TASK_PRIORITY          5
#define SAVE_DATA_ON_FLASH_MEMORY_TASK_PERIOD_MS         1000
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
