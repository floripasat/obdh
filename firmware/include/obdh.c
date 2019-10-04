/*
 * obdh.c
 *
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina.
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
 * \brief Main functions of the OBDH module
 *
 * \author Elder Tramontin
 *
 * \version 1.0.7
 *
 * \addtogroup obdh
 */

#include "../interface/debug/debug.h"
#include "../interface/isis_antenna.h"

#include "obdh.h"

void create_tasks(void) {
    /*
     * Create queues to communicate between tasks
     */
    obdh_status_queue               = xQueueCreate( 5, sizeof( satellite_data.obdh_status ) );
    imu_queue                       = xQueueCreate( 5, sizeof( satellite_data.imu ) );
    obdh_misc_queue                 = xQueueCreate( 5, sizeof( satellite_data.obdh_misc ) );
    obdh_uptime_queue               = xQueueCreate( 5, sizeof( satellite_data.obdh_uptime ) );
    solar_panels_queue              = xQueueCreate( 5, sizeof( satellite_data.solar_panels_sensors ) );
    main_radio_queue                = xQueueCreate( 1, sizeof( satellite_data.main_radio ) );
    eps_queue                       = xQueueCreate( 5, sizeof( eps_package_t ) );
    ttc_queue                       = xQueueCreate( 1, sizeof( uint8_t ) );
    tx_queue                        = xQueueCreate( 1, sizeof( uint8_t ) );
    payload_rush_queue              = xQueueCreate( 5, sizeof( satellite_data.payload_rush) );
    status_eps_queue                = xQueueCreate( 1, sizeof(uint8_t) );
    status_payload_rush_queue       = xQueueCreate( 1, sizeof(uint8_t) );
    command_to_payload_rush_queue   = xQueueCreate( 5, sizeof(uint8_t) ); // definir tamanho dessa queue
    status_mem1_queue               = xQueueCreate( 1, sizeof(uint8_t) );
    status_imu_queue                = xQueueCreate( 1, sizeof(uint8_t) );
    eps_charge_queue                = xQueueCreate( 1, sizeof(uint8_t) );
#if OBDH_PAYLOAD_X_ENABLED == 1
    status_payload_brave_queue      = xQueueCreate( 1, sizeof(uint8_t) );
    payload_brave_uplink_queue      = xQueueCreate( 1, sizeof(payload_brave_uplink_t));
    payload_brave_downlink_queue    = xQueueCreate( 1, sizeof(payload_brave_downlink_t));
    payload_brave_queue             = xQueueCreate( 1, sizeof(payload_brave_downlink_t));
#endif // OBDH_PAYLOAD_X_ENABLED

    /*
     * Create the semaphores to synchronize the use of shared resources (mutual exclusion)
     */
    spi1_semaphore   = xSemaphoreCreateMutex();
    i2c0_semaphore   = xSemaphoreCreateMutex();
    fsp_semaphore    = xSemaphoreCreateMutex();
    flash_semaphore  = xSemaphoreCreateMutex();

    /*
     * Create each task: links with a routine, allocates the requested task
     * stack size, sets the priority, passes parameters and get a handler
     */
    xTaskCreate( wdt_task, "WDT", configMINIMAL_STACK_SIZE, NULL, WDT_TASK_PRIORITY, &wdt_task_handle );
    xTaskCreate( communications_task, "Communications", 1800, NULL, COMMUNICATIONS_TASK_PRIORITY, &communications_task_handle );
    xTaskCreate( store_data_task, "StoreData", 11 * configMINIMAL_STACK_SIZE, NULL , STORE_DATA_TASK_PRIORITY, &store_data_task_handle);
    xTaskCreate( housekeeping_task, "Housekeeping", configMINIMAL_STACK_SIZE, NULL, HOUSEKEEPING_TASK_PRIORITY, &housekeeping_task_handle);
    xTaskCreate( ttc_interface_task, "TT&C", 700, NULL, TTC_INTERFACE_TASK_PRIORITY, &ttc_interface_task_handle );
    xTaskCreate( eps_interface_task, "EPS", 512, NULL, EPS_INTERFACE_TASK_PRIORITY, &eps_interface_task_handle );
//    xTaskCreate( imu_interface_task, "IMU", configMINIMAL_STACK_SIZE, NULL, IMU_INTERFACE_TASK_PRIORITY, &imu_interface_task_handle);

#ifdef OBDH_PAYLOAD_X_ENABLED == 1
    xTaskCreate( payload_brave_interface_task, "Payload2", 3 * configMINIMAL_STACK_SIZE, NULL, PAYLOAD_BRAVE_INTERFACE_TASK_PRIORITY, &payload_brave_interface_task_handle);
#endif // OBDH_PAYLOAD_X_ENABLED
//    xTaskCreate( solar_panels_interface_task, "SolarPanels", configMINIMAL_STACK_SIZE, NULL, SOLAR_PANELS_INTERFACE_TASK_PRIORITY, &solar_panels_interface_task_handle);
//    xTaskCreate( payload_rush_interface_task, "PayloadRush", configMINIMAL_STACK_SIZE, NULL, PAYLOAD_RUSH_INTERFACE_TASK_PRIORITY, &payload_rush_interface_task_handle );
}

void gpio_setup() {
    //TODO: set the configuration of every pins. //MAGNETORQUER   //SD
    debug_print_event_from_module(DEBUG_INFO, "GPIO", "Initializing status LED...");
    debug_new_line();
    BIT_SET(LED_SYSTEM_DIR, LED_SYSTEM_PIN);            // Led pin setup


    BIT_SET(uSDCard_CE_OUT, uSDCard_CE_PIN);            // disable memory
    BIT_SET(uSDCard_CE_DIR, uSDCard_CE_PIN);

    rf4463_gpio_init();
}

