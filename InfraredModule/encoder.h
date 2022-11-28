/** @file encoder.h
 *
 * @brief This module is able to
 * 1) calculate linear velocity of the car
 * 2) calculate linear distance travelled by the car
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2018 Barr Group.  All rights reserved.
 */

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

typedef struct EncoderStruct
{
    volatile int notches_detected;
    volatile int notch_counter;
    volatile int pulses_p_min;
    volatile int prev_RPM;
    volatile int new_RPM;
    volatile int last_pulse_interval;
    volatile int start_time;
    volatile int end_time;
} Encoder;

/*************************************************************
 * FUNCTIONS
 */

// INTERRUPTS
void TA2_0_IRQHandler(void);
void PORT3_IRQHandler(void);

// PRIVATE FUNCTIONS
static void initTimer(void);
static void initPins(void);
static void initInterrupts(void);
static void detectPulse(volatile Encoder *e);
void updateRPM(volatile Encoder *e);

// PUBLIC FUNCTIONS
void Encoder_init(void);
void Encoder_main(void);
void Infrared_startNotchesCount(void);
int  Infrared_stopNotchesCount(void);
float Infrared_getCarSpeed(void);

#endif /* ENCODER_H_ */

/*** end of file ***/
