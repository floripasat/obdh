/*
 * msp_internal.c
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
 * \brief Interface with internal components of the OBDH SoC
 *
 * \author Mario Baldini
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 1.0.3
 *
 * \addtogroup msp_internal
 */

#include "../interface/debug/debug.h"

#include "msp_internal.h"

const float CURR_COEF = (AVCC / (ADC_RANGE * RL_VALUE * CURRENT_GAIN * RSENSE_VALUE));
uint32_t minutes_counter = 0;

float obdh_temperature_convert(uint16_t temperature_raw) {
	float temperature = (float)(((long)temperature_raw * 2 - CALADC12_15V_30C) * (85 - 30)) /
                        (CALADC12_15V_85C - CALADC12_15V_30C) + 30.0f;  // based on datasheet
	return temperature;
}

uint16_t obdh_temperature_read(void) {
    uint16_t temp_raw;
    temp_raw = adc_read(INTERNAL_TEMP_SENSOR_ADC_CH);

    return temp_raw;
}

float obdh_current_convert(uint16_t curr_raw) {
    volatile float curr;
    curr = ((float)curr_raw) * CURR_COEF;

    return curr;
}

uint16_t obdh_current_read(void) {
    uint16_t curr_raw;
    curr_raw = adc_read(OBDH_CURRENT_ADC_CH);

    return curr_raw;
}

float obdh_voltage_convert(uint16_t volt_raw) {
    float volt = volt_raw * AVCC * VOLTAGE_DIVISOR / ADC_RANGE;
    return volt;
}

uint16_t obdh_voltage_read(void) {
    uint16_t volt_raw;
    volt_raw = adc_read(VCC_3V3_ADC_CH);

    return volt_raw;
}

uint8_t read_fault_flags(void) {
    return (UCSCTL7 & 0x0F); // XT2OFFG | XT1HFOFFG | XT1LFOFFG | DCOFFG
}

uint32_t read_reset_value(void) {
    return flash_read_long(RESET_ADDR_FLASH);
}

uint8_t read_reset_cause(void) {
    return (SYSRSTIV & 0xFF);
}

void update_reset_value(void) {
    uint32_t previous_value = read_reset_value() & 0xFFFFFF;                                        // retrieve the last reset counter value
    uint32_t new_value = ((uint32_t)read_reset_cause()) << 24 | ((previous_value + 1) & 0xFFFFFF);  // join reset cause with reset counter value incremented

    debug_print_event_from_module(DEBUG_INFO, "System", "Previous reset value = ");
    debug_print_dec(previous_value);
    debug_print_msg(", new reset value = ");
    debug_print_dec(new_value);
    debug_new_line();

    flash_erase(RESET_ADDR_FLASH);                                                                  // erase the memory to be able to write
    flash_write_long(new_value, RESET_ADDR_FLASH);                                                  // store reset cause and counter
}

uint32_t read_time_counter(void) {
    return minutes_counter;
}

void update_time_counter(void) {
    uint32_t addr;

    addr = ++minutes_counter % 32;          // store as a circular vector

    if (addr == 0) {
        flash_erase(TIME_COUNTER_ADDR_FLASH);
    }
    flash_write_long(minutes_counter, (TIME_COUNTER_ADDR_FLASH + addr));
}

void restore_time_counter(void) {
    uint32_t *addr_check;
    uint32_t zero = 0;
    addr_check = (uint32_t*)(END_TIME_COUNTER_ADDR_FLASH);

    while(*addr_check == 0xFFFFFFFF) {      // retrieve of the circular vector
        addr_check--;
        if (addr_check < TIME_COUNTER_ADDR_FLASH) {
            addr_check = &zero;
            break;
        }
    }
    minutes_counter = *addr_check;

    debug_print_event_from_module(DEBUG_INFO, "System", "Restored time counter: ");
    debug_print_dec(minutes_counter);
    debug_print_msg(" min");
    debug_new_line();
}

uint8_t read_current_state(void) {
    uint8_t current_state, operation_mode, energy_level;

    current_state = flash_read_single(CURRENT_STATE_ADDR_FLASH);

    operation_mode = current_state & OPERATION_MODE_MASK;
    energy_level = current_state & ENERGY_LEVEL_MASK;

    // use a default value in case of the memory holds a wrong one
    if ( ( operation_mode != NORMAL_OPERATION_MODE ) && ( operation_mode != HIBERNATION_MODE ) ) {
        operation_mode = NORMAL_OPERATION_MODE;
    }
    if ( ( energy_level != ENERGY_L1_MODE ) && ( energy_level != ENERGY_L2_MODE ) &&
         ( energy_level != ENERGY_L3_MODE ) && ( energy_level != ENERGY_L4_MODE ) &&
         ( energy_level != ENERGY_L5_MODE )) {
        energy_level = ENERGY_L3_MODE;
    }

    current_state = operation_mode;
    current_state |= energy_level;

    return current_state;
}

void update_energy_level(uint8_t new_energy_level) {
    uint8_t operation_mode;
    uint8_t time_state_changed;
    uint8_t new_state;

    operation_mode = read_current_operation_mode();
    new_state = operation_mode | (new_energy_level & ENERGY_LEVEL_MASK);

    time_state_changed = read_time_state_changed();
    flash_erase((uint32_t *)CURRENT_STATE_ADDR_FLASH);
    flash_write_single(new_state, CURRENT_STATE_ADDR_FLASH);

    flash_write_long(time_state_changed, TIME_STATE_CHANGED_ADDR_FLASH);
}

uint32_t read_time_state_changed(void) {
    return flash_read_long(TIME_STATE_CHANGED_ADDR_FLASH);
}

void update_operation_mode(uint8_t new_operation_mode) {
    uint8_t energy_level;
    uint8_t new_state;

    energy_level = read_current_energy_level();
    new_state = energy_level | (new_operation_mode & OPERATION_MODE_MASK);
    flash_erase((uint32_t *)CURRENT_STATE_ADDR_FLASH);
    flash_write_single(new_state, CURRENT_STATE_ADDR_FLASH);

    flash_write_long(read_time_counter(), TIME_STATE_CHANGED_ADDR_FLASH);
}

void set_hibernation_period_min(uint16_t min) {
    flash_write_long((uint32_t)min, HIBERNATION_PERIOD_ADDR_FLASH);
}

uint16_t get_hibernation_period_min(void) {
    return (uint16_t)flash_read_long(HIBERNATION_PERIOD_ADDR_FLASH);
}

void low_power_mode_sleep(void) {
    __bis_SR_register(LPM1_bits | GIE);       // Enter LPM1, enable interrupts
}

//! \} End of msp_internal group