void setup_hardware(void) {
    taskDISABLE_INTERRUPTS();

    gpio_reset();

    // Configure and reset the watchdog timers
    wdti_setup(WATCHDOG, WD_16_SEC);
    wdte_setup();
    wdte_reset_counter();

    // Setup clocks
    uint8_t test_result = clocks_setup();

    // Debug interface and boot messages
    debug_init();

    if (test_result == TEST_SUCESS) {
        debug_print_event_from_module(DEBUG_INFO, "System", "Clock configuration: Master = 16 MHz, Subsystem Master = 16 MHz, Auxiliary = 32768 kHz");
        debug_new_line();
    }
    else {
        debug_print_event_from_module(DEBUG_ERROR, "System", "Error during clock configuration!");
        debug_new_line();
    }

    // Setup I2C interfaces 0, 1 and 2
    i2c_setup(0);
    i2c_setup(1);
    i2c_setup(2);

    // Setup SPI interfaces 0 and 1
    spi_setup(0);
    spi_setup(1);

    // Setup ADC
    adc_setup();

    // Setup software SPI
    sspi_setup();

    // Setup all GPIO pins according each function
    gpio_setup();

    update_reset_value();           // Read the previous value, increment it and store again
    restore_time_counter();         // Read the time counter after a reset and restore it value to RAM

/*
    uint16_t temp_val = read_antenna_temperature();
    debug_print_event_from_module(DEBUG_INFO, "System", "Antenna module temperature = ");
    debug_print_dec(temp_val);
    debug_new_line();
*/
    debug_print_event_from_module(DEBUG_INFO, "System", "Boot completed!");
    debug_new_line();
}

void hibernate(void) {
    if (read_time_counter() > MINUTES_BEFORE_DEPLOY_ANTENNAS) {
        debug_print_event_from_module(DEBUG_INFO, "System", "Deployment hibernation already executed! Skipping...");
        debug_new_line();

        return;
    }

    debug_print_event_from_module(DEBUG_WARNING, "System", "Deployment hibernation never executed!");
    debug_new_line();

    debug_print_event_from_module(DEBUG_INFO, "System", "First deployment attempt in ");
    debug_print_dec(MINUTES_BEFORE_DEPLOY_ANTENNAS);
    debug_print_msg(" minute(s)...");
    debug_new_line();

    uint8_t seconds_counter = 0;

    do {
        // Reset Watchdog timers
        wdte_reset_counter();
        wdti_reset_counter();

        __delay_cycles(16000000);       // 1000 ms

        // Count 1 minute and store the value in a flash memory
        if (seconds_counter++ >= 60) {
            seconds_counter = 0;
            update_time_counter();

            debug_print_event_from_module(DEBUG_INFO, "System", "First deployment attempt in ");
            debug_print_dec(MINUTES_BEFORE_DEPLOY_ANTENNAS - read_time_counter());
            debug_print_msg(" minute(s)...");
            debug_new_line();
        }

    } while (read_time_counter() < MINUTES_BEFORE_DEPLOY_ANTENNAS); // loop until reach 45 minutes

    debug_print_event_from_module(DEBUG_INFO, "System", "Deployment hibernation executed!");
    debug_new_line();
}

void reset_memory(void) {
    debug_print_event_from_module(DEBUG_INFO, "System", "Erasing flash memories...");
    debug_new_line();

    flash_erase(SEGD_ADDR);
    flash_erase(SEGC_ADDR);
    flash_erase(SEGB_ADDR);
    flash_erase(SEGA_ADDR);
}

void vApplicationTickHook( void ) {
    /*
     * Tick hook functions execute within the context of the tick interrupt so must be kept very short,
     *  use only a moderate amount of stack space,
     *  and not call any FreeRTOS API functions whose name does not end with ‘FromISR()’.
     */
    static unsigned long ulCounter = 0;

    /* Is it time to toggle the LED again? */
    ulCounter++;

    if( ( ulCounter & 0xff ) == 0 ) {
        /* Once in 256 tick counts, do that */
    }
}
/*-----------------------------------------------------------*/

/* The MSP430X port uses this callback function to configure its tick interrupt.
This allows the application to choose the tick interrupt source.
configTICK_VECTOR must also be set in FreeRTOSConfig.h to the correct
interrupt vector for the chosen tick interrupt source.  This implementation of
vApplicationSetupTimerInterrupt() generates the tick from timer A0, so in this
case configTICK_VECTOR is set to TIMER0_A0_VECTOR. */
void vApplicationSetupTimerInterrupt( void ) {
    const unsigned short usACLK_Frequency_Hz = 32768;

    /* Ensure the timer is stopped. */
    TA0CTL = 0;

    /* Run the timer from the ACLK. */
    TA0CTL = TASSEL_1;

    /* Clear everything to start with. */
    TA0CTL |= TACLR;

    /* Set the compare match value according to the tick rate we want. */
    TA0CCR0 = usACLK_Frequency_Hz / configTICK_RATE_HZ;

    /* Enable the interrupts. */
    TA0CCTL0 = CCIE;

    /* Start up clean. */
    TA0CTL |= TACLR;

    /* Up mode. */
    TA0CTL |= MC_1;
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void ) {
    /* Called on each iteration of the idle task.  In this case the idle task
    just enters a low(ish) power mode. */
    __bis_SR_register( LPM1_bits + GIE );
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void ) {
    /* Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues or
    semaphores. */
    taskDISABLE_INTERRUPTS();
    for( ;; );

}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName ) {
    ( void ) pxTask;
    ( void ) pcTaskName;

    /* Run time stack overflow checking is performed if
    configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}
/*-----------------------------------------------------------*/

//! \} End of obdh group
