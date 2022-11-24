
#ifndef ENCODER_H_
#define ENCODER_H_

/*************************************************************
 * INCLUDES
 */

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* Application Defines  */
#define TIMER_A_TICKPERIOD 1000

/*************************************************************
 * FUNCTIONS
 */

// PRIVATE FUNCTIONS
void initTimer(void);
void initPins(void);
void initInterrupts(void);
void PORT3_IRQHandler(void);
void TA2_0_IRQHandler(void);
void detectPulse(volatile Encoder *e);
void updateRPM(volatile Encoder *e);

// PUBLIC FUNCTIONS
void Encoder_init(void);
void Encoder_main(void);
void Infrared_startNotchesCount(void);
float Infrared_getCarSpeed(void);
int Infrared_stopNotchesCount(void);

#endif /* ENCODER_H_ */