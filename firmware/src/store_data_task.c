/*
 * store_data_task.c
 *
 *  Created on: 19 de out de 2016
 *      Author: elder
 */

#include <store_data_task.h>

volatile uint32_t last_read_pointer, last_write_pointer;
volatile uint32_t card_size;

void store_data_task( void *pvParameters ) {
    TickType_t last_wake_time;
    data_packet_t new_packet;
    last_wake_time = xTaskGetTickCount();

    card_size = mmc_setup();

    if(card_size < 128000000) { //test if memory card is working
        //TODO: use another memory

    }
    else {
        last_read_pointer = FIRST_DATA_SECTOR;
        last_write_pointer = FIRST_DATA_SECTOR;
    }

    while(1) {
        //save in the begining of the memory the log_status: (packages counter, resets counter, etc).
        new_packet = read_and_pack_data();
        store_data_on_flash(&new_packet);

        vTaskDelayUntil( (TickType_t *) &last_wake_time, STORE_DATA_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}

data_packet_t read_and_pack_data( void ) {
    data_packet_t packet = {0};

    packet = satellite_data;
    packet.package_flags = 0;

    if(xQueueReceive(imu_queue, (void *) packet.imu, IMU_QUEUE_WAIT_TIME) == pdPASS) {
        packet.package_flags |= IMU_FLAG;
    }


    if(xQueueReceive(internal_sensors_queue, (void *) packet.msp_sensors, INTERNAL_SENSORS_QUEUE_WAIT_TIME) == pdPASS) {
        packet.package_flags |= MSP_SENSORS_FLAG;
    }

    uint32_t systick = xTaskGetTickCount();
    packet.systick[0] = systick & 0xFF;
    packet.systick[1] = systick>>8 & 0xFF;
    packet.systick[2] = systick>>16 & 0xFF;
    packet.systick[3] = systick>>24 & 0xFF;
    packet.package_flags |= SYSTICK_FLAG;

    uint32_t reset = 0;
    packet.system_status[0] = reset & 0xFF;
    packet.system_status[1] = reset>>8 & 0xFF;
    packet.system_status[2] = reset>>16 & 0xFF;
    packet.system_status[3] = 0;
    packet.system_status[4] = 0;
    packet.package_flags |= SYSTEM_STATUS_FLAG;

    return packet;
}

void store_data_on_flash( data_packet_t *packet ) {
    //write new packet
    unsigned char data[512];
    int i;

    for(i = 0; i < sizeof(data_packet_t); i++) {
        data[i] = ((uint8_t *)packet)[i];
    }

    mmcWriteSector(++last_write_pointer, data);
    //write new status
//    mmcWriteSector(0, (unsigned char *) status_packet);
}


uint16_t get_packet(uint8_t* to_send_packet, request_packet_t *rqst_data_packet) {
    uint8_t flash_package[512];
    uint16_t package_size = 0;
    uint32_t read_sector;
    data_packet_t *p_data_packet;

    //mmcReadSector(older_package_sector, (unsigned char *) status_package);
    if(rqst_data_packet->packages_origin == OLDER_PACKAGES_ORIGIN) {
        read_sector =  last_read_pointer + rqst_data_packet->packages_offset;
    }
    else {
        read_sector = last_write_pointer + rqst_data_packet->packages_offset;;
    }

    if(read_sector < last_read_pointer) { //TODO: incluir o caso do setor de leitura chegar a posição maxima da memoria
        read_sector = last_read_pointer;
    }
    if(read_sector > last_write_pointer) {
        read_sector = last_write_pointer;
    }


    mmcReadSector(read_sector, (unsigned char *) flash_package);
    p_data_packet = (data_packet_t*)flash_package;

    uint16_t flags = 0x00;
    flags = rqst_data_packet->flags & p_data_packet->package_flags;


    pack_module_data( 1 , 1, (uint8_t*) &p_data_packet->package_flags, sizeof(p_data_packet->package_flags), to_send_packet, &package_size);
    pack_module_data(flags, SYSTEM_STATUS_FLAG,  p_data_packet->system_status, sizeof(p_data_packet->system_status), to_send_packet, &package_size);
    pack_module_data(flags, IMU_FLAG,  p_data_packet->imu, sizeof(p_data_packet->imu), to_send_packet, &package_size);
    pack_module_data(flags, MSP_SENSORS_FLAG,  p_data_packet->msp_sensors, sizeof(p_data_packet->msp_sensors), to_send_packet, &package_size);
    pack_module_data(flags, SYSTICK_FLAG,  p_data_packet->systick, sizeof(p_data_packet->systick), to_send_packet, &package_size);
    pack_module_data(flags, SOLAR_PANELS_FLAG,  p_data_packet->solar_panels, sizeof(p_data_packet->solar_panels), to_send_packet, &package_size);
    pack_module_data(flags, RTC_FLAG,  p_data_packet->rtc, sizeof(p_data_packet->rtc), to_send_packet, &package_size);
    pack_module_data(flags, RADIO_FLAG,  p_data_packet->radio, sizeof(p_data_packet->radio), to_send_packet, &package_size);
    pack_module_data(flags, ADC_SOLAR_PANELS_FLAG,  p_data_packet->adc_solar_panels, sizeof(p_data_packet->adc_solar_panels), to_send_packet, &package_size);
    pack_module_data(flags, MSP430_ADC_FLAG,  p_data_packet->msp430_adc, sizeof(p_data_packet->msp430_adc), to_send_packet, &package_size);
    pack_module_data(flags, BATTERY_MONITOR_FLAG,  p_data_packet->battery_monitor, sizeof(p_data_packet->battery_monitor), to_send_packet, &package_size);
    pack_module_data(flags, ADS1248_FLAG,  p_data_packet->ads1248, sizeof(p_data_packet->ads1248), to_send_packet, &package_size);
    pack_module_data(flags, TASK_SCHEDULER_FLAG,  p_data_packet->task_scheduler, sizeof(p_data_packet->task_scheduler), to_send_packet, &package_size);
    pack_module_data(flags, BEACON_FLAG,  p_data_packet->beacon, sizeof(p_data_packet->beacon), to_send_packet, &package_size);
    pack_module_data(flags, TRANSCEIVER_FLAG,  p_data_packet->transceiver, sizeof(p_data_packet->transceiver), to_send_packet, &package_size);
    pack_module_data(flags, PAYLOAD1_FLAG,  p_data_packet->payload1, sizeof(p_data_packet->payload1), to_send_packet, &package_size);
    pack_module_data(flags, PAYLOAD2_FLAG,  p_data_packet->payload2, sizeof(p_data_packet->payload2), to_send_packet, &package_size);

    rqst_data_packet->packages_offset++;
    rqst_data_packet->packages_count--;

    return package_size;
}

void update_last_read_position(uint32_t new_position) {
//    last_read_pointer = new_position;
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
