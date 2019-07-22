/*
 * main.c
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
 * \brief Main file
 *
 * \author Elder Tramontin
 */

#include "obdh.h"
#include "config.h"

int main(void) {

    setup_hardware();                       /**< setup the watchdog timers, I/O pins, communications and other peripherals */

#if OBDH_RESET_MEMORY_ON_BOOT == 1
    reset_memory();

    return 0;
#endif // OBDH_RESET_MEMORY_ON_BOOT

#if OBDH_EXECUTE_DEPLOYMENT_ROUTINE == 1
    if(verify_deployment_status() == ANTENNAS_NOT_DEPLOYED){    /**< **** TODO: Verify the byte order in I2C (MSB or LSB first). */
        hibernate();                        /**< sleep for 45 minutes */
        antennas_deployment_routine();      /**< open the antennas */
    }
#endif // OBDH_EXECUTE_DEPLOYMENT_ROUTINE

    create_tasks();

    vTaskStartScheduler();                  /**< start to run the tasks */

    for( ;; );                              /**< will never reach here */

	return 0;                               /**< will never reach here */
}
