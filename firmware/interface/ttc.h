/*
 * ttc.h
 *
 *  Created on: 09 de fev de 2017
 *      Author: elder
 */

#ifndef INTERFACE_TTC_H_
#define INTERFACE_TTC_H_

#include "../driver/i2c.h"
#include "../include/floripasat_def.h"

#define TTC_TX_NOT_BUSY     0x00
#define TTC_TX_BUSY         0x01


#define TTC_CMD_SHUTDOWN        0x01
#define TTC_CMD_REQUEST_TX      0x10
#define TTC_CMD_FREE_TX         0x20


void ttc_setup(void);
uint8_t ttc_is_beacon_on(void);
void ttc_prepare_to_tx(void);
void ttc_free_to_send_beacon(void);
void ttc_shutdown(void);
void ttc_return_from_shutdown(void);

#endif /* INTERFACE_TTC_H_ */
