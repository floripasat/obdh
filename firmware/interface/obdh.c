/*
 * obdh.c
 *
 *  Created on: 30 de mai de 2016
 *      Author: mario
 */

// Interface with internal components of the OBDH SoC

#include <interface/obdh.h>

void obdh_read(char* obdhData) {

    //     Clear frame memory space
    int i;
    uint32_t systime_s;
    uint32_t systime_ms;

    volatile unsigned long systick;

    for (i = 0; i < OBDH_DATA_LENGTH; i++) {
        obdhData[i] = 0x00;
    }


    //systick = (TickType_t)xTaskGetTickCount();
    systick = 0; //TODO mudar
    //systime_ms = systick*1000/configTICK_RATE_HZ;
    systime_ms = 0;
    systime_s = systime_ms/1000.0;
    systime_ms = systime_ms%1000;
    //TODO acho que o melhor, em questão de processamento, é enviar esta informação em ms apenas e realizar a conersão para dias, horas, minutos,... no receptor

    obdh_temp_read();

    obdhData[0] = systime_s >> 8;		//sysclock  S high
    obdhData[1] = systime_s;		//sysclock  S low
    obdhData[2] = systime_ms >> 8;		//sysclock MS high
    obdhData[3] = systime_ms;		//sysclock MS low
    obdhData[4] = obdhTemperatureBuffer >> 8;		//temperature high
    obdhData[5] = obdhTemperatureBuffer;		//temperature low
    obdhData[6] = 5;		//system status code
}

char* obdh_data2string(char* stringBuffer, char* obdhData) {

    if (DEBUG_LOG_ENABLE) {

        temperatureDegC = (float) (((long) obdhTemperatureBuffer
                - CALADC12_15V_30C) * (85 - 30))
                / (CALADC12_15V_85C - CALADC12_15V_30C) + 30.0f;
        // Temperature in Fahrenheit Tf = (9/5)*Tc + 32
        //	temperatureDegF = temperatureDegC * 9.0f / 5.0f + 32.0f;

        sprintf(stringBuffer, "    Internal OBDH Temperature: %.3f C",
                temperatureDegC);
    }
    return stringBuffer;
}



void obdh_temp_convert(unsigned int temp){
	temperatureDegC = (float)(((long)temp - CALADC12_15V_30C) * (85 - 30)) /
	        (CALADC12_15V_85C - CALADC12_15V_30C) + 30.0f;
	// Temperature in Fahrenheit Tf = (9/5)*Tc + 32
	temperatureDegF = temperatureDegC * 9.0f / 5.0f + 32.0f;
}

void obdh_temp_read(void){
    //ADC12CTL0 &= ~ADC12SC;
    //ADC12CTL0 |= ADC12SC;                   // Sampling and conversion start
    //ADC12IE = 0x001;                        // ADC_IFG upon conv result-ADCMEMO
}

void obdh_setup(void){


//	Internal temperature reading setup
	  REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control to
	                                            // ADC12_A ref control registers
	  ADC12CTL0 = ADC12SHT0_8 | ADC12REFON | ADC12ON;
	                                            // Internal ref = 1.5V
	  ADC12CTL1 = ADC12SHP;                     // enable sample timer
	  ADC12MCTL0 = ADC12SREF_1 | ADC12INCH_10;  // ADC i/p ch A10 = temp sense i/p
	  __delay_cycles(DELAY_1_MS_IN_CYCLES);   // Allow ~100us (at default UCS settings)
	                                            // for REF to settle
	  ADC12CTL0 |= ADC12ENC;
}


