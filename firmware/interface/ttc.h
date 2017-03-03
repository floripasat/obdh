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

#define TTC_REQUEST_DATA_CMD   0x54      //an arbitrary value was choosen


/* TTC FRAME */
typedef struct {
    uint8_t beacon[3];
} ttc_package_t;

#define TTC_PACKAGE_LENGTH  sizeof(ttc_package_t)


void ttc_setup(void);
void ttc_read(ttc_package_t *package);

#endif /* INTERFACE_TTC_H_ */
