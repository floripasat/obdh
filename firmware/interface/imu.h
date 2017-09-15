/*
 * imu.h
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
 * \file imu.h
 *
 * \brief This file deals with the IMU module
 *
 * \author Mario Baldini
 *
 */

#ifndef INTERFACE_IMU_H_
#define INTERFACE_IMU_H_

#include "../driver/i2c.h"
#include "../hal/obdh_hal.h"
#include "../include/floripasat_def.h"
#include "../util/misc.h"

#define MPU9150_SELF_TEST_X        0x0D   /**< Read/Write */
#define MPU9150_SELF_TEST_Y        0x0E   /**< Read/Write */
#define MPU9150_SELF_TEST_Z        0x0F   /**< Read/Write */
#define MPU9150_SELF_TEST_A        0x10   /**< Read/Write */
#define MPU9150_SMPLRT_DIV         0x19   /**< Read/Write */
#define MPU9150_CONFIG             0x1A   /**< Read/Write */
#define MPU9150_GYRO_CONFIG        0x1B   /**< Read/Write */
#define MPU9150_ACCEL_CONFIG       0x1C   /**< Read/Write */
#define MPU9150_FF_THR             0x1D   /**< Read/Write */
#define MPU9150_FF_DUR             0x1E   /**< Read/Write */
#define MPU9150_MOT_THR            0x1F   /**< Read/Write */
#define MPU9150_MOT_DUR            0x20   /**< Read/Write */
#define MPU9150_ZRMOT_THR          0x21   /**< Read/Write */
#define MPU9150_ZRMOT_DUR          0x22   /**< Read/Write */
#define MPU9150_FIFO_EN            0x23   /**< Read/Write */
#define MPU9150_I2C_MST_CTRL       0x24   /**< Read/Write */
#define MPU9150_I2C_SLV0_ADDR      0x25   /**< Read/Write */
#define MPU9150_I2C_SLV0_REG       0x26   /**< Read/Write */
#define MPU9150_I2C_SLV0_CTRL      0x27   /**< Read/Write */
#define MPU9150_I2C_SLV1_ADDR      0x28   /**< Read/Write */
#define MPU9150_I2C_SLV1_REG       0x29   /**< Read/Write */
#define MPU9150_I2C_SLV1_CTRL      0x2A   /**< Read/Write */
#define MPU9150_I2C_SLV2_ADDR      0x2B   /**< Read/Write */
#define MPU9150_I2C_SLV2_REG       0x2C   /**< Read/Write */
#define MPU9150_I2C_SLV2_CTRL      0x2D   /**< Read/Write */
#define MPU9150_I2C_SLV3_ADDR      0x2E   /**< Read/Write */
#define MPU9150_I2C_SLV3_REG       0x2F   /**< Read/Write */
#define MPU9150_I2C_SLV3_CTRL      0x30   /**< Read/Write */
#define MPU9150_I2C_SLV4_ADDR      0x31   /**< Read/Write */
#define MPU9150_I2C_SLV4_REG       0x32   /**< Read/Write */
#define MPU9150_I2C_SLV4_DO        0x33   /**< Read/Write */
#define MPU9150_I2C_SLV4_CTRL      0x34   /**< Read/Write */
#define MPU9150_I2C_SLV4_DI        0x35   /**< Read       */
#define MPU9150_I2C_MST_STATUS     0x36   /**< Read       */
#define MPU9150_INT_PIN_CFG        0x37   /**< Read/Write */
#define MPU9150_INT_ENABLE         0x38   /**< Read/Write */
#define MPU9150_INT_STATUS         0x3A   /**< Read       */
#define MPU9150_ACCEL_XOUT_H       0x3B   /**< Read       */
#define MPU9150_ACCEL_XOUT_L       0x3C   /**< Read       */
#define MPU9150_ACCEL_YOUT_H       0x3D   /**< Read       */
#define MPU9150_ACCEL_YOUT_L       0x3E   /**< Read       */
#define MPU9150_ACCEL_ZOUT_H       0x3F   /**< Read       */
#define MPU9150_ACCEL_ZOUT_L       0x40   /**< Read       */
#define MPU9150_TEMP_OUT_H         0x41   /**< Read       */
#define MPU9150_TEMP_OUT_L         0x42   /**< Read       */
#define MPU9150_GYRO_XOUT_H        0x43   /**< Read       */
#define MPU9150_GYRO_XOUT_L        0x44   /**< Read       */
#define MPU9150_GYRO_YOUT_H        0x45   /**< Read       */
#define MPU9150_GYRO_YOUT_L        0x46   /**< Read       */
#define MPU9150_GYRO_ZOUT_H        0x47   /**< Read       */
#define MPU9150_GYRO_ZOUT_L        0x48   /**< Read       */
#define MPU9150_EXT_SENS_DATA_00   0x49   /**< Read       */
#define MPU9150_EXT_SENS_DATA_01   0x4A   /**< Read       */
#define MPU9150_EXT_SENS_DATA_02   0x4B   /**< Read       */
#define MPU9150_EXT_SENS_DATA_03   0x4C   /**< Read       */
#define MPU9150_EXT_SENS_DATA_04   0x4D   /**< Read       */
#define MPU9150_EXT_SENS_DATA_05   0x4E   /**< Read       */
#define MPU9150_EXT_SENS_DATA_06   0x4F   /**< Read       */
#define MPU9150_EXT_SENS_DATA_07   0x50   /**< Read       */
#define MPU9150_EXT_SENS_DATA_08   0x51   /**< Read       */
#define MPU9150_EXT_SENS_DATA_09   0x52   /**< Read       */
#define MPU9150_EXT_SENS_DATA_10   0x53   /**< Read       */
#define MPU9150_EXT_SENS_DATA_11   0x54   /**< Read       */
#define MPU9150_EXT_SENS_DATA_12   0x55   /**< Read       */
#define MPU9150_EXT_SENS_DATA_13   0x56   /**< Read       */
#define MPU9150_EXT_SENS_DATA_14   0x57   /**< Read       */
#define MPU9150_EXT_SENS_DATA_15   0x58   /**< Read       */
#define MPU9150_EXT_SENS_DATA_16   0x59   /**< Read       */
#define MPU9150_EXT_SENS_DATA_17   0x5A   /**< Read       */
#define MPU9150_EXT_SENS_DATA_18   0x5B   /**< Read       */
#define MPU9150_EXT_SENS_DATA_19   0x5C   /**< Read       */
#define MPU9150_EXT_SENS_DATA_20   0x5D   /**< Read       */
#define MPU9150_EXT_SENS_DATA_21   0x5E   /**< Read       */
#define MPU9150_EXT_SENS_DATA_22   0x5F   /**< Read       */
#define MPU9150_EXT_SENS_DATA_23   0x60   /**< Read       */
#define MPU9150_MOT_DETECT_STATUS  0x61   /**< Read       */
#define MPU9150_I2C_SLV0_DO        0x63   /**< Read/Write */
#define MPU9150_I2C_SLV1_DO        0x64   /**< Read/Write */
#define MPU9150_I2C_SLV2_DO        0x65   /**< Read/Write */
#define MPU9150_I2C_SLV3_DO        0x66   /**< Read/Write */
#define MPU9150_I2C_MST_DELAY_CTRL 0x67   /**< Read/Write */
#define MPU9150_SIGNAL_PATH_RESET  0x68   /**< Read/Write */
#define MPU9150_MOT_DETECT_CTRL    0x69   /**< Read/Write */
#define MPU9150_USER_CTRL          0x6A   /**< Read/Write */
#define MPU9150_PWR_MGMT_1         0x6B   /**< Read/Write */
#define MPU9150_PWR_MGMT_2         0x6C   /**< Read/Write */
#define MPU9150_FIFO_COUNTH        0x72   /**< Read/Write */
#define MPU9150_FIFO_COUNTL        0x73   /**< Read/Write */
#define MPU9150_FIFO_R_W           0x74   /**< Read/Write */
#define MPU9150_WHO_AM_I           0x75   /**< Read       */

