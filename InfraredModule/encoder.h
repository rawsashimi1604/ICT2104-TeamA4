
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

void getSpeed(void);
void TA1_0_IRQHandler(void);

#endif /* ENCODER_H_ */
