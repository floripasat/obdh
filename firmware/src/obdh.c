#include "../include/obdh.h"

void vDeployAntenna()
{

}

void prvCreateTasks()
{
//    xTaskCreate( prvEPSTask, "EPS", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &epsTask );
    xTaskCreate( prvImuTask, "IMU", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &xImuTask );
//    xTaskCreate( prvOBDHTask, "OBDH", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &obdhTask );
//    xTaskCreate( prvTTCTask, "TTC", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &ttcTask );
//    xTaskCreate( prvReadTempTask, "ReadTemperature", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &readTemp );
//    xTaskCreate( prvWdtTask, "WDT", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, &wdtTask );
//    xTaskCreate( prvDebugTask, "DEBUG", 4*configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &debugTask );
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

    /* Disable the watchdog. */
    WDTCTL = WDTPW + WDTHOLD;

    P5DIR |= BIT4;
    P5OUT |= BIT4;

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


