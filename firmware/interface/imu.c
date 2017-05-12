/*
 * imu.c
 *
 *  Created on: 26 de mai de 2016
 *      Author: mario
 */

#include <imu.h>

uint8_t imu_temp_data[20];


uint8_t imu_setup(void){
    uint8_t status, who_am_i;
    //    i2c_setup(1);

	//imu_i2c_write(MPU9150_PWR_MGMT_1, 0x00);
    imu_temp_data[0] = MPU9150_PWR_MGMT_1;   //reg address
    imu_temp_data[1] = 0x00;                 //data

    i2c_set_mode(IMU_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_set_slave(IMU_BASE_ADDRESS, IMU0_I2C_SLAVE_ADRESS);
    i2c_send(IMU_BASE_ADDRESS, imu_temp_data[0], NO_STOP);
    i2c_send(IMU_BASE_ADDRESS, imu_temp_data[1], NO_START);

    imu_temp_data[0] = MPU9150_ACCEL_CONFIG;

	if (IMU_ACC_RANGE == 2.0) {
	    imu_temp_data[1] = 0x00;   // config for +-2g range
	} else {
	    imu_temp_data[1] = 0x18;   // config for +-16g range
	}

	i2c_send(IMU_BASE_ADDRESS, imu_temp_data[0], NO_STOP);
    i2c_send(IMU_BASE_ADDRESS, imu_temp_data[1], NO_START);


    i2c_send(IMU_BASE_ADDRESS, MPU9150_WHO_AM_I, NO_STOP);
    i2c_set_mode(IMU_BASE_ADDRESS, RECEIVE_MODE);
    i2c_receive(IMU_BASE_ADDRESS, &who_am_i, START_STOP);

    if(who_am_i == IMU_WHO_AM_I_VALUE) {
        status = IMU_WORKING;
    }
    else {
        status = IMU_NOT_WORKING;
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

    i2c_set_slave(IMU_BASE_ADDRESS, imu_slave_address);
    i2c_set_mode(IMU_BASE_ADDRESS, TRANSMIT_MODE);
    if(i2c_send(IMU_BASE_ADDRESS, MPU9150_ACCEL_XOUT_H, NO_STOP) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }
    i2c_set_mode(IMU_BASE_ADDRESS, RECEIVE_MODE);

    if(i2c_receive(IMU_BASE_ADDRESS, p_imu_data, NO_STOP) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }

    if(i2c_receive_burst(IMU_BASE_ADDRESS, (p_imu_data + 1), 12) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }

    if(i2c_receive(IMU_BASE_ADDRESS, (p_imu_data + 13), NO_START) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }

    return imu_status;
}
