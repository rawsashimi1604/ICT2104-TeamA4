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

#include <AccelerometerModule/i2c_library.h>
#include "init.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

/*************************************************************
 * DEFINITIONS
 */
// Addresses on MPU6050
#define RESET_ADDRESS                0x6B
#define ACCEL_ADDRESS                0x3B
#define GYRO_ADDRESS                 0x43
#define TEMP_ADDRESS                 0x41

// How many times to calibrate
#define RECALIBRATION_COUNT          200

// Threshold to detect hump
#define HUMP_PITCH_ANGLE_THRESHOLD       5.0
#define NEG_HUMP_PITCH_ANGLE_THRESHOLD   -5.0

/*************************************************************
 * GLOBAL VARIABLES
 */
static float accErrorX;
static float accErrorY;

static float gyroErrorX;
static float gyroErrorY;
static float gyroErrorZ;

static float humpAngle;
static bool detectedHump;

/*************************************************************
 * FUNCTIONS
 */

// General
void resetMPU6050();
void recalibrateMPU6050();
float getTimerDuration();

// Accelerometer
static float readMPU6050AccelX();
static float readMPU6050AccelY();
static float readMPU6050AccelZ();

// Gyroscope
static float readMPU6050GyroX();
static float readMPU6050GyroY();
static float readMPU6050GyroZ();

// Angles
float getPitchAngle();
void calculateHumpAngle();

// Temperature
static float readMPU6050Temp();

#endif /* ACCEL_H_ */
