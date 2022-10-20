/*
 * ultrasonic.h
 *
 *  Created on: 28 Sep 2022
 *      Author: loowe
 */

#ifndef ULTRASONICMODULE_ULTRASONIC_H_
#define ULTRASONICMODULE_ULTRASONIC_H_

/*************************************************************
 * INCLUDES
 */
#include "driverlib/MSP432P4xx/driverlib.h"
#include "init.h"

/*************************************************************
 * DEFINITIONS
 */

/*************************************************************
 * VARIABLES
 */
uint32_t sensorInterruptCount;

/*************************************************************
 * FUNCTIONS
 */

// Public functions to overall robot car
bool Ultrasonic_checkFront();

// Private functions
void Delay(uint32_t loop);

void triggerUltrasonicSensor();
float getDistance();
uint32_t getDuration(void);

// Interrupts
void TA0_0_IRQHandler(void);

#endif /* ULTRASONICMODULE_ULTRASONIC_H_ */
