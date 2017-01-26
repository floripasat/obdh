
#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include "../driver/obdh_hal.h"

#define AVCC 3.0
#define ADC_RANGE 4095.0


void adc_setup(void);
uint16_t adc_read(uint8_t channel);

#endif
