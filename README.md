
# OBDH
On-Board Data Handling module of the FloripaSat Cubesat Project

The main goal of On Board Data Handling, a.k.a. OBDH, is manage all the cubesat information aimming to reach the mission goal.

![OBDH 3D ISO](images/obdh_v2_3d_iso.png)
![OBDH 3D Panes](images/obdh_v2_3d_panes.png)

## Hardware

* The MCU used in this module is the [MSP430F6659](http://www.ti.com/product/MSP430F6659);
* The tracking IC used is [MPU9250](https://www.invensense.com/products/motion-tracking/9-axis/mpu-9250/), which contains a 9-axis sensors (3-axis each): a gyroscope, an accelerometer and a compass;
* For space-ground communications, is used the radio transceiver [CC1175](http://www.ti.com/product/CC1175);
* Detailed information can be found [here](https://github.com/floripasat/obdh/hardware/).

## Software

* The programming language is C;
* All software was developed in the [Code Composer Studio IDE](http://www.ti.com/tool/ccstudio) (version 6.1.3.00034), a.k.a. CCS;
* Uses FreeRTOS, a RTOS for embedded systems. [FreeRTOS page](http://freertos.org/);
* Detailed information can be found [here](https://github.com/floripasat/obdh/firmware/).
<!-- * Doxygen documentation [here](http://fsat-server.duckdns.org:8000/obdh/). nao foi feito ainda -->


## References

The general documentation of the project can be found in [GitBook](https://www.gitbook.com/book/tuliogomesp/floripasat-technical-manual).

All source code of the project can be found in [GitHub](https://github.com/floripasat).

The official website of the project is [www.floripasat.ufsc.br](http://www.floripasat.ufsc.br/).

