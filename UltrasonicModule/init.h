/*
 * init.h
 *
 *  Created on: 28 Sep 2022
 *      Author: loowe
 */

#ifndef ULTRASONICMODULE_INIT_H_
#define ULTRASONICMODULE_INIT_H_

/*************************************************************
 * INCLUDES
 */
#include "driverlib.h"

#include <stdio.h>

/*************************************************************
 * DEFINITIONS
 */
// Debug mode...
#define DEBUG_ULTRASONIC

// Pins used
#define LED1_PORT                   GPIO_PORT_P1
#define LED1_PIN                    GPIO_PIN0

#define ULTRASONIC_TRIGGER_PORT     GPIO_PORT_P1
#define ULTRASONIC_TRIGGER_PIN      GPIO_PIN5

#define ULTRASONIC_ECHO_PORT        GPIO_PORT_P2
#define ULTRASONIC_ECHO_PIN         GPIO_PIN4

// Timers
#define TIMER_A0_TICKPERIOD         1000

// Threshold in cm
#define ULTRASONIC_THRESHOLD        15.0f

/*************************************************************
 * FUNCTIONS
 */
// Public functions
void _initUltrasonicSensor();

// Private functions
void initPortsAndPins();
void initTimers();
void initInterrupts();



#endif /* ULTRASONICMODULE_INIT_H_ */
