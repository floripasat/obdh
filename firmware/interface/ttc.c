/*
 * ttc.c
 *
 *  Created on: 09 de fev de 2017
 *      Author: elder
 */

#include <ttc.h>

#define TIME_TO_PROCESS_CMD     1600        /**< 100 microseconds */

beacon_packet_t ttc_copy_data(void){
    beacon_packet_t beacon_packet;

    /**< V_batteries */
    beacon_packet.batteries[0]  = satellite_data.battery_monitor[4];
    beacon_packet.batteries[1]  = satellite_data.battery_monitor[5];
    beacon_packet.batteries[2]  = satellite_data.battery_monitor[6];
    beacon_packet.batteries[3]  = satellite_data.battery_monitor[7];

    /**< I_SolarPanels */
    beacon_packet.solar_panels[0]  = satellite_data.adc_solar_panels[0];
    beacon_packet.solar_panels[1]  = satellite_data.adc_solar_panels[1];
    beacon_packet.solar_panels[2]  = satellite_data.adc_solar_panels[2];
    beacon_packet.solar_panels[3]  = satellite_data.adc_solar_panels[3];
    beacon_packet.solar_panels[4]  = satellite_data.adc_solar_panels[4];
    beacon_packet.solar_panels[5]  = satellite_data.adc_solar_panels[5];
    beacon_packet.solar_panels[6]  = satellite_data.adc_solar_panels[6];
    beacon_packet.solar_panels[7]  = satellite_data.adc_solar_panels[7];
    beacon_packet.solar_panels[8]  = satellite_data.adc_solar_panels[8];
    beacon_packet.solar_panels[9]  = satellite_data.adc_solar_panels[9];
    beacon_packet.solar_panels[10] = satellite_data.adc_solar_panels[10];
    beacon_packet.solar_panels[11] = satellite_data.adc_solar_panels[11];

    /**< V_SolarPanels */
    beacon_packet.solar_panels[12] = satellite_data.adc_solar_panels[12];
    beacon_packet.solar_panels[13] = satellite_data.adc_solar_panels[13];
    beacon_packet.solar_panels[14] = satellite_data.adc_solar_panels[14];
    beacon_packet.solar_panels[15] = satellite_data.adc_solar_panels[15];
    beacon_packet.solar_panels[16] = satellite_data.adc_solar_panels[16];
    beacon_packet.solar_panels[17] = satellite_data.adc_solar_panels[17];

    /**< T_Batteries */
    beacon_packet.batteries[4] = satellite_data.ads1248[3];
    beacon_packet.batteries[5] = satellite_data.ads1248[4];
    beacon_packet.batteries[6] = satellite_data.ads1248[5];
    beacon_packet.batteries[7] = satellite_data.ads1248[6];
    beacon_packet.batteries[8] = satellite_data.ads1248[7];
    beacon_packet.batteries[9] = satellite_data.ads1248[8];

    /**< Q_Batteries */
    beacon_packet.batteries[10] = satellite_data.battery_monitor[10];
    beacon_packet.batteries[11] = satellite_data.battery_monitor[11];

    /**< IMU */
    beacon_packet.imu[0] = satellite_data.imu[0];
    beacon_packet.imu[1] = satellite_data.imu[1];
    beacon_packet.imu[2] = satellite_data.imu[2];
    beacon_packet.imu[3] = satellite_data.imu[3];
    beacon_packet.imu[4] = satellite_data.imu[4];
    beacon_packet.imu[5] = satellite_data.imu[5];
    beacon_packet.imu[6] = satellite_data.imu[6];
    beacon_packet.imu[7] = satellite_data.imu[7];
    beacon_packet.imu[8] = satellite_data.imu[8];
    beacon_packet.imu[9] = satellite_data.imu[9];
    beacon_packet.imu[10] = satellite_data.imu[10];
    beacon_packet.imu[11] = satellite_data.imu[11];

    /**< System time */
    beacon_packet.system_time[0] = satellite_data.systick[0];
    beacon_packet.system_time[1] = satellite_data.systick[1];
    beacon_packet.system_time[2] = satellite_data.systick[2];
    beacon_packet.system_time[3] = satellite_data.systick[3];

    /**< FSat status */
    beacon_packet.satellite_status[0] = satellite_data.system_status[5];
    beacon_packet.satellite_status[1] = satellite_data.task_scheduler[0];

    /**< Reset counter */
    beacon_packet.reset_counter[0] = satellite_data.system_status[1];
    beacon_packet.reset_counter[1] = satellite_data.system_status[2];

    return beacon_packet;
}

void ttc_send_data(ttc_packet_t* ttc_packet) {
    sspi_tx(TTC_CMD_DATA_TRANSFER);             /**< send the data transfer command                     */
    sspi_tx_multiple((uint8_t*) ttc_packet,
                     sizeof(ttc_packet_t));     /**< send the data                  */
}


uint8_t ttc_send_mutex_request(void) {
    uint8_t response;

    sspi_tx(TTC_CMD_TX_MUTEX_REQUEST);          /**< send the mutex request command                     */
    __delay_cycles(TIME_TO_PROCESS_CMD);        /**< wait 100us until TT&C process the received data    */
    response = sspi_rx();                       /**< receive the response                               */

    return response;
}

void ttc_tx_mutex_release(void) {
    sspi_tx(TTC_CMD_TX_MUTEX_RELEASE);          /**< send the mutex release command                     */
}


void ttc_send_shutdown(void) {
    uint8_t response;

    do {
        sspi_tx(TTC_CMD_SHUTDOWN);              /**< send the shutdown command                          */
        __delay_cycles(TIME_TO_PROCESS_CMD);    /**< wait 100us until TT&C process the received data    */
        response = sspi_rx();                   /**< receive the ACK/NACK                               */

    } while(response != TTC_SHUTDOWN_ACK);      /**< repeat until receive ACK                           */
}
