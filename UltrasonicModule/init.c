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
static void initPortsAndPins() {

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

static void initInterrupts() {

    // Enable interrupt for Timer_A1
    Interrupt_enableInterrupt(ULTRASONIC_TIMER_INT);

    // Enable interrupts globally...
    Interrupt_enableMaster();

}
static void initTimers() {

    // Halt watchdog timer so that the microcontroller does not restart.
    WDT_A_holdTimer();

    // Set up Timer_A1, used for interrupt count check, (1MHz clock).
    const Timer_A_UpModeConfig upConfig =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_3,          // SMCLK/3 = 1MHz
        TIMER_A_TICKPERIOD,                     // 1000 tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,     // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
    };

    Timer_A_configureUpMode(ULTRASONIC_TIMER_MODULE, &upConfig);
    Timer_A_clearTimer(ULTRASONIC_TIMER_MODULE);

}

static void initFilters() {

    // Create Queue for SMA
    smaQueue = createQueue(SMA_PERIOD);

    // Create Queue for EMA
    emaQueue = createQueue(EMA_PERIOD);

}

void Ultrasonic_init() {

    // Initialize all Ports and Pins
    initPortsAndPins();

    // Initialize all Timers
    initTimers();

    // Initialize interrupt settings
    initInterrupts();

    // Initialize queues for ultrasonic filters
    initFilters();

}
