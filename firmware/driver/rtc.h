/*
 * rtc.h
 *
 *  Created on: 21 de jun de 2016
 *      Author: mfrata
 */

#ifndef UTIL_RTC_H_
#define UTIL_RTC_H_

#include <msp430.h>
#include <stdint.h>


/**
 *     RTC_setup will configure the RTC_B to the calendar mode, generating one interruption
 * every second. RTC uses a external crystal(XT1) by default, it can't be modified.
 */
void RTC_setup(void);

/**
 *     RTC_get functions returns current time
 */
uint16_t RTC_getSec(void);

//todo implement the min and hour functions
//current still only incrementing  the seconds
uint8_t RTC_getMin(void);
uint8_t RTC_getHour(void);


#endif /* UTIL_RTC_H_ */
