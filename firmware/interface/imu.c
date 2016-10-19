/*
 * imu.c
 *
 *  Created on: 26 de mai de 2016
 *      Author: mario
 */

#include <interface/imu.h>

uint8_t pucImuTmpData[40];


void vImuConfig(void){
	//imu_i2c_write(MPU9150_PWR_MGMT_1, 0x00);
    pucImuTmpData[0] = MPU9150_PWR_MGMT_1;   //reg address
    pucImuTmpData[1] = 0x00;                 //data

    //device, data address, bytes
    vI2cWrite(MPU1_I2C_SLAVE_ADRESS, pucImuTmpData, 1);


//	if (IMU_ACC_RANGE == 2.0) {
//		imu_i2c_write(MPU9150_ACCEL_CONFIG, 0x00);   // config for +-2g range
//	} else {
//		imu_i2c_write(MPU9150_ACCEL_CONFIG, 0x18);   // config for +-16g range
//	}

}




void vImuRead(uint8_t *pucImuData, uint8_t ucImuSelect)
{
    volatile uint8_t ucSlaveAddress = 0x00;
    switch(ucImuSelect)
    {
        case IMU1:
            ucSlaveAddress = MPU1_I2C_SLAVE_ADRESS;
            break;
        case IMU2:
            ucSlaveAddress = MPU2_I2C_SLAVE_ADRESS;
            break;
    }

    vI2cSetSlave( IMU_BASE_ADDRESS, ucSlaveAddress);

    vI2cRead(IMU_BASE_ADDRESS, MPU9150_ACCEL_XOUT_H, pucImuData, IMU_DATA_LENGTH);
}


void imu_read(char* imuData){
    //     Clear frame memory space
    int i;
    for (i = 0; i < IMU_DATA_LENGTH; i++) {
        imuData[i] = 0x00;
    }
	imu_i2c_read(MPU9150_ACCEL_XOUT_H, imuData, IMU_DATA_LENGTH );
	__delay_cycles(DELAY_5_MS_IN_CYCLES);
}


char* imu_data2string(char* stringBuffer, char* imuData, float accRange,
        float gyrRange) {



        //	IMU MPU6050 response:
        //	[accXH][accXL][accYH][accYL][accZH][accZL][tempH][tempL][gyrXH][gyrXL][gyrYH][gyrYL][gyrZH][gyrZL]

        char accXH = imuData[0];
        char accXL = imuData[1];
        char accYH = imuData[2];
        char accYL = imuData[3];
        char accZH = imuData[4];
        char accZL = imuData[5];

        char tempH = imuData[6];
        char tempL = imuData[7];

        char gyrXH = imuData[8];
        char gyrXL = imuData[9];
        char gyrYH = imuData[10];
        char gyrYL = imuData[11];
        char gyrZH = imuData[12];
        char gyrZL = imuData[13];

        float temp = (float) (((tempH << 8) | tempL) / 340.0 + 35.0);

        float accX = (float) (((accXH << 8 | accXL) * accRange) / 32768.0);
        float accY = (float) (((accYH << 8 | accYL) * accRange) / 32768.0);
        float accZ = (float) (((accZH << 8 | accZL) * accRange) / 32768.0);

        float gyrX = (float) (((gyrXH << 8 | gyrXL) * gyrRange) / 32768.0);
        float gyrY = (float) (((gyrYH << 8 | gyrYL) * gyrRange) / 32768.0);
        float gyrZ = (float) (((gyrZH << 8 | gyrZL) * gyrRange) / 32768.0);

        sprintf(stringBuffer, "\tTemperature: %.3f C"
                "\t\tAcc (X,Y,Z):\t%.6f\t%.6f\t%.6f G"
                "\t\tGyr (X,Y,Z):\t%.3f\t%.3f\t%.3f g/S", temp, accX, accY,
                accZ, gyrX, gyrY, gyrZ);



    return stringBuffer;
}


void imu_i2c_write(unsigned char reg_adrr, unsigned char data) {
	unsigned char TxData[] = { reg_adrr, data };
	PTxData = (unsigned char *) TxData;
	TXByteCtr = sizeof TxData;
	UCB1CTL1 |= UCTR + UCTXSTT;
	while (UCB1CTL1 & UCTXSTP);
}

void imu_i2c_read(unsigned char reg_adrr, char buffer[],unsigned int bytes) {
	PTxData = &reg_adrr;
	TXByteCtr = 1;
	UCB1CTL1 |= UCTR + UCTXSTT;
	while (UCB1CTL1 & UCTXSTP);
	RXByteCtr = bytes;
	PRxData = buffer;
	UCB1CTL1 &= ~UCTR;
	UCB1CTL1 |= UCTXSTT;
	while (UCB1CTL1 & UCTXSTP);
}
