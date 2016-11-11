/*
 * debug.c
 *
 *  Created on: 26 de mai de 2016
 *      Author: mario
 */


#include "debug.h"

void debug_inline(char* msg){
	if (DEBUG_LOG_ENABLE){
		uart_tx(msg);
	}
}



void debug(char* msg){
	if (DEBUG_LOG_ENABLE){
		char tmpStr[16];
		uart_tx("[ ");
		//TODO:
		//should use the returned float from sysclock;
		//but it may be causing syslock drift (due debug print itself of sysclock float math)
		sprintf(tmpStr, "%u", sysclock_read_s() );
		uart_tx(tmpStr);
		uart_tx(".");
		sprintf(tmpStr, "%03u", sysclock_read_ms() );
		uart_tx(tmpStr);
		uart_tx(" ] ");
		uart_tx(msg);
		uart_tx("\n\r");
	}
}



void debug_uint(char* msg, uint16_t value){
	if (DEBUG_LOG_ENABLE){
		char tmpStr[16];
		uart_tx("[ ");
		//TODO:
		//should use the returned float from sysclock;
		//but it may be causing syslock drift (due debug print itself of sysclock float math)
		sprintf(tmpStr, "%u", sysclock_read_s() );
		uart_tx(tmpStr);
		uart_tx(".");
		sprintf(tmpStr, "%03u", sysclock_read_ms() );
		uart_tx(tmpStr);
		uart_tx(" ] ");
		uart_tx(msg);
		uart_tx(" ");
		sprintf(tmpStr, "%u", value);
		uart_tx( tmpStr );
		uart_tx("\n\r");
	}
}


void debug_int(char* msg, int16_t value){
	if (DEBUG_LOG_ENABLE){
		char tmpStr[16];
		uart_tx("[ ");
		//TODO:
		//should use the returned float from sysclock;
		//but it may be causing syslock drift (due debug print itself of sysclock float math)
		sprintf(tmpStr, "%u", sysclock_read_s() );
		uart_tx(tmpStr);
		uart_tx(".");
		sprintf(tmpStr, "%03u", sysclock_read_ms() );
		uart_tx(tmpStr);
		uart_tx(" ] ");
		uart_tx(msg);
		uart_tx(" ");
		sprintf(tmpStr, "%d", value);
		uart_tx( tmpStr );
		uart_tx("\n\r");
	}
}



void debug_float(char* msg, float value){
	if (DEBUG_LOG_ENABLE){
		char tmpStr[16];
		uart_tx("[ ");
		//TODO:
		//should use the returned float from sysclock;
		//but it may be causing syslock drift (due debug print itself of sysclock float math)
		sprintf(tmpStr, "%u", sysclock_read_s() );
		uart_tx(tmpStr);
		uart_tx(".");
		sprintf(tmpStr, "%03u", sysclock_read_ms() );
		uart_tx(tmpStr);
		uart_tx(" ] ");
		uart_tx(msg);
		uart_tx(" ");
		sprintf(tmpStr, "%.3f", value);
		uart_tx( tmpStr );
		uart_tx("\n\r");
	}
}



void debug_array  (char* msg, char* array, uint16_t length){
	if (DEBUG_LOG_ENABLE){
		char tmpStr[16];
		uart_tx("[ ");
		sprintf(tmpStr, "%u", sysclock_read_s() );
		uart_tx(tmpStr);
		uart_tx(".");
		sprintf(tmpStr, "%03u", sysclock_read_ms() );
		uart_tx(tmpStr);
		uart_tx(" ] ");

		uart_tx(msg);

		uart_tx(" {");

		uint16_t i = 0;
		for (i=0; i<(length-1); i++){
			sprintf(tmpStr, "0x%02X,", array[i]);
			uart_tx(tmpStr);
		}
		sprintf(tmpStr, "0x%02X", array[i]);		// prevent last comma
		uart_tx(tmpStr);
		uart_tx("}\n\r");

	}
}


void debug_array_ascii  (char* msg, char* array, uint16_t length){
	if (DEBUG_LOG_ENABLE){
		char tmpStr[16];
		uart_tx("[ ");
		sprintf(tmpStr, "%u", sysclock_read_s() );
		uart_tx(tmpStr);
		uart_tx(".");
		sprintf(tmpStr, "%03u", sysclock_read_ms() );
		uart_tx(tmpStr);
		uart_tx(" ] ");

		uart_tx(msg);

		uart_tx(" {");

		uint16_t i = 0;
		for (i=0; i<(length-1); i++){
			sprintf(tmpStr, "%c", array[i]);
			uart_tx(tmpStr);
		}
		sprintf(tmpStr, "%c", array[i]);		// prevent last comma
		uart_tx(tmpStr);
		uart_tx("}\n\r");

	}
}


