# Packet Description

Esse documento deve seguir as informações contidas em: https://docs.google.com/spreadsheets/d/1e5KTyQqeFW5pgzqara0UhxRcPHhF5Oq5QgL1BtvwexI

## Package flags

### Length (bytes)
2

### Description

Contém 1 bit flag para cada um dos submódulos seguintes do pacote. Se o bit estiver setado, significa que o pacote contém dados daquele determinado módulo. Durante a persistência de dados em memória não-volátil, uma flag zerada significa que os dados do submódulo referido serão armazenados com o valor 0 e podem ser ignorados no pacote, porém mantendo o pacote de tamanho fixo. Já durante um envio de dados para o segmento terrestre, uma flag zerada significa que o pacote não contém aquela seção de dados referente ao submódulo, fazendo com que o tamanho total do pacote seja menor.


## System status

### Length (bytes)
5

### Description

Contém um contador de resets (3 bytes) e flags que sinalizam quais submódulos estão funcionando (2 bytes). 


## IMU

### Length (bytes)
20

### Description
Inercial Measure Unit data.

3-axis accelerometer - 6 bytes
<p><strong>Cálculo:</strong>   (((HIGH_BYTE << 8 | LOW_BYTE) * IMU_ACC_RANGE) / 32768.0) , onde IMU_ACC_RANGE depende da sensibilidade configurada (2g a 16g).</p>

3-axis gyroscope - 6 bytes
<p><strong>Cálculo:</strong>   (((HIGH_BYTE << 8 | LOW_BYTE) * IMU_GYRO_RANGE) / 32768.0) , onde IMU_GYRO_RANGE depende da sensibilidade configurada (250º/s a 2000º/s).</p>

3-axis magnetometer - 6 bytes

temperature - 2 bytes

<p><strong>Cálculo:</strong>   (((HIGH_BYTE << 8 | LOW_BYTE) / IMU_TEMP_RANGE ) + 21), onde IMU_TEMP_RANGE = 500.</p>


## MSP Sensors

### Length (bytes)
6
### Description

MSP internal temperature sensor (2 bytes)

<p><strong>Cálculo:</strong>   ((HIGH_BYTE << 8 | LOW_BYTE) * 2 - COEF_30C) * 55) / (COEF_85C - COEF_30C) + 30, onde COEF_XX é o coeficiente de calibraçao salvo na memoria flash do MSP.</p>

Supply voltage (2 bytes)

<p><strong>Cálculo:</strong>    (HIGH_BYTE << 8 | LOW_BYTE) * AVCC * VOLTAGE_DIVISOR / ADC_RANGE, onde AVCC = 3.3, VOLTAGE_DIVISOR = 2, ADC_RANGE = 3.</p>

Supply current (2 bytes)
<p><strong>Cálculo:</strong>    (HIGH_BYTE << 8 | LOW_BYTE) * (AVCC / (ADC_RANGE * RL_VALUE * CURRENT_GAIN * RSENSE_VALUE), onde AVCC = 3.3, ADC_RANGE = 3, RL_VALUE = 0.05, CURRENT_GAIN = 0.025 e RSENSE_VALUE = 20000.</p>


## Systick

### Length (bytes)
4

### Description
Tempo desde o último reset, em milisegundos.



## Solar Panels

### Length (bytes)
42

### Description
3 solar panels has 1-axis accelerometer, gyroscope and magnetometer (2 bytes each axis each panel). Every panels has a sun sensor and a temperature sensor (2 bytes each). (2+2+2) * 3 + (2+2) * 6 = 42 bytes.


## RTC

### Length (bytes)
8

### Description
Seconds, minutes, hours, day of week, day of month, month (1 byte each) and year (2 bytes).



## Radio

### Length (bytes)
100

### Description
RSSI do rádio (2 bytes)
Últimos comandos recebidos


## ADC Solar Panels

### Length (bytes)
18

### Description
ADC solar panel current - 12 bytes
ADC solar panel voltage - 6 bytes


## MSP430 ADC

### Length (bytes)
8

### Description
ADC total voltage - 2 bytes 
ADC bus voltage - 2 bytes
ADC beacon/eps current - 2 bytes
ADC internal temperature sensor - 2 bytes



## Battery Monitor

### Length (bytes)
21

### Description
Battery average current - 2 bytes
Battery monitor temperature - 2 bytes
Battery voltage - 4 bytes
Battery current - 2 bytes
Battery accumulated current - 2 bytes
Protection register - 1 byte
Status register - 1 byte
Cycle counter register - 1 byte
Remaining active absolute capacity register - 2 bytes
Remaining standby absolute capacity register - 2 bytes
Remaining active relative capacity - 1 byte
Remaining standby relative capacity - 1 byte


## ADS1248

### Length (bytes)
21

### Description
7 RTDs measurements via ADS1248 - 21 bytes


## Task Scheduler

### Length (bytes)
1

### Description
EPS status - 1 byte



## Beacon

### Length (bytes)
3

### Description
Radio temperature - 1 byte
Transmitted power - 1 byte
DAC voltage (power amplifier control) - 1 byte



## Transceiver

### Length (bytes)
4

### Description
Radio temperature - 1 byte
Received signal power - 1 byte
Transmitted power - 1 byte
DAC voltage (power amplifier control) - 1 byte


## Payload1

### Length (bytes)
7

### Description
Voltage on the thermoeletric generator with/without load and energy stored on the last 3 minutes (7 bytes total)

## Payload2

### Length (bytes)
100

### Description
Single Event Upset Detection
