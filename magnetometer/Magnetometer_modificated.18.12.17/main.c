#include <msp430.h>
#include "imu.h"
#include "math.h"
#include "flash.h"

#define CAL_X_ACCEL_ADDR_FLASH   (BANK3_ADDR)
#define CAL_Y_ACCEL_ADDR_FLASH   (BANK3_ADDR + 4)
#define CAL_Z_ACCEL_ADDR_FLASH   (BANK3_ADDR + 8)

#define CAL_X_GYR_ADDR_FLASH     (BANK3_ADDR + 12)
#define CAL_Y_GYR_ADDR_FLASH     (BANK3_ADDR + 16)
#define CAL_Z_GYR_ADDR_FLASH     (BANK3_ADDR + 20)

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    int16_t mag_temp[3];
    uint8_t i = 0;
    float mag_max[3] = {-32767, -32767, -32767};
    float mag_min[3] = { 32767,  32767,  32767};
    float mag_offset[3];
    float sum_x, sum_y, sum_z;
    float mag_x_calibrated[50], mag_y_calibrated[50], mag_z_calibrated[50];
    float mag_x_average, mag_y_average,mag_z_average;
    float mag_x_calibrated_average, mag_y_calibrated_average, mag_z_calibrated_average;
    float mag_root;
    uint8_t measures_count = 50;

    float sensors[7], sensors_buffer[7];
    float gyr_x, gyr_y, gyr_z;
    float accel_x, accel_y, accel_z, temper;
 //   uint8_t average_sensors= 0;
    float Xcal[6], Ycal[6], Zcal[6], Temp, Xgyr[6], Ygyr[6], Zgyr[6];
    float X_ACC_CAL, Y_ACC_CAL, Z_ACC_CAL;
    float X_ACC_CAL_READ, Y_ACC_CAL_READ, Z_ACC_CAL_READ;
    float S_x, S_y, S_z;
    float Off_accel[3];
    uint8_t n = 50.0;
    uint8_t YES = 1, NO = 0;
    uint8_t CALIBRATION_ACCELEROMETER;

    i2c_setup(1);
    imu_setup();
    __delay_cycles(100000); //delay of 50 ms At 8 Hz ODR, new mag data is available every 125 ms

    mag_setup(1); //1 for Fuse ROM mode for Continuous measurement mode 1

//    calibration(mag_max, mag_min); //Calibration magnetometer

    //Hard Iron Correction of Magnetometer
       mag_max[0]  = 39;     //value measured with calibration
        mag_min[0] =  -1;    //value measured with calibration
        mag_max[1] =  37.0;  //value measured with calibration
        mag_min[1] = -4;     //value measured with calibration
        mag_max[2] =  24;    //value measured with calibration
        mag_min[2] = -26.0;  //value measured with calibration

        mag_offset[0] = (float)((mag_max[0] + mag_min[0])/2.); //average x mag bias in counts of magnetometer
        mag_offset[1] = (float)((mag_max[1] + mag_min[1])/2.); //average y mag bias in counts of magnetometer
        mag_offset[2] = (float)((mag_max[2] + mag_min[2])/2.); //average z mag bias in counts of magnetometer

        read_magnetometer(mag_temp);
        mag_temp[0] = mag_temp[0] - mag_offset[0];  //magnetometer calibrated
        mag_temp[1] = mag_temp[1] - mag_offset[1];  //magnetometer calibrated
        mag_temp[2] = mag_temp[2] - mag_offset[2];  //magnetometer calibrated

        while(1)  //accelerometer and gyroscope
        {
           CALIBRATION_ACCELEROMETER= NO;
           if (CALIBRATION_ACCELEROMETER == YES)
           {
               i = 0;
               uint8_t k = 0;

               for (k=0; k< 6; k++)   //calibration gyroscope and accelerometer (6 positions)
                   //1nd: Front; 2nd:Back ; 3nd: LL; 4nd: LR; 5nd: PU ; 6nd: PD.
               {
                   average(sensors_buffer, n);

                   Xcal[k] = sensors_buffer[0] /n; //average to 50 times and X current accelerometer
                   Ycal[k] = sensors_buffer[1] /n; //average to 50 times
                   Zcal[k] = sensors_buffer[2] /n;
                   Temp    = sensors_buffer[3] /n;  //average to 50 times
                   Xgyr[k] = sensors_buffer[4] /n;   //average to 50 times
                   Ygyr[k] = sensors_buffer[5] /n;  //average to 50 times
                   Zgyr[k] = sensors_buffer[6] /n;   //average to 50 times
               }

               X_ACC_CAL = (Xcal[0] + Xcal[1] + Xcal[2] + Xcal[3])/4;
               Y_ACC_CAL = (Ycal[0] + Ycal[1] + Ycal[4] + Ycal[5])/4;
               Z_ACC_CAL = (Zcal[2] + Zcal[3] + Zcal[4] + Zcal[5])/4;
               flash_write_float(X_ACC_CAL, CAL_X_ACCEL_ADDR_FLASH);
               flash_write_float(Y_ACC_CAL, CAL_Y_ACCEL_ADDR_FLASH);
               flash_write_float(Z_ACC_CAL, CAL_Z_ACCEL_ADDR_FLASH);

               S_x = (Xcal[4] + Xcal[5]) /2;
               S_y = (Xcal[2] + Xcal[3]) /2;
               S_z = (Xcal[0] + Xcal[1]) /2;
           }


           X_ACC_CAL_READ =  flash_read_float(CAL_X_ACCEL_ADDR_FLASH);
           Y_ACC_CAL_READ =  flash_read_float(CAL_Y_ACCEL_ADDR_FLASH);
           Z_ACC_CAL_READ =  flash_read_float(CAL_Z_ACCEL_ADDR_FLASH);


         while(1)
         {
                 average(sensors_buffer, n);

                 accel_x = (sensors_buffer[0] /n) - X_ACC_CAL_READ; //average to 50 times and X current accelerometer
                 accel_y = (sensors_buffer[1] /n) - Y_ACC_CAL_READ; //average to 50 times
                 accel_z = (sensors_buffer[2] /n) - Z_ACC_CAL_READ;
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
