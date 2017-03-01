#include <obdh.h>

void create_tasks( void ) {
    xTaskCreate( wdt_task, "WDT", configMINIMAL_STACK_SIZE, NULL, WDT_TASK_PRIORITY, &wdt_task_handle );
    xTaskCreate( store_data_task, "StoreData", 10 * configMINIMAL_STACK_SIZE, NULL, STORE_DATA_TASK_PRIORITY, &store_data_task_handle);
//    xTaskCreate( communications_task, "Communications", configMINIMAL_STACK_SIZE, NULL, COMMUNICATIONS_TASK_PRIORITY, &communications_task_handle );
    xTaskCreate( read_internal_sensors_task, "ReadInternal", configMINIMAL_STACK_SIZE, NULL, READ_INTERNAL_SENSORS_TASK_PRIORITY, &read_internal_sensors_task_handle);
    xTaskCreate( imu_interface_task, "IMU", configMINIMAL_STACK_SIZE, NULL, IMU_INTERFACE_TASK_PRIORITY, &imu_interface_task_handle);
//    xTaskCreate( solar_panels_interface_task, "SolarPanels", configMINIMAL_STACK_SIZE, NULL, SOLAR_PANELS_INTERFACE_TASK_PRIORITY, &solar_panels_interface_task_handle);
//    xTaskCreate( eps_interface_task, "EPS", configMINIMAL_STACK_SIZE, NULL, EPS_INTERFACE_TASK_PRIORITY, &eps_interface_task_handle );
//    xTaskCreate( ttc_interface_task, "TTC", configMINIMAL_STACK_SIZE, NULL, TTC_INTERFACE_TASK_PRIORITY, &ttc_interface_task_handle );

    xTaskCreate( debug_task, "DEBUG", 4 * configMINIMAL_STACK_SIZE, NULL, DEBUG_TASK_PRIORITY, &debug_task_handle);
}

void setup_hardware( void ) {
    taskDISABLE_INTERRUPTS();

    /*   External watchdog timer reset pin */
    wdti_setup(WATCHDOG, WD_16_SEC);
    wdte_setup();
    wdte_reset_counter();

    /*  SETUP CLOCKS */
    clocks_setup();

    /*  SETUP I2C */
    i2c_setup(0);
    i2c_setup(1);
    i2c_setup(2);

    /*  SETUP SPI */


    /*  SETUP UART */
    uart0_setup(9600);

    /*  SETUP ADC */
    adc_setup();

    /*  SETUP GPIO */
    //MAGNETORQUER
    //SD
    //
//    adc_setup_temp();
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

    if( ( ulCounter & 0xff ) == 0 )
    {
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


