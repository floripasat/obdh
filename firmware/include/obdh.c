/*
 * obdh.c
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
 * \file obdh.c
 *
 * \brief Main functions of the OBDH module
 *
 * \author Elder Tramontin
 *
 */
#include "obdh.h"

void create_tasks( void ) {
    /**
     * Create queues to communicate between tasks
     */
    obdh_status_queue       = xQueueCreate( 5, sizeof( satellite_data.obdh_status ) );
    imu_queue               = xQueueCreate( 5, sizeof( satellite_data.imu ) );
    obdh_misc_queue         = xQueueCreate( 5, sizeof( satellite_data.obdh_misc ) );
    obdh_uptime_queue       = xQueueCreate( 5, sizeof( satellite_data.obdh_uptime ) );
    solar_panels_queue      = xQueueCreate( 5, sizeof( satellite_data.solar_panels_sensors ) );
    main_radio_queue        = xQueueCreate( 5, sizeof( satellite_data.main_radio ) );
    eps_queue               = xQueueCreate( 5, sizeof( eps_package_t ) );
    ttc_queue               = xQueueCreate( 1, sizeof( uint8_t ) );
    tx_queue                = xQueueCreate( 1, sizeof( uint8_t ) );
    payload1_queue          = xQueueCreate( 5, sizeof( satellite_data.payload1) );
    payload2_queue          = xQueueCreate( 5, sizeof( satellite_data.payload2) );
    status_eps_queue        = xQueueCreate( 1, sizeof(uint8_t) );
    status_payload1_queue   = xQueueCreate( 1, sizeof(uint8_t) );
    status_payload2_queue   = xQueueCreate( 1, sizeof(uint8_t) );
    status_mem1_queue       = xQueueCreate( 1, sizeof(uint8_t) );
    status_imu_queue        = xQueueCreate( 1, sizeof(uint8_t) );

    /**
     * Create the semaphores to synchronize the use of shared resources (mutual exclusion)
     */
    spi1_semaphore   = xSemaphoreCreateMutex();
    i2c0_semaphore   = xSemaphoreCreateMutex();
    fsp_semaphore    = xSemaphoreCreateMutex();
    flash_semaphore  = xSemaphoreCreateMutex();

    /**
     * Create each task: links with a routine, allocates the requested task
     * stack size, sets the priority, passes parameters and get a handler
     */
    xTaskCreate( wdt_task, "WDT", configMINIMAL_STACK_SIZE, NULL, WDT_TASK_PRIORITY, &wdt_task_handle );
    xTaskCreate( communications_task, "Communications", 6 * configMINIMAL_STACK_SIZE, NULL, COMMUNICATIONS_TASK_PRIORITY, &communications_task_handle );
    xTaskCreate( store_data_task, "StoreData", 8 * configMINIMAL_STACK_SIZE, NULL , STORE_DATA_TASK_PRIORITY, &store_data_task_handle);
    xTaskCreate( housekeeping_task, "Housekeeping", configMINIMAL_STACK_SIZE, NULL, HOUSEKEEPING_TASK_PRIORITY, &housekeeping_task_handle);
    xTaskCreate( ttc_interface_task, "TT&C", 4 * configMINIMAL_STACK_SIZE, NULL, TTC_INTERFACE_TASK_PRIORITY, &ttc_interface_task_handle );
    xTaskCreate( eps_interface_task, "EPS", configMINIMAL_STACK_SIZE, NULL, EPS_INTERFACE_TASK_PRIORITY, &eps_interface_task_handle );
    xTaskCreate( imu_interface_task, "IMU", configMINIMAL_STACK_SIZE, NULL, IMU_INTERFACE_TASK_PRIORITY, &imu_interface_task_handle);
//    xTaskCreate( solar_panels_interface_task, "SolarPanels", configMINIMAL_STACK_SIZE, NULL, SOLAR_PANELS_INTERFACE_TASK_PRIORITY, &solar_panels_interface_task_handle);
//    xTaskCreate( payload1_interface_task, "Payload1", configMINIMAL_STACK_SIZE, NULL, PAYLOAD1_INTERFACE_TASK_PRIORITY, &payload1_interface_task_handle );
#ifdef _DEBUG
    //xTaskCreate( debug_task, "DEBUG", 4 * configMINIMAL_STACK_SIZE, NULL, DEBUG_TASK_PRIORITY, &debug_task_handle);
#endif
}

void gpio_setup() {
    //TODO: set the configuration of every pins. //MAGNETORQUER   //SD
    BIT_SET(LED_SYSTEM_DIR, LED_SYSTEM_PIN);            /**< Led pin setup */


    BIT_SET(uSDCard_CE_OUT, uSDCard_CE_PIN);            /**< disable memory */
    BIT_SET(uSDCard_CE_DIR, uSDCard_CE_PIN);

    rf4463_gpio_init();
}

void setup_hardware( void ) {
    uint8_t test_result;

    taskDISABLE_INTERRUPTS();

    gpio_reset();

    /**
     *  Configure and reset the watchdog timers
     */
    wdti_setup(WATCHDOG, WD_16_SEC);
    wdte_setup();
    wdte_reset_counter();

    test_result = clocks_setup();   /**< Setup clocks                                                       */

    uart0_setup(9600);              /**< Setup UART                                                         */

    /*
     * Print some booting messages
     */
    debug(BOOTING_MSG);
    debug(UART_INFO_MSG);
    if(test_result == TEST_SUCESS) {
        debug(CLOCK_INFO_MSG);
    }
    else {
        debug(CLOCK_FAIL_MSG);
    }

    /*
     * Setup I2C interfaces 0, 1 and 2
     */
    i2c_setup(0);
    i2c_setup(1);
    i2c_setup(2);

    debug(I2C_INFO_MSG);            /**< Setup I2C                                                          */

    /*
     * Setup SPI interfaces 0 and 1
     */
    spi_setup(0);
    spi_setup(1);

    debug(SPI_INF_MSG);             /**< Setup SPI                                                          */

    adc_setup();                    /**< Setup ADC                                                          */

    debug(ADC_INFO_MSG);

    sspi_setup();                   /**< Setup software SPI                                                 */

    gpio_setup();                   /**< Setup all GPIO pins according each function                        */

    update_reset_value();           /**< Read the previous value, increment it and store again              */
    restore_time_counter();         /**< Read the time counter after a reset and restore it value to RAM    */

    debug("\n --- Boot completed ---\n");
}

void hibernate(void) {
    uint8_t seconds_counter = 0;

    start_timer_b();                    /**< configure and start counting time over the timer B */

    do {
        /*
         *  Reset Watchdog timers
         */
        wdte_reset_counter();
        wdti_reset_counter();

        low_power_mode_sleep();         /**< Enter in Low-power mode */
        /**< Wake-up after a interrupt event */

        /*
         * Count 1 minute and store the value in a flash memory
         */
        if(seconds_counter++ == 60) {
            seconds_counter = 0;
            update_time_counter();
        }

    } while (read_time_counter() < 45); /**< loop until reach 45 minutes */

    stop_timer_b();                     /**< stop counting time over the timer B */
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


