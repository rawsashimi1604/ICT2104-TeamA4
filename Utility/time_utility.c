/*
 * time_utility.c
 *
 *  Created on: 16 Nov 2022
 *      Author: loowe
 */

#include "time_utility.h"

static bool delayFlag = false;

void UtilityTime_initDelayTimer() {

    // Init 32 Bit Timer
    Timer32_initModule(
            UTILITY_DELAY_TIMER,            // MCLK = 3Mhz
            UTILITY_DELAY_DIVIDER,          // 3Mhz/1 = 1s
            TIMER32_32BIT,                  // 32 Bit timer
            TIMER32_FREE_RUN_MODE           // Free run mode
    );

    // Enable interrupt
    Interrupt_enableInterrupt(UTILITY_DELAY_INTERRUPT);
    Interrupt_enableMaster();

}

void UtilityTime_delay(uint32_t delayMs) {

    // Reset delay Flag
    delayFlag = false;

    // Get register value to set
    // 3000 Ticks in Timer = 1ms
    uint32_t registerValue = getRegisterValueToSet(delayMs);

    // Set the timer register value
    Timer32_setCount(UTILITY_DELAY_TIMER, registerValue);
    uint32_t regValue = Timer32_getValue(UTILITY_DELAY_TIMER);

    // Start the timer
    // It will interrupt when stopped
    Timer32_startTimer(UTILITY_DELAY_TIMER, true);

    // wait for interrupt to occur, block.
    while (!delayFlag);

    Timer32_haltTimer(UTILITY_DELAY_TIMER);

    // interrupt has occured, done!
    return;
}

static uint32_t getRegisterValueToSet(uint32_t delayMs) {
    uint32_t timerFreq = CS_getMCLK() / UTILITY_DELAY_DIVIDER_INT;
    return (timerFreq / 1000) * delayMs;
}

// Interrupt handler
void T32_0_IRQHandler(void) {

    // Set delay flag to true, remove blocking mechanism
    delayFlag = true;

    // Clear interrupt flag...
    Timer32_clearInterruptFlag(UTILITY_DELAY_TIMER);
}

