/*
 * init.h
 *
 *  Created on: 11 Nov 2022
 *      Author: loowe
 */

#ifndef INIT_H_
#define INIT_H_

/*************************************************************
 * INCLUDES
 */
#include "driverlib/MSP432P4xx/driverlib.h"
#include "accel.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/*************************************************************
 * DEFINITIONS
 */
// I2C Module used
#define I2C_MODULE      EUSCI_B1_BASE

// GPIO PINS used for I2C Protocol
// SCL 6.5
#define ACCEL_SCL_PORT  GPIO_PORT_P6
#define ACCEL_SCL_PIN   GPIO_PIN5

// SDA 6.4
#define ACCEL_SDA_PORT  GPIO_PORT_P6
#define ACCEL_SDA_PIN   GPIO_PIN4

// Slave Address for I2C Slave MPU6050
#define SLAVE_ADDRESS 0x68

/*************************************************************
 * FUNCTIONS
 */
void initPortsAndPins();
void initI2CConfigs();
void initInterrupts();

void Accelerometer_init();

#endif /* INIT_H_ */
