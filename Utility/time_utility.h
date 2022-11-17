/*
 * time_utility.h
 *
 *  Created on: 16 Nov 2022
 *      Author: loowe
 */

#ifndef UTILITY_TIME_UTILITY_H_
#define UTILITY_TIME_UTILITY_H_

#include "driverlib/MSP432P4xx/driverlib.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define UTILITY_DELAY_TIMER       TIMER32_0_BASE
#define UTILITY_DELAY_INTERRUPT   INT_T32_INT1
#define UTILITY_DELAY_DIVIDER     TIMER32_PRESCALER_1
#define UTILITY_DELAY_DIVIDER_INT 1

// Variable
static bool delayFlag;

// Delay
void UtilityTime_initDelayTimer();
void UtilityTime_delay(uint32_t delayMs);

static uint32_t getRegisterValueToSet(uint32_t delayMs);

void T32_0_IRQHandler(void);

#endif /* UTILITY_TIME_UTILITY_H_ */
