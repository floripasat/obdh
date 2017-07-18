#include <msp430.h>
#include <stdint.h>

#include "i2c.h"
#include "antenna.h"

int main(void) {

    WDTCTL = WDTPW + WDTHOLD;

    ant_setup();
    i2c_setup();

    while(1) {
        switch_command();
    }
}
