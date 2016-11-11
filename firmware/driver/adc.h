
#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
#include "../driver/obdh_hal.h"

void adc_setup_A4(void);
void adc_setup_temp(void);
uint16_t adc_read(void);

#endif
