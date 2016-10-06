/*
 * fast_tasks.h
 *
 *  Created on: 8 de set de 2016
 *      Author: Fsat
 */

#ifndef UART_TASK_H_
#define UART_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "hal/uart.h"

#include "globals.h"

#include "msp430.h"



void prvSendUartTask( void *pvParameters );




#endif /* UART_TASK_H_ */

