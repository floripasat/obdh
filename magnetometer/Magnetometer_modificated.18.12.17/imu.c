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


uint8_t imu_read(uint8_t *p_imu_data)
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
    i2c_send(IMU_BASE_ADDRESS, reg, NO_STOP);           /**< send the register address              */
    i2c_send(IMU_BASE_ADDRESS, value, NO_START);
}

void imu_offset_accel(uint8_t *p_imu_data)
{
    volatile uint8_t imu_slave_address = 0x00;
    uint8_t imu_status = IMU_WORKING;

    imu_slave_address = IMU0_I2C_SLAVE_ADRESS;

    i2c_set_slave(IMU_BASE_ADDRESS, imu_slave_address); /**< set the I2C slave to be IMU */
    i2c_set_mode(IMU_BASE_ADDRESS, TRANSMIT_MODE);      /**< set to transmit mode        */

    /**< try to send a register address */
    if(i2c_send(IMU_BASE_ADDRESS, MPU9150_XA_OFFSET_H, NO_STOP) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }
    i2c_set_mode(IMU_BASE_ADDRESS, RECEIVE_MODE);       /**< change to receive mode */

    /**< try to read from the register address */
    if(i2c_receive_burst(IMU_BASE_ADDRESS, p_imu_data, 6, START_STOP) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }


    return imu_status;
}

