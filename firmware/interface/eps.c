/*
 * eps.c
 *
 *  Created on: 11 de mai de 2016
 *      Author: mario
 */

#include <eps.h>

void eps_read(char* data){
//     Clear frame memory space
    int i;
    for (i=0; i<EPS_DATA_LENGTH; i++){
        data[i] = 0x00;
    }
//	i2c_read_eps(data,EPS_DATA_LENGTH);
}


char* eps_data2string(char* stringBuffer, char* epsData) {

    if (DEBUG_LOG_ENABLE) {

        //		EPS response:
        //		[Current H][Current L]
        //		[Voltage BAT1 H][Voltage BAT1 L]
        //		[Voltage BAT2 H][Voltage BAT2 L]
        //		[Temperature  H][Temperature  L]
        //		[Current Accum. H][Current Accum  L]
        //		[Bat Mon. Protection Reg.]

        //	char[0]-char[2] = SOF
        char currBatH = epsData[3];
        char currBatL = epsData[4];
        char voltBat1H = epsData[5];
        char voltBat1L = epsData[6];
        char voltBat2H = epsData[7];
        char voltBat2L = epsData[8];
        char tempH = epsData[9];
        char tempL = epsData[10];
        char currAccH = epsData[11];
        char currAccL = epsData[12];
        char batReg = epsData[13];
        //	char[14]-char[16] = EOF

        float currBat = 0;
        int sinal = 0, a = 0, b = 0;
        sinal = currBatH & 0x80;
        if (sinal == 0x00) {
            b = currBatH;
            a = currBatL;
        }
        if (sinal == 0x80) {
            b = ~currBatH;
            a = ~currBatL + 1;
            a &= 255;
        }
        b = b << 8;
        a = a + b;
        currBat = a * 0.0000015625 / 0.015;

        a = voltBat1L;
        b = voltBat1H;
        b = b << 8;
        a = a + b;
        float voltBat1 = a * 0.004886;

        a = voltBat2L;
        b = voltBat2H;
        b = b << 8;
        a = a + b;
        float voltBat2 = a * 0.004886;

        a = tempL;
        b = tempH;
        b = b << 8;
        a = a + b;
        float temp = a * 0.125;

        a = currAccL;
        b = currAccH;
        b = b << 8;
        a = a | b;
        float currAcc = a * 0.00000625 / 0.015;

        sprintf(stringBuffer, "    Bat Curr: %.3f A"
                "  Bat1 Volt: %.3f V"
                "  Bat2 Volt: %.3f V"
                "  Tempe: %.3f C"
                "  Curr Acc: %.3f A"
                "  Bat Reg: 0x%02X", currBat, voltBat1, voltBat2, temp, currAcc,
                batReg);
    }

    return stringBuffer;
}

