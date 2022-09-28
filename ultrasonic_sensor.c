/*
 * Lab 4, done by
 * Gavin Loo Wei Ren (2101927)
 * Lim Jian Wei (2101391)
 */

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/* Macro Definitions */
#define TIMER_A1_PERIOD 2048                    // ACLK / DIVIDER (Timer interrupts every 2s)
#define TIMER_A2_PERIOD 327                     // ACLK / DIVIDER (Timer interrupts every ~10ms)
#define TIMER_A3_PERIOD 1024                    // ACLK / DIVIDER (Timer interrupts every 1s)

#define MIN_DISTANCE    15.0f                   // Distance Threshold (in cm) for sensor

/* Global Variables */
uint32_t timerInterruptCount;
bool isEchoReceived;

/*
 * Initialize settings for Ultrasonic Sensor HC-SR04.
 *
 */
void intializeUltrasonicSensor() {

    // Halting the Watchdog
    WDT_A_holdTimer();


    // TimerA1 -> Interval between each ultrasonic sensor check...
    // Configuration settings for Timer_A1 (every 2s)
    const Timer_A_UpModeConfig timerA1Config =
    {
         TIMER_A_CLOCKSOURCE_ACLK,              // ACLK Clock Source = 32768
         TIMER_A_CLOCKSOURCE_DIVIDER_32,        // CLK = 32768 / 32 = 1024 (1024 ticks = 1s)
         TIMER_A1_PERIOD,                       // Timer period = ACLK / DIVIDER
         TIMER_A_TAIE_INTERRUPT_DISABLE,        // Disable TAIE interrupt
         TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,    // Interrupt that happens when CCR clears
         TIMER_A_DO_CLEAR                       // Clear value in CCR when timer hits max or TIMER_PERIOD...
    };

    // TimerA2 -> Used for sending trigger to ultrasonic sensor...
    // Configuration settings for Timer_A2 (every 1/32768s )
    const Timer_A_UpModeConfig timerA2Config =
    {
         TIMER_A_CLOCKSOURCE_ACLK,              // ACLK Clock Source = 32768
         TIMER_A_CLOCKSOURCE_DIVIDER_1,         // CLK = 32768 / 1 = 32768 (32768 ticks = 1s)
         TIMER_A2_PERIOD,                       // Timer period = ACLK / DIVIDER
         TIMER_A_TAIE_INTERRUPT_DISABLE,        // Disable TAIE interrupt
         TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,    // Interrupt that happens when CCR clears
         TIMER_A_DO_CLEAR                       // Clear value in CCR when timer hits max or TIMER_PERIOD...
    };

    // TimerA3 -> Used to calculate distance from sensor to object...
    // Configuration settings for Timer_A3 (every 1s)
    const Timer_A_UpModeConfig timerA3Config =
    {
         TIMER_A_CLOCKSOURCE_ACLK,              // ACLK Clock Source = 32768
         TIMER_A_CLOCKSOURCE_DIVIDER_32,        // CLK = 32768 / 32 = 1024 (1024 ticks = 1s)
         TIMER_A3_PERIOD,                       // Timer period = ACLK / DIVIDER
         TIMER_A_TAIE_INTERRUPT_DISABLE,        // Disable TAIE interrupt
         TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,    // Interrupt that happens when CCR clears
         TIMER_A_DO_CLEAR                       // Clear value in CCR when timer hits max or TIMER_PERIOD...
    };

    // Configure Timers for Up Mode
    Timer_A_configureUpMode(TIMER_A1_BASE, &timerA1Config);
    Timer_A_configureUpMode(TIMER_A2_BASE, &timerA2Config);
    Timer_A_configureUpMode(TIMER_A3_BASE, &timerA3Config);


    // Set 1.0 to output (LED)
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    // Set 2.0 to output (LED)
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);

    // Set 3.6 to output (echo)
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);

    // Set 3.7 to input (pulse)
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN7);

    // Interrupt configurations
    // Save power, enabling processor to sleep when exiting an ISR.
    Interrupt_enableSleepOnIsrExit();

    // Enable interrupt for Port 3.7 (echo from sensor)
    GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN7);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN7);
    Interrupt_enableInterrupt(INT_PORT3);

    // Enable interrupt for Timer_A_1, Timer_A_2 Instance 0
    Interrupt_enableInterrupt(INT_TA1_0);
    Interrupt_enableInterrupt(INT_TA2_0);
    Interrupt_enableInterrupt(INT_TA3_0);

    // Enable interrupts globally...
    Interrupt_enableMaster();

    // Start counter...
    Timer_A_clearTimer(TIMER_A1_BASE);
    Timer_A_clearTimer(TIMER_A2_BASE);
    Timer_A_clearTimer(TIMER_A3_BASE);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);


}

