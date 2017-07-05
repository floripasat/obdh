/*
 * ttc_interface_task.c
 *
 *  Created on: 18 de jan de 2017
 *      Author: elder
 */

#include <ttc_interface_task.h>

#define MILISECONDS_IN_A_DAY    86400000      //1000ms * 60s * 60min * 24hr

void wait_before_transmit(void) {
    uint8_t counter = 0;

    //wait the tx_busy pin is on low state or until reach 2 seconds
    while(ttc_is_beacon_on() == TTC_TX_BUSY && counter++ < 20) {
        vTaskDelayMs(100); //sleeps for 100ms until test again
    }
}


void ttc_interface_task( void *pvParameters ) {
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    uint8_t to_send_data = 0;
    uint8_t tx_allow = 1;
    TaskHandle_t store_task_handle;

    ttc_setup();

    store_task_handle = xTaskGetHandle("StoreData");
    while(1) {

        xQueueReceive(ttc_queue, (void *) &to_send_data, portMAX_DELAY); //wait until receive data

        if(to_send_data & TTC_CMD_SHUTDOWN) {       //if is a shutdown cmd
            ttc_shutdown();
            //TODO: Implements the shutdown routine
            vTaskSuspend(store_task_handle);
            vTaskDelayMs(20000); //**this delay is a shutdown simulation **
//            vTaskDelayMs(MILISECONDS_IN_A_DAY); // wait for 24 hours
            vTaskResume(store_task_handle);
            //shutdown_routine(); wait 24 hours

            ttc_return_from_shutdown();
        }

        if(to_send_data & TTC_CMD_REQUEST_TX) {     //if is a data request cmd
            wait_before_transmit();
            ttc_prepare_to_tx();
            wait_before_transmit();

            xQueueOverwrite(tx_queue, &tx_allow); //request to beacon a tx permission
            //TODO: transmit_routine
        }

        if(to_send_data & TTC_CMD_FREE_TX) {
            ttc_free_to_send_beacon();
        }

        vTaskDelayUntil( (TickType_t *) &last_wake_time, TTC_INTERFACE_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
