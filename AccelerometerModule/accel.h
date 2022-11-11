/*
 * accel.h
 *
 *  Created on: 11 Nov 2022
 *      Author: loowe
 */

#ifndef ACCEL_H_
#define ACCEL_H_

/*************************************************************
 * INCLUDES
 */

#include "init.h"

#include <i2c_lib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/*************************************************************
 * DEFINITIONS
 */
// Addresses on MPU6050
#define RESET_ADDRESS 0x6B
#define ACCEL_ADDRESS 0x3B
#define GYRO_ADDRESS 0x43
#define TEMP_ADDRESS 0x41

/*************************************************************
 * FUNCTIONS
 */
void resetMPU6050();
int16_t readMPU6050AccelX();
int16_t readMPU6050AccelY();
int16_t readMPU6050AccelZ();

int16_t readMPU6050GyroX();
int16_t readMPU6050GyroY();
int16_t readMPU6050GyroZ();

float readMPU6050Temp();

#endif /* ACCEL_H_ */
