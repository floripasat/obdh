/*
 * ttc.c
 *
 *  Created on: 09 de fev de 2017
 *      Author: elder
 */

#include <ttc.h>

void ttc_setup(void) {

}

uint8_t ttc_is_beacon_on(void) {
    uint8_t beacon_status = TTC_TX_NOT_BUSY;

    //read tx_busy pin
    if(BIT_READ(TTC_TX_BUSY_IN, TTC_TX_BUSY_PIN) != 0) {
        beacon_status = TTC_TX_BUSY;
    }

    return beacon_status;
}

void ttc_prepare_to_tx(void) {
    BIT_SET(TTC_TX_REQUEST_OUT, TTC_TX_REQUEST_PIN);    //set tx_request
    BIT_CLEAR(TTC_SHUTDOWN_OUT, TTC_SHUTDOWN_PIN);      //assure to do not send ttc a shutdown command

    BIT_SET(TTC_INTERRUPT_OUT, TTC_INTERRUPT_PIN);  //set interrupt pin

    __delay_cycles(16000); //1ms

    BIT_CLEAR(TTC_INTERRUPT_OUT, TTC_INTERRUPT_PIN);  //clear interrupt pin
}

void ttc_free_to_send_beacon(void) {
    BIT_CLEAR(TTC_TX_REQUEST_OUT, TTC_TX_REQUEST_PIN);    //set tx_request
    BIT_CLEAR(TTC_SHUTDOWN_OUT, TTC_SHUTDOWN_PIN);      //assure to do not send ttc a shutdown command

    BIT_SET(TTC_INTERRUPT_OUT, TTC_INTERRUPT_PIN);  //set interrupt pin

    __delay_cycles(16000); //1ms

    BIT_CLEAR(TTC_INTERRUPT_OUT, TTC_INTERRUPT_PIN);  //clear interrupt pin
}


void ttc_shutdown(void) {

    BIT_SET(TTC_SHUTDOWN_OUT, TTC_SHUTDOWN_PIN);    //set shutdown pin

    BIT_SET(TTC_INTERRUPT_OUT, TTC_INTERRUPT_PIN);  //set interrupt pin
}

void ttc_return_from_shutdown(void) {

    BIT_CLEAR(TTC_SHUTDOWN_OUT, TTC_SHUTDOWN_PIN);    //clear shutdown pin

    BIT_CLEAR(TTC_INTERRUPT_OUT, TTC_INTERRUPT_PIN);  //clear interrupt pin
}
