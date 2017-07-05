#include <msp430.h>
#include <stdint.h>
#include "../hal/obdh_hal.h"
#include "../driver/spi.h"

#define SOLAR_PANEL_X 0
#define SOLAR_PANEL_Y 1
#define SOLAR_PANEL_Z 2

void solar_panel_setup (void);

int16_t read_temperature (uint8_t panel_selection);
