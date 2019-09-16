/*
 * debug_mutex.h
 * 
 * Copyright (C) 2017-2019, Universidade Federal de Santa Catarina
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
 * along with FloripaSat-OBDH. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Debug mutex implementation.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.2.6
 *
 * \date 02/12/2018
 *
 * \defgroup debug_mutex Mutex
 * \ingroup debug
 * \{
 */

#include "debug.h"
#include "debug_config.h"

SemaphoreHandle_t xDebugSemaphore = NULL;

bool debug_mutex_create()
{
    // Create a mutex type semaphore
    xDebugSemaphore = xSemaphoreCreateMutex();

    if (xDebugSemaphore == NULL)
    {
        debug_print_event_from_module(DEBUG_ERROR, "Debug", "Error creating a mutex!");
        debug_new_line();

        return false;
    }

    return true;
}

bool debug_mutex_take()
{
    if (xDebugSemaphore != NULL)
    {
        // See if we can obtain the semaphore. If the semaphore is not
        // available wait DEBUG_MUTEX_WAIT_TIME_MS ms to see if it becomes free
        if (xSemaphoreTake(xDebugSemaphore, pdMS_TO_TICKS(DEBUG_MUTEX_WAIT_TIME_MS)) == pdTRUE)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool debug_mutex_give()
{
    if (xDebugSemaphore != NULL)
    {
        xSemaphoreGive(xDebugSemaphore);

        return true;
    }
    else
    {
        return false;
    }
}

//! \} End of debug_mutex group
