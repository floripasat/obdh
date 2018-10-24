/*
 * store_data_task.c
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
 * \file store_data_task.c
 *
 * \brief Task that deals with the non-volatile memory module
 *
 * \author Elder Tramontin
 *
 */

#include "store_data_task.h"

#define     DATA_QUEUE_WAIT_TIME    ( 100 / portTICK_PERIOD_MS ) //100ms of wait time

uint32_t get_last_read_pointer(void);
uint32_t get_last_write_pointer(void);
void update_last_write_position(void);
void update_last_read_position(uint32_t new_position);

volatile uint32_t last_read_position, last_write_position;
volatile uint32_t card_size;

void store_data_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();

    data_packet_t new_packet;
    uint8_t mem1_status;

    uint8_t memflag;
    uint8_t datatest_write[4]={0xA1, 0xB2, 0xC3, 0xD4};
    uint8_t datatest_read[4]={0};
    uint32_t address;
    uint8_t byte_length;

    if (xSemaphoreTake(spi1_semaphore, SPI_SEMAPHORE_WAIT_TIME) == pdPASS) {
        card_size = mmc_setup();
        pin_setup();
        xSemaphoreGive(spi1_semaphore);
    }

    /**< Test if memory card is working */
    if(card_size > MEMORY_CHECK_OPERATION_SIZE) {
        last_read_position = get_last_read_pointer();
        last_write_position = get_last_write_pointer();
        memflag = 0;
    }
    else {
        memflag =1;
    }

    while(1) {
        /**< Test if memory size is working to report status */
        mem1_status = 0;
        if (xSemaphoreTake(spi1_semaphore, SPI_SEMAPHORE_WAIT_TIME) == pdPASS) {
            card_size = mmcReadCardSize();
            if(card_size >= MEMORY_CHECK_OPERATION_SIZE) {
                mem1_status = 1;
            }
            xSemaphoreGive(spi1_semaphore);
        }
        xQueueOverwrite(status_mem1_queue, &mem1_status);

        if (memflag == 0) {
        /**< Store packet routine */
        new_packet = read_and_pack_data();
        if (xSemaphoreTake(spi1_semaphore, SPI_SEMAPHORE_WAIT_TIME) == pdPASS) {
            store_data_on_flash(&new_packet);
            xSemaphoreGive(spi1_semaphore);
            update_last_write_position();
        }
        }
        else {
            address = 0x00000000;
            byte_length = 4;

            if (xSemaphoreTake(spi1_semaphore, SPI_SEMAPHORE_WAIT_TIME) == pdPASS) {

            mem_SER(address, Mem0);
            mem_SER(address, Mem1);
            mem_SER(address, Mem2);

            mem_read_multiple(datatest_read, address, byte_length, Mem0);
            mem_read_multiple(datatest_read, address, byte_length, Mem1);
            mem_read_multiple(datatest_read, address, byte_length, Mem2);


            mem_pp_multiple(datatest_write, address, byte_length, Mem0);
            mem_read_multiple(datatest_read, address, byte_length, Mem0);

            mem_pp_multiple(datatest_write, address, byte_length, Mem1);
            mem_read_multiple(datatest_read, address, byte_length, Mem1);

            mem_pp_multiple(datatest_write, address, byte_length, Mem2);
            mem_read_multiple(datatest_read, address, byte_length, Mem2);

            mem_BER64K(address, Mem0);
            mem_read_multiple(datatest_read, address, byte_length, Mem0);

            mem_BER64K(address, Mem1);
            mem_read_multiple(datatest_read, address, byte_length, Mem1);

            mem_BER64K(address, Mem2);
            mem_read_multiple(datatest_read, address, byte_length, Mem2);

            mem_id(datatest_read, Mem0);
            mem_id(datatest_read, Mem1);
            mem_id(datatest_read, Mem2);

            xSemaphoreGive(spi1_semaphore);
            }

        }

        /**< Operating system delay for periodicity between task calls */
        if ( (last_wake_time + STORE_DATA_TASK_PERIOD_TICKS) < xTaskGetTickCount() ) {
            last_wake_time = xTaskGetTickCount();
        }
        else {
            vTaskDelayUntil( (TickType_t *) &last_wake_time, STORE_DATA_TASK_PERIOD_TICKS );
        }
    }

    vTaskDelete( NULL );
}

