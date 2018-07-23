/*
 * sun_position.h
 *
 *  Created on: 17 de jul de 2018
 *      Author: root
 */

#ifndef SUN_POSITION_H
#define SUN_POSITION_H


#define PARAMETER 0                 //1 => receive address by parameter; others => use satellite_data
#define current_conversion (0.000148000148)
#define voltage_conversion (0.00117887667887668)
#define PI 3.14159265

#include "include/floripasat_def.h"
#include <math.h>

typedef struct
{
    double X;
    double Y;
    double teta;
}position_sun;

#if PARAMETER == 1
position_sun sun_position(uint8_t *adress);
#else
position_sun sun_position();
#endif

#endif
