/*
 * imu.c
 *
 *  Created on: 26 de mai de 2016
 *      Author: mario
 */

#include <imu.h>

uint8_t imu_temp_data[20];


uint8_t imu_setup(void){
//    i2c_setup(1);

	//imu_i2c_write(MPU9150_PWR_MGMT_1, 0x00);
    imu_temp_data[0] = MPU9150_PWR_MGMT_1;   //reg address
    imu_temp_data[1] = 0x00;                 //data

    i2c_set_mode(IMU_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_set_slave(IMU_BASE_ADDRESS, MPU0_I2C_SLAVE_ADRESS);
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

    uint8_t who_am_i = 0;
    i2c_send(IMU_BASE_ADDRESS, MPU9150_WHO_AM_I, NO_STOP);
    i2c_set_mode(IMU_BASE_ADDRESS, RECEIVE_MODE);
    who_am_i = i2c_receive(IMU_BASE_ADDRESS, START_STOP);

    return who_am_i;
}


void imu_read(uint8_t *p_imu_data, uint8_t imu_select)
{
    volatile uint8_t ucSlaveAddress = 0x00;

    switch(imu_select)
    {
        case IMU1:
            ucSlaveAddress = MPU0_I2C_SLAVE_ADRESS;
            break;
        case IMU2:
            ucSlaveAddress = MPU1_I2C_SLAVE_ADRESS;
            break;
    }

//    vI2cSetSlave( IMU_BASE_ADDRESS, ucSlaveAddress);
    i2c_set_mode(IMU_BASE_ADDRESS, TRANSMIT_MODE);
    i2c_send(IMU_BASE_ADDRESS, MPU9150_ACCEL_XOUT_H, NO_STOP);
    i2c_set_mode(IMU_BASE_ADDRESS, RECEIVE_MODE);

    p_imu_data[0] = i2c_receive(IMU_BASE_ADDRESS, NO_STOP);

    i2c_receive_burst(IMU_BASE_ADDRESS, p_imu_data + 1, 12);

    p_imu_data[13] = i2c_receive(IMU_BASE_ADDRESS, NO_START);
}




//char* imu_data2string(char* stringBuffer, char* imuData, float accRange,
//        float gyrRange) {
//
//        char accXH = imuData[0];
//        char accXL = imuData[1];
//        char accYH = imuData[2];
//        char accYL = imuData[3];
//        char accZH = imuData[4];
//        char accZL = imuData[5];
//
//        char tempH = imuData[6];
//        char tempL = imuData[7];
//
//        char gyrXH = imuData[8];
//        char gyrXL = imuData[9];
//        char gyrYH = imuData[10];
//        char gyrYL = imuData[11];
//        char gyrZH = imuData[12];
//        char gyrZL = imuData[13];
//
//        float temp = (float) (((tempH << 8) | tempL) / 340.0 + 35.0);
//
//        float accX = (float) (((accXH << 8 | accXL) * accRange) / 32768.0);
//        float accY = (float) (((accYH << 8 | accYL) * accRange) / 32768.0);
//        float accZ = (float) (((accZH << 8 | accZL) * accRange) / 32768.0);
//
//        float gyrX = (float) (((gyrXH << 8 | gyrXL) * gyrRange) / 32768.0);
//        float gyrY = (float) (((gyrYH << 8 | gyrYL) * gyrRange) / 32768.0);
//        float gyrZ = (float) (((gyrZH << 8 | gyrZL) * gyrRange) / 32768.0);
//
//        sprintf(stringBuffer, "\tTemperature: %.3f C"
//                "\t\tAcc (X,Y,Z):\t%.6f\t%.6f\t%.6f G"
//                "\t\tGyr (X,Y,Z):\t%.3f\t%.3f\t%.3f g/S", temp, accX, accY,
//                accZ, gyrX, gyrY, gyrZ);
//
//    return stringBuffer;
//}
