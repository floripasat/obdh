/*
 * store_data_task.h
 *
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
 *
 * This file is part of FloripaSat-OBDH.
 *
 * FloripaSat-OBDH is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FloripaSat-OBDH is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FloripaSat-OBDH.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 /**
 * \file store_data_task.h
 *
 * \brief Task that deals with the non-volatile memory module
 *
 * \author Elder Tramontin
 *
 */

#ifndef SRC_STORE_DATA_TASK_H_
#define SRC_STORE_DATA_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "task_queues.h"
#include "../util/MMC_lib/mmc.h"
#include "../util/MMC_lib/hal_MMC_hardware_board.h"

/* TODO: remove the global variables in obdh_def*/
#include "../include/floripasat_def.h"


#define STORE_DATA_TASK_PRIORITY          5             /**< Store data task priority */
#define STORE_DATA_TASK_PERIOD_MS         1000          /**< Store data task period in miliseconds */
#define STORE_DATA_TASK_PERIOD_TICKS      ( STORE_DATA_TASK_PERIOD_MS / portTICK_PERIOD_MS )    /**< Store data task period in ticks */

#define SECTOR_SIZE                       512
#define FIRST_DATA_SECTOR                 4096          /**< First sector where data can be writen. The previous sectors will be reserved to write some status values*/
#define MEMORY_CHECK_OPERATION_SIZE       128000000

#define TELECOMMAND_COUNTER_SECTOR        1
#define STORE_LAST_READ_SECTOR            2
#define STORE_LAST_WRITE_SECTOR           3
#define STORE_AUXILIAR_LAST_WRITE_SECTOR  4

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
uint16_t get_packet(uint8_t* to_send_packet,  uint16_t rqst_flags, uint32_t read_sector);

uint32_t get_last_read_pointer(void);

uint32_t get_last_write_pointer(void);

void pack_module_data(uint16_t flags, uint16_t bit_flag, uint8_t *module_data, uint8_t module_size, uint8_t* to_send_packet, uint16_t *total_package_size);


#endif /* SRC_STORE_DATA_TASK_H_ */