data_packet_t read_and_pack_data( void ) {
    data_packet_t packet = {0};
    uint16_t telecommand_counter = 0;

    packet = satellite_data;
    packet.package_flags = 0;

    xQueueReceive(obdh_uptime_queue, (void *) packet.obdh_uptime, DATA_QUEUE_WAIT_TIME);

    if(xQueueReceive(obdh_status_queue, (void *) packet.obdh_status, DATA_QUEUE_WAIT_TIME) == pdPASS) {
        packet.package_flags |= SYSTEM_STATUS_FLAG;
    }

    if(xQueueReceive(imu_queue, (void *) packet.imu, DATA_QUEUE_WAIT_TIME) == pdPASS) {
        packet.package_flags |= IMU_FLAG;
    }

    if(xQueueReceive(obdh_misc_queue, (void *) packet.obdh_misc, DATA_QUEUE_WAIT_TIME) == pdPASS) {
        packet.package_flags |= OBDH_MISC_FLAG;
    }

    if(xQueueReceive(solar_panels_queue, (void *) packet.solar_panels_sensors, DATA_QUEUE_WAIT_TIME) == pdPASS) {
        packet.package_flags |= SOLAR_PANELS_SENSORS_FLAG;
    }

    if(xQueueReceive(main_radio_queue, (void *) packet.main_radio, DATA_QUEUE_WAIT_TIME) == pdPASS) {
        /**< update the last telecommand counter value */
        if (xSemaphoreTake(spi1_semaphore, SPI_SEMAPHORE_WAIT_TIME) == pdPASS) {
            mmcReadBlock((TELECOMMAND_COUNTER_SECTOR * SECTOR_SIZE), 2, (unsigned char *)&telecommand_counter);
            telecommand_counter++;
            packet.main_radio[17] = (uint8_t)(telecommand_counter >> 8);
            packet.main_radio[18] = (uint8_t)telecommand_counter;
            mmcWriteSector(TELECOMMAND_COUNTER_SECTOR, (unsigned char *)&telecommand_counter);
            xSemaphoreGive(spi1_semaphore);
        }

        packet.package_flags |= MAIN_RADIO_FLAG;
    }

    if(xQueueReceive(eps_queue, (void *) packet.solar_panels, DATA_QUEUE_WAIT_TIME) == pdPASS) {
        packet.package_flags |= SOLAR_PANELS_FLAG;
        packet.package_flags |= EPS_MISC_FLAG;
        packet.package_flags |= BATTERY_MONITOR_FLAG;
        packet.package_flags |= TEMPERATURES_FLAG;
        packet.package_flags |= TASK_SCHEDULER_FLAG;
    }

    if(xQueueReceive(payload1_queue, (void *) packet.payload1, DATA_QUEUE_WAIT_TIME) == pdPASS) {
        packet.package_flags |= PAYLOAD1_FLAG;
    }

    if(xQueueReceive(payload2_queue, (void *) packet.payload2, DATA_QUEUE_WAIT_TIME) == pdPASS) {
        packet.package_flags |= PAYLOAD2_FLAG;
    }

    satellite_data = packet;

    return packet;
}

void pack_module_data(uint16_t flags, uint16_t bit_flag, uint8_t *module_data, uint8_t module_size, uint8_t* to_send_packet, uint16_t *total_package_size) {
    uint16_t i;

    if(has_flag(flags,bit_flag)) {
        for(i = 0; i < module_size; i++) {
            to_send_packet[*total_package_size + i] = module_data[i];
        }
        *total_package_size += module_size;
    }
}

