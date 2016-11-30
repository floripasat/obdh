#include "../include/obdh.h"

void vWDTsConfiguration()
{
    watchdog_setup(WATCHDOG, WD_250_mSEC);
}

void vCreateTasks()
{
    xTaskCreate( wdtTask, "WDT", configMINIMAL_STACK_SIZE, NULL, WDT_TASK_PRIORITY, &xWdtTask );
    xTaskCreate( antennaTask, "AntennaDeploy", configMINIMAL_STACK_SIZE, NULL, ANTENNA_TASK_PRIORITY, &xAntennaTask);
    xTaskCreate( readInternalSensorsTask, "ReadInternal", configMINIMAL_STACK_SIZE, NULL, READ_INTERNAL_SENSORS_TASK_PRIORITY, &xReadInternalSensorsTask);
    xTaskCreate( epsInterfaceTask, "EPS", configMINIMAL_STACK_SIZE, NULL, EPS_INTERFACE_TASK_PRIORITY, &xEpsInterfaceTask );
//    xTaskCreate( ttcInterfaceTask, "TTC", configMINIMAL_STACK_SIZE, NULL, TTC_TASK_PRIORITY, &xTtcInterfaceTask );
    xTaskCreate( communicationsTask, "Communications", configMINIMAL_STACK_SIZE, NULL, COMMUNICATIONS_TASK_PRIORITY, &xCommunicationsTask );
    xTaskCreate( imuInterfaceTask, "IMU", configMINIMAL_STACK_SIZE, NULL, IMU_INTERFACE_TASK_PRIORITY, &xImuInterfaceTask);
    xTaskCreate( solarPanelsInterfaceTask, "SolarPanels", configMINIMAL_STACK_SIZE, NULL, SOLAR_PANELS_INTERFACE_TASK_PRIORITY, &xSolarPanelsInterfaceTask);
    xTaskCreate( saveDataOnFlashMemoryTask, "SaveDataOnFlash", 10*configMINIMAL_STACK_SIZE, NULL, SAVE_DATA_ON_FLASH_MEMORY_TASK_PRIORITY, &xSaveDataOnFlashMemoryTask);

    xTaskCreate( debugTask, "DEBUG", 4*configMINIMAL_STACK_SIZE, NULL, DEBUG_TASK_PRIORITY, &xDebugTask);
}


//void vPrintEvent (char *string)
//{
//    volatile char msg[100];
//    volatile unsigned long systick;
//
//    systick = (TickType_t)xTaskGetTickCount();
//    sprintf((char*)msg, "\n[ %u ] -> ", systick);
//    uart_tx(msg);
//    sprintf(msg, "%s ", string);
//    uart_tx(msg);
//}

void vSetupHardware( void )
{
    taskDISABLE_INTERRUPTS();

    /*   External watchdog timer reset pin */
    P5DIR |= BIT4;
    P5OUT |= BIT4;
    obdh_setup();

    /*  SETUP CLOCKS */

    P7SEL |= BIT2+BIT3; //XT2
//    P3DIR |= BIT4;    // SCLK set out to pin
//    P3SEL |= BIT4;



    UCSCTL6 &= ~(XT2OFF | XT1OFF);            // Enable XT2 and XT1
    UCSCTL6 |= XCAP_3;                        // Internal load cap

//    do
//    {
//    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);  //TODO: rever este trecho
//                                            // Clear XT2,XT1,DCO fault flags
//    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
//    }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

    UCSCTL6 &= ~(XT2DRIVE0 | XT1DRIVE_3);       // Decrease XT2 Drive according to
                                                // expected frequency

    UCSCTL4 |= SELA_0 + SELS_5 + SELM_5;        // SMCLK = MCLK = XT2 , ACLK = XT1
    
/*
 * TODO: Adicionar retorno para cada setup, de modo a saber, ao fim desta função,
 * quais modulos foram corretamente configurados e quais não. Isto pode ser armazenado no log de eventos
 */
    //set GPIO pins (8.6 -> UCB1SCL, 8.5 -> UCB1SDA)
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8, GPIO_PIN5 + GPIO_PIN6);
    vI2cSetup(IMU_BASE_ADDRESS,MPU1_I2C_SLAVE_ADRESS);

    vImuConfig();

    uart_setup(9600);

    adc_setup_temp();
}


void vApplicationTickHook( void )
{

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
void vApplicationSetupTimerInterrupt( void )
{
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

void vApplicationIdleHook( void )
{
    /* Called on each iteration of the idle task.  In this case the idle task
    just enters a low(ish) power mode. */
    __bis_SR_register( LPM1_bits + GIE );
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
    /* Called if a call to pvPortMalloc() fails because there is insufficient
    free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    internally by FreeRTOS API functions that create tasks, queues or
    semaphores. */
    taskDISABLE_INTERRUPTS();
    for( ;; );

}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pxTask;
    ( void ) pcTaskName;

    /* Run time stack overflow checking is performed if
    configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}
/*-----------------------------------------------------------*/


