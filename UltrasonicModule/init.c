/*
 * init.c
 *
 *  Created on: 28 Sep 2022
 *      Author: loowe
 */

/*************************************************************
 * INCLUDES
 */
#include "init.h"

/*************************************************************
 * Functions
 */
void initPortsAndPins() {

    // Configure LED1 pin as output
    GPIO_setOutputLowOnPin(LED1_PORT, LED1_PIN);
    GPIO_setAsOutputPin(LED1_PORT, LED1_PIN);

    // Configure Ultrasonic trigger pin as output
    GPIO_setOutputLowOnPin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN);
    GPIO_setAsOutputPin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN);

    // Configure Ultrasonic echo pin as input
    GPIO_setAsInputPinWithPullDownResistor(ULTRASONIC_ECHO_PORT, ULTRASONIC_ECHO_PIN);

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);

}

void initInterrupts() {

    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

    // Enable interrupt for Timer_A0
    Interrupt_enableInterrupt(INT_TA0_0);

    // Enable interrupts globally...
    Interrupt_enableMaster();

}
void initTimers() {

    // Halt watchdog timer so that the microcontroller does not restart.
    WDT_A_holdTimer();

    // Set up Timer_A0, used for interrupt count check (1MHz clock).
    const Timer_A_UpModeConfig upConfig =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_3,          // SMCLK/3 = 1MHz
        TIMER_A0_TICKPERIOD,                    // 1000 tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,     // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
    };

    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
    Timer_A_clearTimer(TIMER_A0_BASE);

}
void _initUltrasonicSensor() {

    // Initialize all Ports and Pins
    initPortsAndPins();

    // Initialize all Timers
    initTimers();

    // Initialize interrupt settings
    initInterrupts();

}