uint16_t get_packet(uint8_t* to_send_packet,  uint16_t rqst_flags, uint32_t read_sector) {
    uint8_t flash_package[512];
    uint16_t package_size = 0;
    data_packet_t *p_data_packet;
    uint16_t flags = 0x00;


    if (xSemaphoreTake(spi1_semaphore, SPI_SEMAPHORE_WAIT_TIME) == pdPASS) {
        mmcReadSector(read_sector, (unsigned char *) flash_package);
        xSemaphoreGive(spi1_semaphore);
    }

    p_data_packet = (data_packet_t*)flash_package;

    if(has_flag(WHOLE_ORBIT_DATA_FLAG, rqst_flags)) {
        flags = ALL_FLAGS;
    }
    else {
        flags = rqst_flags & p_data_packet->package_flags;
    }

    pack_module_data( 1 , 1, (uint8_t*) &p_data_packet->package_flags, sizeof(p_data_packet->package_flags), to_send_packet, &package_size);
    pack_module_data( 1 , 1,  p_data_packet->obdh_uptime, sizeof(p_data_packet->obdh_uptime), to_send_packet, &package_size);
    pack_module_data(flags, SYSTEM_STATUS_FLAG,  p_data_packet->obdh_status, sizeof(p_data_packet->obdh_status), to_send_packet, &package_size);
    pack_module_data(flags, IMU_FLAG,  p_data_packet->imu, sizeof(p_data_packet->imu), to_send_packet, &package_size);
    pack_module_data(flags, OBDH_MISC_FLAG,  p_data_packet->obdh_misc, sizeof(p_data_packet->obdh_misc), to_send_packet, &package_size);
    pack_module_data(flags, SOLAR_PANELS_SENSORS_FLAG,  p_data_packet->solar_panels_sensors, sizeof(p_data_packet->solar_panels_sensors), to_send_packet, &package_size);
    pack_module_data(flags, MAIN_RADIO_FLAG,  p_data_packet->main_radio, sizeof(p_data_packet->main_radio), to_send_packet, &package_size);
    pack_module_data(flags, SOLAR_PANELS_FLAG,  p_data_packet->solar_panels, sizeof(p_data_packet->solar_panels), to_send_packet, &package_size);
    pack_module_data(flags, EPS_MISC_FLAG,  p_data_packet->eps_misc, sizeof(p_data_packet->eps_misc), to_send_packet, &package_size);
    pack_module_data(flags, BATTERY_MONITOR_FLAG,  p_data_packet->battery_monitor, sizeof(p_data_packet->battery_monitor), to_send_packet, &package_size);
    pack_module_data(flags, TEMPERATURES_FLAG,  p_data_packet->temperatures, sizeof(p_data_packet->temperatures), to_send_packet, &package_size);
    pack_module_data(flags, TASK_SCHEDULER_FLAG,  p_data_packet->energy_level, sizeof(p_data_packet->energy_level), to_send_packet, &package_size);
    pack_module_data(flags, PAYLOAD1_FLAG,  p_data_packet->payload1, sizeof(p_data_packet->payload1), to_send_packet, &package_size);
    pack_module_data(flags, PAYLOAD2_FLAG,  p_data_packet->payload2, sizeof(p_data_packet->payload2), to_send_packet, &package_size);

    return package_size;
}

void store_data_on_flash( data_packet_t *packet ) {
    //write new packet
    unsigned char data[512];
    unsigned int i;

    for(i = 0; i < sizeof(data_packet_t); i++) {
        data[i] = ((uint8_t *)packet)[i];
    }

    mmcWriteSector(last_write_position, data);
    //write new status
//    mmcWriteSector(0, (unsigned char *) status_packet);
}

/**
 * \fn get_last_read_pointer
 * Get the last read pointer after a MCU reset.
 * \param none
 * \return last_read_pointer is the last sector of successfully data read
 */
uint32_t get_last_read_pointer(void) {

    uint32_t last_read_pointer[128];

    if (xSemaphoreTake(spi1_semaphore, SPI_SEMAPHORE_WAIT_TIME) == pdPASS) {
        mmcReadSector(STORE_LAST_READ_SECTOR, (unsigned char *)last_read_pointer);
        xSemaphoreGive(spi1_semaphore);
    }

    return last_read_pointer[0];
}

