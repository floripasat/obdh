/*
 * sun_position.c
 *
 *  Created on: 17 de jul de 2018
 *      Author: root
 */

#include "sun_position.h"

#if PARAMETER == 1
position_sun sun_position(uint8_t *adress)
#else
position_sun sun_position()
#endif
{
    double Power[4], X, Y, teta;
    uint16_t *painel_pointer;
    position_sun Result;
    #if PARAMETER == 1
    painel_pointer =  (uint16_t*) adress;
    #else
    painel_pointer =  (uint16_t*) &satellite_data.solar_panels[0];
    #endif
    uint8_t a = satellite_data.solar_panels[0];
    uint8_t b = satellite_data.solar_panels[1];
    uint16_t c = painel_pointer[0];

    double i_1, i_2, i_3, i_4, v_1, v_2;
    uint16_t iu_1, iu_2, iu_3, iu_4, vu_1, vu_2;

    iu_1 = ((satellite_data.solar_panels[0] << 8) | satellite_data.solar_panels[1]);
    iu_2 = ((satellite_data.solar_panels[4] << 8) | satellite_data.solar_panels[5]);
    iu_3 = ((satellite_data.solar_panels[6] << 8) | satellite_data.solar_panels[7]);
    iu_4 = ((satellite_data.solar_panels[2] << 8) | satellite_data.solar_panels[3]);
    vu_1 = ((satellite_data.solar_panels[12] << 8) | satellite_data.solar_panels[13]);
    vu_2 = ((satellite_data.solar_panels[14] << 8) | satellite_data.solar_panels[15]);

    i_1 = iu_1 * current_conversion;
    i_2 = iu_2 * current_conversion;
    i_3 = iu_3 * current_conversion;
    i_4 = iu_4 * current_conversion;
    v_1 = vu_1 * voltage_conversion;
    v_2 = vu_2 * voltage_conversion;

    Power[0] = i_1 * v_1;
    Power[1] = i_2 * v_2;
    Power[2] = i_3 * v_2;
    Power[3] = i_4 * v_1;

    X = Power[0]-Power[1];

    Y = Power[3]-Power[2];

    teta = atan2(Y,X);
    teta = teta*180/PI;

    Result.X = X;
    Result.Y = Y;
    Result.teta = teta;

    return Result;
}
