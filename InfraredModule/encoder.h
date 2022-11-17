
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
#define TIMER_PERIOD 150
// TODO: change timer to 48mhz

/*************************************************************
 * FUNCTIONS
 */

// PRIVATE FUNCTIONS
void initTimer(void);
void initPins(void);
void initInterrupts(void);
void getSpeed(void);
void PORT2_IRQHandler(void);
void TA1_0_IRQHandler(void);

// PUBLIC FUNCTIONS
void Encoder_init(void);
void Encoder_main(void);

#endif /* ENCODER_H_ */