void imu_offset_gyro(uint8_t *p_imu_data)
{
    volatile uint8_t imu_slave_address = 0x00;
    uint8_t imu_status = IMU_WORKING;

    imu_slave_address = IMU0_I2C_SLAVE_ADRESS;

    i2c_set_slave(IMU_BASE_ADDRESS, imu_slave_address); /**< set the I2C slave to be IMU */
    i2c_set_mode(IMU_BASE_ADDRESS, TRANSMIT_MODE);      /**< set to transmit mode        */

    /**< try to send a register address */
    if(i2c_send(IMU_BASE_ADDRESS, MPU9150_XG_OFFSET_H, NO_STOP) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }
    i2c_set_mode(IMU_BASE_ADDRESS, RECEIVE_MODE);       /**< change to receive mode */

    /**< try to read from the register address */
    if(i2c_receive_burst(IMU_BASE_ADDRESS, p_imu_data, 6, START_STOP) == I2C_FAIL) {
        imu_status = IMU_NOT_WORKING;
    }


    return imu_status;
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
    write_i2c(0x1B, 0x00); //Gyro_Config    0000000
    write_i2c(0x1C, 0x18); //Accel_Config   00011000
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

void read_accel_gyr(float *mag_temp2){
    uint8_t buffer1[14];
    imu_read(buffer1);
    mag_temp2[0] = imu_acc_raw_to_g(buffer1[0],buffer1[1]);  //read and calculate x axis of accelerometer
    mag_temp2[1] = imu_acc_raw_to_g(buffer1[2],buffer1[3]); //read and calculate y axis of accelerometer
    mag_temp2[2] = imu_acc_raw_to_g(buffer1[4],buffer1[5]); //read and calculate z axis of accelerometer

    mag_temp2[3] = imu_temp_raw_to_degrees(buffer1[6],buffer1[7]); //read and calculate temperature

    mag_temp2[4] = imu_gyr_raw_to_dps(buffer1[8],buffer1[9]); //read and calculate x axis of gyroscpe
    mag_temp2[5] = imu_gyr_raw_to_dps(buffer1[10],buffer1[11]); //read and calculate y axis of gyroscpe
    mag_temp2[6] = imu_gyr_raw_to_dps(buffer1[12],buffer1[13]); //read and calculate z axis of gyroscpe
}

void average(float *sen_buff_temp, uint8_t *times) {
    sen_buff_temp[0] = 0, sen_buff_temp[1] = 0, sen_buff_temp[2] = 0, sen_buff_temp[3] = 0, sen_buff_temp[4] = 0, sen_buff_temp[5] = 0, sen_buff_temp[6] = 0;
    uint8_t average_sensors = 0;

    for (average_sensors = 0; average_sensors < times; average_sensors ++) //execute n times to get average
    {
        float sensors[7];
        read_accel_gyr(sensors);
        sen_buff_temp[0] = sen_buff_temp[0] + sensors[0];
        sen_buff_temp[1] = sen_buff_temp[1] + sensors[1];
        sen_buff_temp[2] = sen_buff_temp[2] + sensors[2];

        sen_buff_temp[3] = sen_buff_temp[3] + sensors[3];

        sen_buff_temp[4] = sen_buff_temp[4] + sensors[4];
        sen_buff_temp[5] = sen_buff_temp[5] + sensors[5];
        sen_buff_temp[6] = sen_buff_temp[6] + sensors[6];
        __delay_cycles(50000); //F = 1Mhz
    }
}

void calibration_magnetometer(float *mag_max, float *mag_min, uint8_t *calibration_magnetometer_mode){
    uint8_t i = 0;
    uint8_t j;
    int16_t count = 50;
    uint8_t  asa_temp[3], ASA_X, ASA_Y, ASA_Z; //axix sensitivity adjustment value
    int16_t mag_temp[3];
    float mag_offset[3];
    uint8_t YES = 1, NO = 0;
 
    mag_setup(0); //0 for Fuse ROM mode
    mag_read_asa(asa_temp);
    ASA_X =  asa_temp[0]; //x- axix sensitivity adjustment value
    ASA_Y =  asa_temp[1]; //y- axix sensitivity adjustment value
    ASA_Z =  asa_temp[2]; //z- axix sensitivity adjustment value
 
    mag_setup(1); //1 for Fuse ROM mode for Continuous measurement mode 1
 
    if (calibration_magnetometer_mode == YES)    //Hard Iron Correction of Magnetometer
    {
        for (i=0; i< count; i++) // verification of the maximum and minimum values for calibration
        {
            read_magnetometer(mag_temp);
            mag_temp[0]= mag_temp[0] * (((ASA_X-128)*0.5/128.) + 1);
            mag_temp[1]= mag_temp[1] * (((ASA_Y-128)*0.5/128.) + 1);
            mag_temp[2]= mag_temp[2] * (((ASA_Z-128)*0.5/128.) + 1);
 
            for (j = 0; j<3; j++)
            {
                if (mag_temp[j] > mag_max[j])
                    mag_max[j]= mag_temp[j];
                if (mag_temp[j] < mag_min[j])
                    mag_min[j]= mag_temp[j];
            }
            __delay_cycles(200000); //frequencia = 1 Mhz; tempo = 100 ms; tempo total = 0.1*128= 12 s
        }
    }
     else
    {
        mag_max[0]  = 39.0;     //value measured with calibration
        mag_min[0] =  -1.0;    //value measured with calibration
        mag_max[1] =  37.0;  //value measured with calibration
        mag_min[1] = -4;     //value measured with calibration
        mag_max[2] =  24;    //value measured with calibration
        mag_min[2] = -26.0;  //value measured with calibration
    }
 
 
    mag_offset[0] = (float)((mag_max[0] + mag_min[0])/2.0); //average x mag bias in counts of magnetometer
    mag_offset[1] = (float)((mag_max[1] + mag_min[1])/2.0); //average y mag bias in counts of magnetometer
    mag_offset[2] = (float)((mag_max[2] + mag_min[2])/2.0); //average z mag bias in counts of magnetometer
 
//    flash_erase(SEGC_ADDR);               //erase if will calibrate magnetometer, accelerometer and gyroscope.
    flash_write_float (mag_offset[0], CAL_X_MAG_ADDR_FLASH);
    flash_write_float (mag_offset[1], CAL_Y_MAG_ADDR_FLASH);
    flash_write_float (mag_offset[2], CAL_Z_MAG_ADDR_FLASH);
}



void calibration_accelerometer(uint8_t *calibration_accelerometer_mode)
{
    float sensors_buffer[7];
    float Xcal[6], Ycal[6], Zcal[6], Temp, Xgyr[6], Ygyr[6], Zgyr[6];
    float x_acc_calib, y_acc_calib, z_acc_calib;
    uint8_t i = 0;
    uint8_t n = 50.0;
    float S_x, S_y, S_z;
    uint8_t YES = 1, NO = 0;
 
    if (calibration_accelerometer_mode == YES)
    {
        i = 0;
        uint8_t k = 0;
 
        for (k=0; k< 6; k++)   //calibration gyroscope and accelerometer (6 positions)
            //1nd: Front; 2nd:Back ; 3nd: LL; 4nd: LR; 5nd: PU ; 6nd: PD.
        {
            average(sensors_buffer, 50);
 
            Xcal[k] = sensors_buffer[0] /n; //average to 50 times and X current accelerometer
            Ycal[k] = sensors_buffer[1] /n; //average to 50 times
            Zcal[k] = sensors_buffer[2] /n;
         }
 
        x_acc_calib = (Xcal[0] + Xcal[1] + Xcal[2] + Xcal[3])/4;
        y_acc_calib = (Ycal[0] + Ycal[1] + Ycal[4] + Ycal[5])/4;
        z_acc_calib = (Zcal[2] + Zcal[3] + Zcal[4] + Zcal[5])/4;
        flash_write_float(x_acc_calib, CAL_X_ACCEL_ADDR_FLASH);
        flash_write_float(y_acc_calib, CAL_Y_ACCEL_ADDR_FLASH);
        flash_write_float(z_acc_calib, CAL_Z_ACCEL_ADDR_FLASH);
 
        S_x = (Xcal[4] + Xcal[5]) /2;
        S_y = (Xcal[2] + Xcal[3]) /2;
        S_z = (Xcal[0] + Xcal[1]) /2;
    }
 
}

void calibration_gyroscope(uint8_t *calibration_gyroscope_mode){
 
 
    float Xcal, Ycal, Zcal, Temp, Xgyr, Ygyr, Zgyr;
    uint8_t YES = 1, NO = 0;
    uint8_t m = 300,i= 0, j = 0, k =0, l=0;
    float  gyroscope_measures_x[500], gyroscope_measures_y[500], gyroscope_measures_z[500];
 
    float sensors[7];
    float gyr_bias_x = 0, gyr_bias_y = 0,gyr_bias_z = 0;
    float integrate_x, integrate_y, integrate_z;
 
    if (calibration_gyroscope_mode == YES)
    {
        for (i = 0; i<m; i++)
        {
            read_accel_gyr(sensors);
            gyroscope_measures_x[i] = sensors[4];
            gyroscope_measures_y[i] = sensors[5];
            gyroscope_measures_z[i] = sensors[6];
            __delay_cycles(100000); //frequencia = 1 Mhz;
        }
 
        for (j = 0; j<75; j++)   //Averaging the first 3 seconds of data
        {
            gyr_bias_x = gyr_bias_x - gyroscope_measures_x[j];
            gyr_bias_y = gyr_bias_y - gyroscope_measures_y[j];
            gyr_bias_z = gyr_bias_z - gyroscope_measures_z[j];
          }
        gyr_bias_x = gyr_bias_x/75.0 ;
        gyr_bias_y = gyr_bias_y/75.0 ;
        gyr_bias_z = gyr_bias_z/75.0 ;

        for (k = 0; k<m; i++)
               {
                   read_accel_gyr(sensors);
                   gyroscope_measures_x[i] = gyroscope_measures_x[i] - gyr_bias_x;
                   gyroscope_measures_y[i] = gyroscope_measures_y[i] - gyr_bias_y;
                   gyroscope_measures_z[i] = gyroscope_measures_z[i] - gyr_bias_z;
                   __delay_cycles(50000); //frequencia = 1 Mhz;
               }
 
        for (l = 100; l<225; l++)   //Integration of 4 to 9 seconds
        {
          integrate_x = integrate_x + gyroscope_measures_x[i];
          integrate_y = integrate_y + gyroscope_measures_y[i];
          integrate_z = integrate_z + gyroscope_measures_z[i];
        }
        integrate_x = integrate_x/125.0;
        integrate_y = integrate_y/125.0;
        integrate_z = integrate_z/125.0;
    }
}


void read_magnetometer (int16_t *mag_temp) {
    uint8_t buffer[20];
    mag_read(buffer);
    mag_temp[0]          =  imu_mag(buffer[4], buffer[3]);
    mag_temp[1]          =  imu_mag(buffer[6], buffer[5]);
    mag_temp[2]          =  imu_mag(buffer[8], buffer[7]);
}


void read_offset_accel (float *mag_temp3) {
    uint8_t buffer2[6];
    imu_offset_accel(buffer2);
    mag_temp3[0]          =  imu_acc_raw_to_g(buffer2[0], buffer2[1]);
    mag_temp3[1]          =  imu_acc_raw_to_g(buffer2[2], buffer2[3]);
    mag_temp3[2]          =  imu_acc_raw_to_g(buffer2[4], buffer2[5]);
}

void read_offset_gyro (float *mag_temp3) {
    uint8_t buffer2[6];
    imu_offset_gyro(buffer2);
    mag_temp3[0]          =  imu_gyr_raw_to_dps(buffer2[0], buffer2[1]);
    mag_temp3[1]          =  imu_gyr_raw_to_dps(buffer2[2], buffer2[3]);
    mag_temp3[2]          =  imu_gyr_raw_to_dps(buffer2[4], buffer2[5]);
}