/*
 * adc.h
 *
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina
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
 * \brief Functions of MSP430's ADC12 peripheral
 *
 * \author Elder Tramontin
 *
 * \version 0.2.2
 *
 * \defgroup adc ADC
 * \ingroup drivers
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

#include "../hal/obdh_hal.h"

#define AVCC 3.0            /**< reference voltage */
#define ADC_RANGE 4095.0    /**< ADC resolution (12 bits) */

/**
 * \brief Configure the channels, sampling period, voltage reference and operation mode of the ADC peripheral.
 * \return None
 */
void adc_setup(void);

/**
 * \brief Read a channel of the ADC.
 * \param channel is the channel to be read (0-15)
 * \return a 12-bit value, digital conversion of the analog voltage in the channel
 */
uint16_t adc_read(uint8_t channel);

#endif // ADC_H_

//! \} End of adc group
