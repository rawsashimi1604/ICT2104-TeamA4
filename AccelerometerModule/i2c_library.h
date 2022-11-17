/*
 * i2c.h
 *
 *  Created on: 11 Nov 2022
 *      Author: loowe
 */

#ifndef I2C_LIB_H_
#define I2C_LIB_H_

/*************************************************************
 * INCLUDES
 */
#include "driverlib/MSP432P4xx/driverlib.h"
#include "init.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/*************************************************************
 * DEFINITIONS
 */


/*************************************************************
 * FUNCTIONS
 */

// Read 1 byte from Slave Address
int8_t I2C_read8(int8_t readAddress);
// Read 2 bytes from Slave Address
int16_t I2C_read16(int8_t readAddress);

// Write 1 byte to Slave Address specified address register
void I2C_write8(int8_t writeAddress, int8_t writeByte);
// Write 2 bytes to Slave Address specified address register
void I2C_write16(int8_t writeAddress, int16_t writeBytes);


#endif /* I2C_LIB_H_ */
