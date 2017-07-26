#ifndef TEMP_DATA_H_
#define TEMP_DATA_H_

#include <msp430.h>
#include <stdint.h>
#include "misc_def.h"

#define CALADC12_15V_30C  *((unsigned int *)0x1A1A)   // Temperature Sensor Calibration-30 C
                                                      //See device datasheet for TLV table memory mapping
#define CALADC12_15V_85C  *((unsigned int *)0x1A1C)   // Temperature Sensor Calibration-85 C

void temperature_setup(void);

uint16_t temperature_read(void);

#endif