/**
 * \fn get_last_write_pointer
 * Get the last write pointer after a MCU reset.
 * \param none
 * \return last_write_pointer is the last sector of write data
 */
uint32_t get_last_write_pointer(void) {
    uint32_t last_write_pointer[128];
    volatile uint32_t store_write_pointer_sector[128];

    if (xSemaphoreTake(spi1_semaphore, SPI_SEMAPHORE_WAIT_TIME) == pdPASS) {
        /**< Get the current position of the storage sector to the last write pointer */
        mmcReadSector(STORE_LAST_WRITE_SECTOR, (unsigned char *)store_write_pointer_sector);

        /**< Prevent overwrite of the status sectors */
        if (store_write_pointer_sector[0] < STORE_AUXILIAR_LAST_WRITE_SECTOR) {
            store_write_pointer_sector[0] = STORE_AUXILIAR_LAST_WRITE_SECTOR;
            mmcWriteSector(STORE_LAST_WRITE_SECTOR, (unsigned char *)store_write_pointer_sector);

            /**< Get the value back after the write sector function change it */
            store_write_pointer_sector[0] = STORE_AUXILIAR_LAST_WRITE_SECTOR;
        }

        /**< Read this sector to get the last write pointer value */
        mmcReadSector(store_write_pointer_sector[0], (unsigned char *)last_write_pointer);

        xSemaphoreGive(spi1_semaphore);
    }

    return last_write_pointer[0];
}

/**
 * \fn update_last_read_position
 * Update the last_read_pointer to a new value. Used after a successfully read operation.
 * \param new_position is the next position for read
 * \return none
 */
void update_last_read_position(uint32_t new_position) {

    uint32_t last_read_pointer[128];

    if (xSemaphoreTake(spi1_semaphore, SPI_SEMAPHORE_WAIT_TIME) == pdPASS) {
        if(new_position > last_read_position) {
            last_read_pointer[0] = new_position;
            mmcWriteSector(STORE_LAST_READ_SECTOR, (unsigned char *)last_read_pointer);
        }
        xSemaphoreGive(spi1_semaphore);
    }
}

/**
 * \fn update_last_write_position
 * Update the last_write_pointer to the next position.
 * \param none
 * \return none
 */
void update_last_write_position(void) {

    uint32_t last_write_pointer[128];
    uint32_t store_write_pointer_sector[128];
    uint32_t store_write_pointer_sector_backup;

    /**< Turn the memory positions cyclic */
    if(card_size >= MEMORY_CHECK_OPERATION_SIZE) {
        last_write_position = ++last_write_position % card_size;
        if (last_write_position == 0){
            last_write_position = FIRST_DATA_SECTOR;
        }
    }

    if (xSemaphoreTake(spi1_semaphore, SPI_SEMAPHORE_WAIT_TIME) == pdPASS) {
        /**< Get the current position of the storage sector to the last write pointer */
        mmcReadSector(STORE_LAST_WRITE_SECTOR, (unsigned char *)store_write_pointer_sector);

        /**< Backup value for the last write position and its store sector*/
        store_write_pointer_sector_backup = store_write_pointer_sector[0];

        /**< Check for corrupted sectors */
        do {
            /**< Update the last write position with the new one and get it back */
            last_write_pointer[0] = last_write_position;
            mmcWriteSector(store_write_pointer_sector[0], (unsigned char *)last_write_pointer);
            mmcReadSector(store_write_pointer_sector[0], (unsigned char *)last_write_pointer);

            if (last_write_position != last_write_pointer[0]) {
                store_write_pointer_sector[0]++;
            }
        } while ( ( last_write_position != last_write_pointer[0] ) && ( store_write_pointer_sector[0] < FIRST_DATA_SECTOR ) );

        /**< Update the current sector to store the last write sector if necessary */
        if (store_write_pointer_sector_backup != store_write_pointer_sector[0]) {
            mmcWriteSector(STORE_LAST_WRITE_SECTOR, (unsigned char *)store_write_pointer_sector);
        }
        xSemaphoreGive(spi1_semaphore);
    }
}
