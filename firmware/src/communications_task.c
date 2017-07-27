/*
 * communications_task.c
 *
 * Copyright (C) 2017, Universidade Federal de Santa Catarina
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
 * \file communications_task.c
 *
 * \brief Task that deals with the downlink and uplink communications
 *
 * \author Elder Tramontin
 *
 */

#include "communications_task.h"

void communications_task( void *pvParameters )
{
    TickType_t last_wake_time;
    last_wake_time = xTaskGetTickCount();
    uint16_t current_turn = 0, turns_to_wait;
    uint8_t energy_level;

    while(1) {

        /**< verify if some telecommand was received on radio */

        energy_level = read_current_energy_level();

        switch (energy_level) {
        case ENERGY_L1_MODE:
        case ENERGY_L2_MODE:
            turns_to_wait = PERIODIC_DOWNLINK_INTERVAL_TURNS;
            break;

        case ENERGY_L3_MODE:
            turns_to_wait = PERIODIC_DOWNLINK_INTERVAL_TURNS * 2;
            break;

        case ENERGY_L4_MODE:
        default:
            turns_to_wait = 0xFFFF;
        }

        if(++current_turn > turns_to_wait) {
            //send_packet(satellite_data);
            current_turn = 0;
        }

        vTaskDelayUntil( (TickType_t *) &last_wake_time, COMMUNICATIONS_TASK_PERIOD_TICKS );
    }

    vTaskDelete( NULL );
}
