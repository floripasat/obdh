# FloripaSat - On Board Data Handling - Software

## FreeRTOS

* Language: C
* The MCU used in this module is the [MSP430F6659](http://www.ti.com/product/MSP430F6659).
* As transmitter, the [CC1175](http://www.ti.com/product/CC1175) IC is used.
* All MCU peripherals are controlled with the ["MSP Driver Library"](http://www.ti.com/tool/mspdriverlib) library, also know as ["DriverLib"](http://www.ti.com/tool/mspdriverlib).
* All software was developed in the [Code Composer Studio IDE](http://www.ti.com/tool/ccstudio) (version 6.1.3.00034), a.k.a. CCS.


## Software flow

1. Antenna Deployment Countdown 
2. Antenna Deployment
3. Watchdog configuration
4. Peripherals initialization 
5. Start Safe state tasks
6. Start Atittude tasks
7. Start Nominal tasks


## USCI Configuration

| MSP INTERFACE | MODE | CONNECTED COMPONENTS |
|:------|:----------|:--------------------------------------------|
| USCI_A0 | SPI | TRANSCEIVER |
| USCI_A1 | SPI | MAIN BUS (flash/SD memory, ADC1, ADC2, ADC3, ADCS) |
| USCI_A2 | UART | DEBUG |
| USCI_B0 | I2C | EPS (EPS as slave) |
| USCI_B1 | I2C | MAIN BUS (IMU1, IMU2) |
| USCI_B2 | I2C | ANTENNA DEPLOYMENT |


## Debug mode

The Debug mode was used to print log messages.

The debug mode can be used with a UART-USB converter (FTDI chip for example) with the follow configuration:
* Baudrate = 9600 bps
* Data bits = 8
* Parity bit = None
* Stop bits = 1


## References

The general documentation of the project can be found in [GitBook](https://www.gitbook.com/book/tuliogomesp/floripasat-technical-manual).

All source code of the project can be found in [GitHub](https://github.com/floripasat/).

The official website of the project is [www.floripasat.ufsc.br](http://www.floripasat.ufsc.br/).
