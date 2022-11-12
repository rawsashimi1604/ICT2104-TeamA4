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
    GPIO_setOutputLowOnPin(ULTRASONIC_TRIGGER_PORT1, ULTRASONIC_TRIGGER_PIN1);
    GPIO_setAsOutputPin(ULTRASONIC_TRIGGER_PORT1, ULTRASONIC_TRIGGER_PIN1);

    GPIO_setOutputLowOnPin(ULTRASONIC_TRIGGER_PORT2, ULTRASONIC_TRIGGER_PIN2);
    GPIO_setAsOutputPin(ULTRASONIC_TRIGGER_PORT2, ULTRASONIC_TRIGGER_PIN2);

    GPIO_setOutputLowOnPin(ULTRASONIC_TRIGGER_PORT3, ULTRASONIC_TRIGGER_PIN3);
    GPIO_setAsOutputPin(ULTRASONIC_TRIGGER_PORT3, ULTRASONIC_TRIGGER_PIN3);

    GPIO_setOutputLowOnPin(ULTRASONIC_TRIGGER_PORT4, ULTRASONIC_TRIGGER_PIN4);
    GPIO_setAsOutputPin(ULTRASONIC_TRIGGER_PORT4, ULTRASONIC_TRIGGER_PIN4);

    // Configure Ultrasonic echo pin as input
    GPIO_setAsInputPinWithPullDownResistor(ULTRASONIC_ECHO_PORT1, ULTRASONIC_ECHO_PIN1);
    GPIO_setAsInputPinWithPullDownResistor(ULTRASONIC_ECHO_PORT2, ULTRASONIC_ECHO_PIN2);
    GPIO_setAsInputPinWithPullDownResistor(ULTRASONIC_ECHO_PORT3, ULTRASONIC_ECHO_PIN3);
    GPIO_setAsInputPinWithPullDownResistor(ULTRASONIC_ECHO_PORT4, ULTRASONIC_ECHO_PIN4);


}

void initInterrupts() {

    // Enable interrupt for Timer_A0
    Interrupt_enableInterrupt(INT_TA0_0);

    // Enable interrupts globally...
    Interrupt_enableMaster();

}
void initTimers() {

    // Halt watchdog timer so that the microcontroller does not restart.
    WDT_A_holdTimer();

    // Set up Timer_A0, used for interrupt count check, (1MHz clock).
    const Timer_A_UpModeConfig upConfig1 =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_3,          // SMCLK/3 = 1MHz
        TIMER_A_TICKPERIOD,                     // 1000 tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,     // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
    };

    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig1);
    Timer_A_clearTimer(TIMER_A0_BASE);

}
void Ultrasonic_init() {

    // Initialize all Ports and Pins
    initPortsAndPins();

    // Initialize all Timers
    initTimers();

    // Initialize interrupt settings
    initInterrupts();

}
