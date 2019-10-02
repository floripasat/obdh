/*
 * eps_interface_task.h
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
 * along with FloripaSat-OBDH.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 /**
 * \brief Task that deals with the EPS interface
 *
 * \author Elder Tramontin
 *
 * \version 1.0.5
 *
 * \defgroup eps_interface EPS Interface
 * \ingroup tasks
 * \{
 */

#ifndef SRC_EPS_INTERFACE_TASK_H_
#define SRC_EPS_INTERFACE_TASK_H_

#include "../include/floripasat_def.h"
#include "FreeRTOS.h"
#include "task.h"
#include "task_queues.h"
#include "../interface/eps.h"
#include "../include/msp_internal.h"

/*
 * PRIORITY =   5
 * FREQUENCY =  1Hz
 */

#define EPS_INTERFACE_TASK_PRIORITY          5
#define EPS_INTERFACE_TASK_PERIOD_MS         1000
#define EPS_INTERFACE_TASK_PERIOD_TICKS      ( EPS_INTERFACE_TASK_PERIOD_MS / portTICK_PERIOD_MS )

/**
 * \brief variable which holds the task reference.
 */
static xTaskHandle eps_interface_task_handle;

/**
 * \brief The task witch do communication with the EPS_INTERFACE module.
 * \param pvParameters Not used
 */
void eps_interface_task( void *pvParameters );

#endif /* SRC_EPS_INTERFACE_TASK_H_ */

//! \} End of eps_interface group
