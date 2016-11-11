/*
 * watchdog.h
 *
 *  Created on: 11 de mai de 2016
 *      Author: mario
 */

#ifndef DRIVER_WATCHDOG_H_
#define DRIVER_WATCHDOG_H_

#include <msp430.h>

//mode
#define INTERVAL			0x10
#define WATCHDOG			0x00

//time to trigger
#define WD_64_uSEC			0x07 //TODO recalcular de acordo com os clocks disponiveis. pag.461
#define WD_512_uSEC			0x06
#define WD_2_mSEC			0x27
#define WD_8_2_mSEC			0x05
#define WD_15_6_mSEC		0x26
#define WD_32_8_mSEC		0x04
#define WD_250_mSEC			0x25
#define WD_524_3_mSEC		0x03
#define WD_1_SEC			0x24
#define WD_8_4_SEC			0x02
#define WD_16_SEC			0x23
#define WD_134_SEC			0x01
#define WD_4_MIN_16_SEC		0x22
#define WD_8_MIN_16_SEC		0x21
#define WD_30_MIN_47_5_SEC	0x00
#define WD_18_H_12_MIN_16_SEC	0x20


#include <msp430.h>

void watchdog_setup(char, char);
void wdt_reset_counter(void);
void wdt_hold_counter(void);
void wdt_release_counter(void);
void system_reboot(void);


#endif /* DRIVER_WATCHDOG_H_ */
