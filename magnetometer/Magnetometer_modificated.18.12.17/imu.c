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


uint8_t imu_setup(void){uint8_t status, who_am_i;

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

    imu_slave_address = IMU0_I2C_SLAVE_ADRESS;

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

void write_i2c(uint8_t reg, uint8_t value)
{
    i2c_set_mode(IMU_BASE_ADDRESS, TRANSMIT_MODE);      /**< set to transmit mode        */
    i2c_send(IMU_BASE_ADDRESS, reg, NO_STOP);  /**< send the register address              */
    i2c_send(IMU_BASE_ADDRESS, value, NO_START);
}

void mag_setup(int8_t mode) {
    write_i2c(0x37, 0x02);
    i2c_set_slave(IMU_BASE_ADDRESS, 0x0C);
    write_i2c(0x0A, 0x00);
    if (mode == 0)
        write_i2c(0x0A, 0x0F); //16 bit output and Fuse ROM mode
    else
        write_i2c(0x0A, 0x12);     //16 bit output and Continuous measurement mode 1
    write_i2c(0x24, 0x40); //Wait for Data at Slave0
    write_i2c(0x26, 0x02); //Set where reading at slave 0 starts
    write_i2c(0x27, 0x88); //set offset at start reading and enable
}


void mag_read(uint8_t *p_imu_data) {


    volatile uint8_t imu_slave_address = 0x0C;
    uint8_t imu_status = IMU_WORKING;
    uint8_t WHOIAM_AK8963[1];

    i2c_set_slave(IMU_BASE_ADDRESS, imu_slave_address); /**< set the I2C slave to be IMU */
    i2c_set_mode(IMU_BASE_ADDRESS, TRANSMIT_MODE);      /**< set to transmit mode        */


    /**< try to send a register address */
    if(i2c_send(IMU_BASE_ADDRESS, 0x00 , NO_STOP) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }
    i2c_set_mode(IMU_BASE_ADDRESS, RECEIVE_MODE);       /**< change to receive mode */

    /**< try to read from the register address */
    if(i2c_receive_burst(IMU_BASE_ADDRESS, p_imu_data, 10, START_STOP) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }
}
void mag_read_asa(uint8_t *p_imu_data) {


    volatile uint8_t imu_slave_address = 0x0C;
    uint8_t imu_status = IMU_WORKING;
    uint8_t WHOIAM_AK8963[1];

    i2c_set_slave(IMU_BASE_ADDRESS, imu_slave_address); /**< set the I2C slave to be IMU */
    i2c_set_mode(IMU_BASE_ADDRESS, TRANSMIT_MODE);      /**< set to transmit mode        */


    /**< try to send a register address */
    if(i2c_send(IMU_BASE_ADDRESS, 0x10 , NO_STOP) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }
    i2c_set_mode(IMU_BASE_ADDRESS, RECEIVE_MODE);       /**< change to receive mode */

    /**< try to read from the register address */
    if(i2c_receive_burst(IMU_BASE_ADDRESS, p_imu_data, 3, START_STOP) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }
}
void read_magnetometer (int16_t *mag_temp) {
    uint8_t buffer[20];
    mag_read(buffer);
    mag_temp[0]          =  imu_mag(buffer[4], buffer[3]);
    mag_temp[1]          =  imu_mag(buffer[6], buffer[5]);
    mag_temp[2]          =  imu_mag(buffer[8], buffer[7]);
}
