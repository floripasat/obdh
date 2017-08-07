/*
 * main.c
 *
 *  Created on: 10 de jul de 2017
 *      Author: André
 */

#include <msp430.h>

#include "i2c.h"
#include "antenna.h"
#include "temp_data.h"

int main(void) {

    WDTCTL = WDTPW + WDTHOLD;

    ant_setup();                // Setup the hardware symbolic assignments of the antenna simulation.
    i2c_setup();                // Setup the I2C to work as slave.
    temperature_setup();        // Setup the peripherals of temperature measurement.
    run();                      // Run the loop that simulates the antenna behavior.
}
