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
#include "driverlib/MSP432P4xx/driverlib.h"
#include "ultrasonic.h"

#include <stdio.h>
#include <Utility/DataStructures/float_queue.h>

/*************************************************************
 * DEFINITIONS
 */
// Debug mode...
#define DEBUG_ULTRASONIC

// Pins used
#define LED1_PORT                    GPIO_PORT_P1
#define LED1_PIN                     GPIO_PIN0

#define ULTRASONIC_TRIGGER_PORT1     GPIO_PORT_P3
#define ULTRASONIC_TRIGGER_PIN1      GPIO_PIN7
#define ULTRASONIC_ECHO_PORT1        GPIO_PORT_P3
#define ULTRASONIC_ECHO_PIN1         GPIO_PIN6

#define ULTRASONIC_TRIGGER_PORT2     GPIO_PORT_P3
#define ULTRASONIC_TRIGGER_PIN2      GPIO_PIN5
#define ULTRASONIC_ECHO_PORT2        GPIO_PORT_P5
#define ULTRASONIC_ECHO_PIN2         GPIO_PIN2

#define ULTRASONIC_TRIGGER_PORT3     GPIO_PORT_P5
#define ULTRASONIC_TRIGGER_PIN3      GPIO_PIN1
#define ULTRASONIC_ECHO_PORT3        GPIO_PORT_P5
#define ULTRASONIC_ECHO_PIN3         GPIO_PIN0

#define ULTRASONIC_TRIGGER_PORT4     GPIO_PORT_P2
#define ULTRASONIC_TRIGGER_PIN4      GPIO_PIN3
#define ULTRASONIC_ECHO_PORT4        GPIO_PORT_P1
#define ULTRASONIC_ECHO_PIN4         GPIO_PIN7

// Timers
#define ULTRASONIC_TIMER_MODULE      TIMER_A2_BASE
#define ULTRASONIC_TIMER_INT         INT_TA2_0
#define TIMER_A_TICKPERIOD           1000

// Threshold in cm
#define ULTRASONIC_THRESHOLD         15.0f

// Number of checks
#define ULTRASONIC_CHECK_COUNT       20
/*************************************************************
 * VARIABLES
 */
FloatQueue* smaQueue;
FloatQueue* emaQueue;

/*************************************************************
 * FUNCTIONS
 */
// Public functions
void Ultrasonic_init();

// Private functions
static void initPortsAndPins();
static void initTimers();
static void initInterrupts();
static void initFilters();

#endif /* ULTRASONICMODULE_INIT_H_ */
