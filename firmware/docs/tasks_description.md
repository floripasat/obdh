# Tasks Description

## wdt_task

#### Descrição

Esta tarefa irá resetar os dois watchdog timers (interno e externo) a cada execução. No modo DEBUG ela também faz o system_led piscar para termos um recurso visual no momento da depuração.

#### Dados gerados

Nenhum

#### Período da tarefa

500 ms.

#### Prioridade

5

#### Recursos utilizados

GPIO WDI_EXT;
GPIO LED_SYSTEM.


## store_data_task

#### Descrição

Esta tarefa recebe, por meio de filas de mensagens, os dados de outras tarefas, os quais deve salvar em uma memória não-volátil. Os dados são salvos em blocos de 512 bytes. Os dados são armazenados conforme a tabela: https://docs.google.com/spreadsheets/d/1e5KTyQqeFW5pgzqara0UhxRcPHhF5Oq5QgL1BtvwexI. 

O esquema de armazenamento dá-se da seguinte forma: se houver mensagens (novos dados) na fila de mensagens referente a determinado submódulo, sua respectiva flag é setada no pacote e os dados salvos na memória. Os submódulos que não possuírem novas mensagens são zerados no pacote, juntamente com sua flag, e ambos salvos na memória (com valor 0). No momento da leitura do pacotes salvos em memória, as flags irão indicar quais submódulos contém dados válidos e quais devem ser ignorados. Isso faz com que as transmissões de dados enviem somente informações relevantes, economizando banda de transmissão.

#### Dados gerados

Não se aplica

#### Período da tarefa

Depende das demais tarefas: ela deve ter período menor ou igual ao menor período das outras tarefas, de maneira a não perder nenhuma leitura.

#### Prioridade

5

#### Recursos utilizados

GPIO uSDCard_CE;
GPIO Mem_WP;
GPIO Mem0_CE;
GPIO Mem1_CE;
GPIO Mem2_CE;
GPIO Mem2_CE;
GPIO Mem2_CE;
SPI 1 (USCI A1).


## communications_task

#### Descrição

Esta tarefa controla o Link de comunicação (uplink e downlink). Ela irá aguardar a recpeção de mensagens pelo rádio (uplink) e, eventualmente, responderá aos comandos recebidos. Alguns comandos são:
- Shutdown all communications over 24 hours;
- Send data (com as especificações dos dados desejados).

#### Dados gerados

RSSI do rádio
Últimos comandos recebidos

#### Período da tarefa

100ms

#### Prioridade

5

#### Recursos utilizados

GPIO TTC_GPIO0_MAIN;
GPIO TTC_GPIO2_MAIN;
GPIO TTC_RESETn_MAIN;
GPIO TTC_CTRL_RF_SWT_TX;
GPIO TTC_CTRL_RF_SWT_RX;
GPIO Mem2_CE;
GPIO Mem2_CE;
SPI 0 (USCI A0);
GPIO uC_BEACON_0;
GPIO uC_BEACON_1;
GPIO uC_BEACON_2;
GPIO uC_BEACON_3.



## read_internal_sensors_task

#### Descrição

Esta tarefa faz a leitura do sensor de temperatura interno do MSP, da tensão e da corrente de alimentação. Estes valores são enviados por meio de uma fila de mensagens para a tarefa store_data.

#### Dados gerados

Internal temperature (2 bytes)
Supply voltage (2 bytes)
Supply current (2 bytes)
TOTAL = 6 bytes

#### Período da tarefa

1000ms

#### Prioridade

1

#### Recursos utilizados

GPIO OBDH_CURRENT_ADC;
GPIO ADC_VCC_3V3;
ADC.

## imu_interface_task

#### Descrição

Esta tarefa faz a leitura dos sensores que compõem a IMU (Inertial Measure Unit). Estes são: acelerômetro, giroscópio e magnetômetro, em 3 eixos, e um sensor de temperatura. 

#### Dados gerados

Accelerometer axis X(2 bytes)
Accelerometer axis Y(2 bytes)
Accelerometer axis Z(2 bytes)
Gyroscope axis X(2 bytes)
Gyroscope axis Y(2 bytes)
Gyroscope axis Z(2 bytes)
Magnetometer axis X(2 bytes)
Magnetometer axis Y(2 bytes)
Magnetometer axis Z(2 bytes)
Temperature sensor(2 bytes)
TOTAL = 20 bytes

#### Período da tarefa

10.000ms

#### Prioridade

1

#### Recursos utilizados

I2C 1.


## debug_task

#### Descrição

Esta tarefa somente é criada quando o build está em modo "DEBUG". Ela imprime mensagens, via UART, 

#### Dados gerados

Internal temperature (2 bytes)
Supply voltage (2 bytes)
Supply current (2 bytes)
TOTAL = 6 bytes

#### Período da tarefa

1000ms

#### Prioridade

1

#### Recursos utilizados

GPIO OBDH_CURRENT_ADC;
GPIO ADC_VCC_3V3;
ADC.

## imu_interface_task

#### Descrição

Esta tarefa faz a leitura dos sensores que compõem a IMU (Inertial Measure Unit). Estes são: acelerômetro, giroscópio e magnetômetro, em 3 eixos, e um sensor de temperatura. 

#### Dados gerados

Accelerometer axis X(2 bytes)
Accelerometer axis Y(2 bytes)
Accelerometer axis Z(2 bytes)
Gyroscope axis X(2 bytes)
Gyroscope axis Y(2 bytes)
Gyroscope axis Z(2 bytes)
Magnetometer axis X(2 bytes)
Magnetometer axis Y(2 bytes)
Magnetometer axis Z(2 bytes)
Temperature sensor(2 bytes)
TOTAL = 20 bytes

#### Período da tarefa

10.000ms

#### Prioridade

1

#### Recursos utilizados

I2C 1.