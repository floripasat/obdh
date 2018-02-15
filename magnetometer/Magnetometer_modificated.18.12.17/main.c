#include <msp430.h>
#include "imu.h"
#include "math.h"


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    int16_t mag_temp[3];
    uint8_t  asa_temp[3], ASA_X, ASA_Y, ASA_Z;

    uint8_t j;
    float mag_max[3] = {-32767, -32767, -32767};
    float mag_min[3] = { 32767,  32767,  32767};
    float mag_offset[3];

    uint8_t i = 0;
    uint8_t measures_count = 50;
    float sum_x, sum_y, sum_z;
    float mag_x_calibrated[50], mag_y_calibrated[50], mag_z_calibrated[50];
    float mag_x_average, mag_y_average,mag_z_average;
    float mag_x_calibrated_average, mag_y_calibrated_average, mag_z_calibrated_average;
    float mag_root;



    i2c_setup(1);
    imu_setup();
    __delay_cycles(100000); //delay of 50 ms At 8 Hz ODR, new mag data is available every 125 ms
    mag_setup(0); //0 for Fuse ROM mode
    mag_read_asa(asa_temp);
    ASA_X =  asa_temp[0]; //x- axix sensitivity adjustment value
    ASA_Y =  asa_temp[1]; //y- axix sensitivity adjustment value
    ASA_Z =  asa_temp[2]; //z- axix sensitivity adjustment value

    mag_setup(1); //1 for Fuse ROM mode for Continuous measurement mode 1

//

//    for (i=0; i< count; i++) // verification of the maximum and minimum values for calibration
//    {
//
//       read_magnetometer(mag_temp);
//        mag_temp[0]= mag_temp[0] * (((ASA_X-128)*0.5/128.) + 1);
//        mag_temp[1]= mag_temp[1] * (((ASA_Y-128)*0.5/128.) + 1);
//        mag_temp[2]= mag_temp[2] * (((ASA_Z-128)*0.5/128.) + 1);
//
//        for (j = 0; j<3; j++)
//        {
//            if (mag_temp[j] > mag_max[j])
//                mag_max[j]= mag_temp[j];
//            if (mag_temp[j] < mag_min[j])
//                mag_min[j]= mag_temp[j];
//        }
//        __delay_cycles(200000); //frequencia = 1 Mhz; tempo = 100 ms; tempo total = 0.1*128= 12 s
//
//    }

    //Hard Iron Correction
        mag_max[0] = 39;     //value measured with calibration
        mag_min[0] =  -1;    //value measured with calibration
        mag_max[1] =  37.0;  //value measured with calibration
        mag_min[1] = -4;     //value measured with calibration
        mag_max[2] =  24;    //value measured with calibration
        mag_min[2] = -26.0;  //value measured with calibration


        mag_offset[0] = (float)((mag_max[0] + mag_min[0])/2.); //average x mag bias in counts
        mag_offset[1] = (float)((mag_max[1] + mag_min[1])/2.); //average y mag bias in counts
        mag_offset[2] = (float)((mag_max[2] + mag_min[2])/2.); //average z mag bias in counts

        while(1)
        {
            sum_x = 0.0;
            sum_y = 0.0;
            sum_z = 0.0;
            for (j=0; j< measures_count; j++)  //get the the average of 50 measures
            {
                read_magnetometer(mag_temp);

/*             mag_temp2[0]= mag_temp[0] * (((ASA_X-128)*0.5/128.) + 1);
               mag_temp2[1]= mag_temp[1] * (((ASA_Y-128)*0.5/128.) + 1); //multiplication axe y by coefficient
               mag_temp2[2]= mag_temp[2] * (((ASA_Z-128)*0.5/128.) + 1); //multiplication axe z by coefficient
*/

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


        return 0;
}
