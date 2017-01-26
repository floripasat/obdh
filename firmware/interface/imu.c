/*
 * imu.c
 *
 *  Created on: 26 de mai de 2016
 *      Author: mario
 */

#include <interface/imu.h>

uint8_t pucImuTmpData[40];


void imu_setup(void){
    i2c2_setup();
    //    I2C1_SDA_DIR &= I2C1_SDA_PIN
//    P8SEL |= BIT5 + BIT6;

	//imu_i2c_write(MPU9150_PWR_MGMT_1, 0x00);
    pucImuTmpData[0] = MPU9150_PWR_MGMT_1;   //reg address
    pucImuTmpData[1] = 0x00;                 //data

    vI2cSetMode(IMU_BASE_ADDRESS, TRANSMIT_MODE);
    vI2cSetSlave(IMU_BASE_ADDRESS, MPU0_I2C_SLAVE_ADRESS);
    vI2cSend(IMU_BASE_ADDRESS, pucImuTmpData[0], NO_STOP);
    vI2cSend(IMU_BASE_ADDRESS, pucImuTmpData[1], NO_START);

    pucImuTmpData[0] = MPU9150_ACCEL_CONFIG;
	if (IMU_ACC_RANGE == 2.0) {
	    pucImuTmpData[1] = 0x00;   // config for +-2g range
	} else {
	    pucImuTmpData[1] = 0x18;   // config for +-16g range
	}

	vI2cSend(IMU_BASE_ADDRESS, pucImuTmpData[0], NO_STOP);
    vI2cSend(IMU_BASE_ADDRESS, pucImuTmpData[1], NO_START);

    volatile uint8_t ucWhoAmI = 0;
    vI2cSend(IMU_BASE_ADDRESS, MPU9150_WHO_AM_I, NO_STOP);
    vI2cSetMode(IMU_BASE_ADDRESS, RECEIVE_MODE);
    ucWhoAmI = vI2cReceive(IMU_BASE_ADDRESS, START_STOP);

//    while(ucWhoAmI != 0x68); // TRAVAR O CODIGO AQUI CASO A IDENTIFICACAO DA IMU DE ERRADO

}


void imu_read(uint8_t *pucImuData, uint8_t ucImuSelect)
{
    volatile uint8_t ucSlaveAddress = 0x00;
    volatile imuData;
    switch(ucImuSelect)
    {
        case IMU1:
            ucSlaveAddress = MPU0_I2C_SLAVE_ADRESS;
            break;
        case IMU2:
            ucSlaveAddress = MPU1_I2C_SLAVE_ADRESS;
            break;
    }

//    vI2cSetSlave( IMU_BASE_ADDRESS, ucSlaveAddress);
    vI2cSetMode(IMU_BASE_ADDRESS, TRANSMIT_MODE);
    vI2cSend(IMU_BASE_ADDRESS, MPU9150_ACCEL_XOUT_H, NO_STOP);
    vI2cSetMode(IMU_BASE_ADDRESS, RECEIVE_MODE);

    pucImuData[0] = vI2cReceive(IMU_BASE_ADDRESS, NO_STOP);

    vI2cReceiveBurst(IMU_BASE_ADDRESS, pucImuData+1,12);

    pucImuData[13] = vI2cReceive(IMU_BASE_ADDRESS, NO_START);
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
