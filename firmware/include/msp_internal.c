/*
 * obdh.c
 *
 *  Created on: 30 de mai de 2016
 *      Author: mario
 */

// Interface with internal components of the OBDH SoC

#include <msp_internal.h>

const float CURR_COEF = (AVCC / (ADC_RANGE * RL_VALUE * CURRENT_GAIN * RSENSE_VALUE));

float obdh_temperature_convert(uint16_t temperature_raw){
	float temperature = (float)(((long)temperature_raw * 2 - CALADC12_15V_30C) * (85 - 30)) /
            (CALADC12_15V_85C - CALADC12_15V_30C) + 30.0f;
	return temperature;
}

uint16_t obdh_temperature_read(void){
    uint16_t temp_raw;
    temp_raw = adc_read(INTERNAL_TEMP_SENSOR_ADC_CH);

    return temp_raw;
}

float obdh_current_convert(uint16_t curr_raw){
    volatile float curr;
    curr = ((float)curr_raw) * CURR_COEF;

    return curr;
}

uint16_t obdh_current_read(void){
    uint16_t curr_raw;
    curr_raw = adc_read(OBDH_CURRENT_ADC_CH);

    return curr_raw;
}

float obdh_voltage_convert(uint16_t volt_raw){
    float volt = volt_raw * AVCC * VOLTAGE_DIVISOR / ADC_RANGE;
    return volt;
}

uint16_t obdh_voltage_read(void){
    uint16_t volt_raw;
    volt_raw = adc_read(VCC_3V3_ADC_CH);

    return volt_raw;
}

uint8_t read_fault_flags(void){
    return (UCSCTL7 & 0x0F); //XT2OFFG | XT1HFOFFG | XT1LFOFFG | DCOFFG
}

uint32_t read_reset_value(void) {
    return flash_read_long(RESET_ADDR_FLASH);
}

uint8_t read_reset_cause(void){
    return (SYSRSTIV & 0xFF);
}

void update_reset_value(void){
    uint32_t previous_value;
    uint32_t new_value;

    previous_value = read_reset_value() & 0xFFFFFF;
    new_value = ((uint32_t)read_reset_cause())<<24 | ((previous_value + 1) & 0xFFFFFF);
    flash_erase(RESET_ADDR_FLASH);
    flash_write_long(new_value, RESET_ADDR_FLASH);
}

uint8_t read_current_state() {
    return flash_read_single(CURRENT_STATE_ADDR_FLASH);
}

void update_energy_level(uint8_t new_energy_level) {
    uint8_t operation_mode;
    uint8_t new_state;

    operation_mode = read_current_state() & OPERATION_MODE_MASK;
    new_state = operation_mode | (new_energy_level & ENERGY_LEVEL_MASK);
    flash_erase(CURRENT_STATE_ADDR_FLASH);
    flash_write_single(new_state, CURRENT_STATE_ADDR_FLASH);
}

void update_operation_mode(uint8_t new_operation_mode) {
    uint8_t energy_level;
    uint8_t new_state;

    energy_level = read_current_state() & ENERGY_LEVEL_MASK;
    new_state = energy_level | (new_operation_mode & OPERATION_MODE_MASK);
    flash_erase(CURRENT_STATE_ADDR_FLASH);
    flash_write_single(new_state, CURRENT_STATE_ADDR_FLASH);
}
