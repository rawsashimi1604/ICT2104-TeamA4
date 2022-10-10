/*
 * ultrasonic.c
 *
 *  Created on: 28 Sep 2022
 *      Author: loowe
 */

/*************************************************************
 * INCLUDES
 */
#include "ultrasonic.h"

/*************************************************************
 * Functions
 */

bool Ultrasonic_checkFront() {

    bool frontHasObject = false;

    // Get distance from object.
    float distance = getDistance();

    // If distance is lower then threshold, object is near.
    frontHasObject = distance < ULTRASONIC_THRESHOLD;

    printf("Distance: %.2f\n", distance);

    if (frontHasObject) {
        GPIO_setOutputHighOnPin(LED1_PORT, LED1_PIN);
    } else {
        GPIO_setOutputLowOnPin(LED1_PORT, LED1_PIN);
    }

    return frontHasObject;
}

void Delay(uint32_t loop)
{
    volatile uint32_t i;

    for (i = 0 ; i < loop ; i++);
}


void triggerUltrasonicSensor() {

    GPIO_setOutputHighOnPin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN);
    Delay(1000);
    GPIO_setOutputLowOnPin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN);

}

float getDistance() {

    uint32_t pulseDuration = 0;
    float distance = 0;

    triggerUltrasonicSensor();

    // Wait for positive edge from sensor...
    while(GPIO_getInputPinValue(ULTRASONIC_ECHO_PORT, ULTRASONIC_ECHO_PIN) == 0);

    // Restart global interrupt count for sensor
    sensorInterruptCount = 0;

    // Start timer.
    Timer_A_clearTimer(TIMER_A0_BASE);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    // Detect Negative edge
    while(GPIO_getInputPinValue(ULTRASONIC_ECHO_PORT, ULTRASONIC_ECHO_PIN) == 1);

    // Stop timer.
    Timer_A_stopTimer(TIMER_A0_BASE);

    // Obtain Pulse Width in microseconds...
    pulseDuration = getDuration();

    // Calculate distance in terms of cm
    distance = (float)pulseDuration / 58.0f;

    return distance;
}

uint32_t getDuration(void) {

    uint32_t pulseTime = 0;

    pulseTime = sensorInterruptCount * TIMER_A0_TICKPERIOD;
    pulseTime += Timer_A_getCounterValue(TIMER_A0_BASE);

    // Clear Timer
    Timer_A_clearTimer(TIMER_A0_BASE);

    Delay(3000);

    return pulseTime;
}

void TA0_0_IRQHandler(void)
{

    // Increase global interrupt count
    sensorInterruptCount++;

    /* Clear interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
