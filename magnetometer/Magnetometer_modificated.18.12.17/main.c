#include <msp430.h>
#include "imu.h"
#include "math.h"
#include "flash.h"


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;  // stop watchdog timer
    int16_t mag_temp[3];
    uint8_t i = 0;
    float mag_max[3] = {-32767, -32767, -32767};
    float mag_min[3] = { 32767,  32767,  32767};

    float mag_x, mag_y, mag_z;
    float mag_x_calib_read, mag_y_calib_read, mag_z_calib_read;
    float sensors[7], sensors_buffer[7];


    float accel_x, accel_y, accel_z, temper;
    float accel_x_calib_read, accel_y_calib_read, accel_z_calib_read;

    float gyr_x, gyr_y, gyr_z;
    float gyr_x_calib_read, gyr_y_calib_read, gyr_z_calib_read;

    float X_ACC_CAL, Y_ACC_CAL, Z_ACC_CAL;


    uint8_t CALIBRATION_ACCELEROMETER;

    uint8_t n = 50.0;
    uint8_t YES = 1, NO = 0;
    float a= 256.096;
    float a1;

    i2c_setup(1);
    imu_setup();
    __delay_cycles(100000); //delay of 50 ms At 8 Hz ODR, new mag data is available every 125 ms

    mag_setup(1); //1 for Fuse ROM mode for Continuous measurement mode 1

        while(1)  //accelerometer and gyroscope
        {

           calibration_magnetometer(mag_max, mag_min, NO);
           calibration_accelerometer(NO);
           calibration_gyroscope(YES);


           mag_x_calib_read =  flash_read_float(CAL_X_MAG_ADDR_FLASH);
           mag_y_calib_read =  flash_read_float(CAL_Y_MAG_ADDR_FLASH);
           mag_z_calib_read =  flash_read_float(CAL_Z_MAG_ADDR_FLASH);

           accel_x_calib_read =  flash_read_float(CAL_X_ACCEL_ADDR_FLASH);
           accel_y_calib_read =  flash_read_float(CAL_Y_ACCEL_ADDR_FLASH);
           accel_z_calib_read =  flash_read_float(CAL_Z_ACCEL_ADDR_FLASH);
           gyr_x_calib_read =  flash_read_float(CAL_X_GYR_ADDR_FLASH);
           gyr_y_calib_read =  flash_read_float(CAL_X_GYR_ADDR_FLASH);
           gyr_z_calib_read =  flash_read_float(CAL_X_GYR_ADDR_FLASH);


         while(1)
         {
                 average(sensors_buffer, n);
                 read_magnetometer(mag_temp);

                 mag_x   = mag_temp[0] - mag_x_calib_read;
                 mag_y   = mag_temp[0] - mag_y_calib_read;
                 mag_z   = mag_temp[0] - mag_z_calib_read;

                 accel_x = (sensors_buffer[0] /n) - accel_x_calib_read; //average to 50 times and X current accelerometer
                 accel_y = (sensors_buffer[1] /n) - accel_y_calib_read; //average to 50 times
                 accel_z = (sensors_buffer[2] /n) - accel_z_calib_read;
                 temper  = (sensors_buffer[3] /n);  //average to 50 times
                 gyr_x   = (sensors_buffer[4] /n);   //average to 50 times
                 gyr_y   = (sensors_buffer[5] /n);  //average to 50 times
                 gyr_z   = (sensors_buffer[6] /n);   //average to 50 times

//               read_offset_accel(Off_accel);
    //             accel_x = sensors[0] - X_ACC_CAL;
               //  accel_y = sensors[1] - Y_ACC_CAL;
             //    accel_z = sensors[2] - Z_ACC_CAL;

                 __delay_cycles(50000); //F = 1Mhz
  }

        /*     while(1)  //magnetometer
        {
            sum_x = 0.0;
            sum_y = 0.0;
            sum_z = 0.0;
            for (j=0; j< measures_count; j++)  //get the the average of 50 measures
            {
                read_magnetometer(mag_temp);

              mag_temp2[0]= mag_temp[0] * (((ASA_X-128)*0.5/128.) + 1);
               mag_temp2[1]= mag_temp[1] * (((ASA_Y-128)*0.5/128.) + 1); //multiplication axe y by coefficient
               mag_temp2[2]= mag_temp[2] * (((ASA_Z-128)*0.5/128.) + 1); //multiplication axe z by coefficient


                mag_x_calibrated[j] = (mag_temp[0] - mag_offset[0]);  //calcule x calibrated
                sum_x = sum_x + mag_x_calibrated[j];                   // calculate of axe x

                mag_y_calibrated[j] = (mag_temp[1] - mag_offset[1]);  //calcule y calibrated
                sum_y = sum_y + mag_y_calibrated[j];                   // calculate sum of axe y

                mag_z_calibrated[j] = (mag_temp[2] - mag_offset[2]);  //calcule z calibrated
                sum_z = sum_z + mag_z_calibrated[j];                   //calculate sum of axe z
            }

            mag_x_average = sum_x/50.0;
            mag_x_calibrated_average = mag_x_average * (((ASA_X-128)*0.5/128.) + 1); //multiplication axis x by coefficient
            mag_y_average = sum_y/50.0;
            mag_y_calibrated_average= mag_y_average * (((ASA_Y-128)*0.5/128.) + 1); //multiplication axis y by coefficient
            mag_z_average = sum_z/50.0;
            mag_z_calibrated_average= mag_z_average * (((ASA_Z-128)*0.5/128.) + 1); //multiplication axis z by coefficient

            mag_root= sqrt(mag_x_average * mag_x_average + mag_y_average * mag_y_average + mag_z_average * mag_z_average);

         }

         */

        }
        return 0;
}
 