/*
 * store_data_task.h
 *
 *  Created on: 18 de out de 2016
 *      Author: elder
 */

#ifndef SRC_STORE_DATA_TASK_H_
#define SRC_STORE_DATA_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "../util/MMC_lib/mmc.h"
#include "../util/MMC_lib/hal_MMC_hardware_board.h"

/* TODO: remove the global variables in obdh_def*/
#include "../include/floripasat_def.h"


/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define STORE_DATA_TASK_PRIORITY          5
#define STORE_DATA_TASK_PERIOD_MS         1000
#define STORE_DATA_TASK_PERIOD_TICKS      ( STORE_DATA_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \var static xTaskHandle store_data_task_handle
 * \brief variable which holds the task reference
 */
static xTaskHandle store_data_task_handle;

/**
 * \fn void store_data_task( void *pvParameters )
 * The task witch do the storage data module main function
 * \param pvParameters Not used
 */
void store_data_task( void *pvParameters );
data_packet_t read_and_pack_data( void );
void store_data_on_flash( data_packet_t *packet );
uint16_t grab_packet(uint8_t* to_send_packet, request_data_packet_t *rqst_data_packet);
void update_last_read_position(uint32_t new_position);
void pack_module_data(uint8_t flag, uint8_t *module_data, uint8_t module_size, uint8_t* to_send_packet, uint16_t *total_package_size);


#endif /* SRC_STORE_DATA_TASK_H_ */