//MPU9150 Compass
#define MPU9150_CMPS_XOUT_L        0x4A   /**< Read       */
#define MPU9150_CMPS_XOUT_H        0x4B   /**< Read       */
#define MPU9150_CMPS_YOUT_L        0x4C   /**< Read       */
#define MPU9150_CMPS_YOUT_H        0x4D   /**< Read       */
#define MPU9150_CMPS_ZOUT_L        0x4E   /**< Read       */
#define MPU9150_CMPS_ZOUT_H        0x4F   /**< Read       */


#define IMU_ACC_RANGE       16.0          /**< Accelerometer resolution */
#define IMU_GYR_RANGE       250.0         /**< Gyroscope resolution     */
#define IMU_TEMP_RANGE      500.0         /**< Temperature resolution   */
#define ROOM_TEMP_OFFSET    0.0

#define IMU_NOT_WORKING     0
#define IMU_WORKING         1

#define IMU_WHO_AM_I_VALUE  0x71          /**< Default factory value    */

/**
 * \fn imu_setup
 *
 * \brief Power on the IMU, configure the resolution of accelerometer and
 * gyroscope and verify if the communication is working.
 * \return if communication with IMU is working or not
 */
uint8_t imu_setup(void);

/**
 * \fn imu_read
 *
 * \brief Read the 3-axis accelerometer and 3-axis gyroscope, each one with 12-bit resolution.
 * \return if communication with IMU is working or not
 */
uint8_t imu_read(uint8_t *p_imu_data, uint8_t imu_select);


/**
 * Macros to convert raw value into understandable values
 */
#define imu_acc_raw_to_g(H, L)       (float)  (((H << 8 | L) * IMU_ACC_RANGE) / 32768.0)
#define imu_gyr_raw_to_dps(H, L)     (float)  (((H << 8 | L) * IMU_GYR_RANGE) / 32768.0)
#define imu_temp_raw_to_degrees(H, L)(float) ((((H << 8 | L) - ROOM_TEMP_OFFSET) / IMU_TEMP_RANGE) + 21)



#endif /* INTERFACE_IMU_H_ */