int main(void)
{
    // Initialize settings for Ultrasonic Sensor HC-SR04.
    intializeUltrasonicSensor();

    // put to lower power mode...
    while (1)
    {
        PCM_gotoLPM3();
    }
}

// Return the distance the object is away from...
float getHCSR04Distance(void)
{
    uint32_t pulseDuration = 0;
    float calculateddistance = 0;

    // Trigger the interrupt to send trigger to Ultrasonic Sensor...
    /* Generate 10us pulse at P3.6 */
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);
    Timer_A_clearTimer(TIMER_A2_BASE);
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);

    /* Wait for ultrasonic sensor to start echoing... */

    /* Obtain Pulse Width in microseconds */
    pulseDuration = getHCSR04Time();

    /* Calculating distance in cm */
    calculateddistance = (float)pulseDuration / 58.0f;

    return calculateddistance;
}

// -------------------------------------------------------------------------
// Port 3.7 -> Echo from ultrasonic sensor
// Interrupt handler for Port 3.7
// -------------------------------------------------------------------------
void PORT3_IRQHandler(void) {
    printf("Port 3 IRQ!\n");

    uint_fast16_t pinActivated = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);

    // Check if 3.7 was interrupted...
    if (pinActivated == GPIO_PIN7) {
        printf("Port 3.7 Interrupted! Received signal from sensor!\n");

        timerInterruptCount = 0;

        // Start timer A3
        Timer_A_clearTimer(TIMER_A3_BASE);
        Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);

        // Wait for ultrasonic sensor to stop echoing... (detect stop of interrupt)
        // Stop timer A3
        Timer_A_stopTimer(TIMER_A3_BASE);
    }

    // Clear interrupt flags...
    GPIO_clearInterruptFlag(GPIO_PORT_P3, pinActivated);
}

// -------------------------------------------------------------------------
// TIMER A1 -> Interval between each ultrasonic sensor check...
// Interrupt handler for Timer_A_1 (TIMER_A_CCIE_CCR0_INTERRUPT)...
// -------------------------------------------------------------------------
void TA1_0_IRQHandler(void) {

//    // Trigger LED
//    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

    // Obtain distance from HCSR04 sensor and check if its less then threshold
    if((getHCSR04Distance() < MIN_DISTANCE))
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    else
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);


    // Clear interrupt flag
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

// -------------------------------------------------------------------------
// TIMER A2 -> Sending trigger to ultrasonic sensor (from P3.6)...
// Interrupt handler for Timer_A_2 (TIMER_A_CCIE_CCR0_INTERRUPT)...
// -------------------------------------------------------------------------
void TA2_0_IRQHandler(void) {

    printf("TimerA2 Interrupt Triggered!\n");

    // Set pulse to low after timer finishes 1 cycle...
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);

    // Stop timer...
    Timer_A_stopTimer(TIMER_A2_BASE);

    // Clear interrupt flag
    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

// -------------------------------------------------------------------------
// TIMER A3 -> Used to calculate distance from sensor to object...
// Interrupt handler for Timer_A_3 (TIMER_A_CCIE_CCR0_INTERRUPT)...
// -------------------------------------------------------------------------
void TA3_0_IRQHandler(void) {

    // Increase interrupt count to calculate distance...
    timerInterruptCount++;

    // Clear interrupt flag
    Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
