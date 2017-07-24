/*
 * imu.c
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
 * \file imu.c
 *
 * \brief This file deals with the IMU module
 *
 * \author Mario Baldini
 *
 */


#include "imu.h"

uint8_t imu_temp_data[20];


uint8_t imu_setup(void){
    uint8_t status, who_am_i;

    imu_temp_data[0] = MPU9150_PWR_MGMT_1;   /**< power management register address */
    imu_temp_data[1] = 0x00;                 /**< data */

    i2c_set_mode(IMU_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_set_slave(IMU_BASE_ADDRESS, IMU0_I2C_SLAVE_ADRESS);
    i2c_send(IMU_BASE_ADDRESS, imu_temp_data[0], NO_STOP);  /**< send the register address              */
    i2c_send(IMU_BASE_ADDRESS, imu_temp_data[1], NO_START); /**< send the data to write in the register */

    imu_temp_data[0] = MPU9150_ACCEL_CONFIG;

	if (IMU_ACC_RANGE == 2.0) {
	    imu_temp_data[1] = 0x00;   /**< config for +-2g range  */
	} else {
	    imu_temp_data[1] = 0x18;   /**< config for +-16g range */
	}

	i2c_send(IMU_BASE_ADDRESS, imu_temp_data[0], NO_STOP);  /**< send the register address              */
    i2c_send(IMU_BASE_ADDRESS, imu_temp_data[1], NO_START); /**< send the data to write in the register */


    i2c_send(IMU_BASE_ADDRESS, MPU9150_WHO_AM_I, NO_STOP);  /**< send the register address          */
    i2c_set_mode(IMU_BASE_ADDRESS, RECEIVE_MODE);           /**< change to receive mode             */
    i2c_receive(IMU_BASE_ADDRESS, &who_am_i, START_STOP);   /**< read the data from the register    */

    if(who_am_i == IMU_WHO_AM_I_VALUE) {                    /**< if the read value is the expected  */
        status = IMU_WORKING;                               /**< IMU is OK     */
    }
    else {
        status = IMU_NOT_WORKING;                           /**< IMU is not OK */
    }

    return status;
}


uint8_t imu_read(uint8_t *p_imu_data, uint8_t imu_select)
{
    volatile uint8_t imu_slave_address = 0x00;
    uint8_t imu_status = IMU_WORKING;

    switch(imu_select)
    {
        case IMU1:
            imu_slave_address = IMU0_I2C_SLAVE_ADRESS;
            break;
        case IMU2:
            imu_slave_address = IMU1_I2C_SLAVE_ADRESS;
            break;
    }

    i2c_set_slave(IMU_BASE_ADDRESS, imu_slave_address); /**< set the I2C slave to be IMU */
    i2c_set_mode(IMU_BASE_ADDRESS, TRANSMIT_MODE);      /**< set to transmit mode        */

    /**< try to send a register address */
    if(i2c_send(IMU_BASE_ADDRESS, MPU9150_ACCEL_XOUT_H, NO_STOP) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }
    i2c_set_mode(IMU_BASE_ADDRESS, RECEIVE_MODE);       /**< change to receive mode */

    /**< try to read from the register address */
    if(i2c_receive_burst(IMU_BASE_ADDRESS, p_imu_data, 14, START_STOP) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }

    return imu_status;
}
