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


#define STORE_DATA_TASK_PRIORITY          5             /**< Store data task priority */
#define STORE_DATA_TASK_PERIOD_MS         1000          /**< Store data task period in miliseconds */
#define STORE_DATA_TASK_PERIOD_TICKS      ( STORE_DATA_TASK_PERIOD_MS / portTICK_PERIOD_MS )    /**< Store data task period in ticks */

/**
 * \var static xTaskHandle store_data_task_handle
 * \brief variable which holds the task reference
 */
static xTaskHandle store_data_task_handle;

/**
 * \fn store_data_task
 * The task witch do the storage data module main function
 * \param pvParameters Not used
 * \return None
 */
void store_data_task( void *pvParameters );

/**
 * \fn read_and_pack_data
 * Read the submodules data and storage into a pack
 * \return One data packet with the last read data
 */
data_packet_t read_and_pack_data( void );

/**
 * \fn store_data_on_flash
 * Store the last read data packet into a non-volatile memory
 * \param Reference to a data packet
 * \return None
 */
void store_data_on_flash( data_packet_t *packet );

/**
 * \fn get_packet
 * Retrieve 1 packet data from the non-volatile memory, according the requested sobmodules data
 * \param to_send_packet is a pointer to the memory position where the requested data will be write
 * \param rqst_data_packet is a pointer to the request
 * \return length, in bytes, of the requested submodules data
 */
uint16_t get_packet(uint8_t* to_send_packet, request_packet_t *rqst_data_packet);
void update_last_read_position(uint32_t new_position);
void pack_module_data(uint16_t flags, uint16_t bit_flag, uint8_t *module_data, uint8_t module_size, uint8_t* to_send_packet, uint16_t *total_package_size);


#endif /* SRC_STORE_DATA_TASK_H_ */
