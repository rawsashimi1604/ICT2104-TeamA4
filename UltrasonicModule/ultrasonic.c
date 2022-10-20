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
 * STRUCTS
 */
UltrasonicSensorConfiguration sensor1Config = {
   ULTRASONIC_TRIGGER_PORT1,
   ULTRASONIC_TRIGGER_PIN1,
   ULTRASONIC_ECHO_PORT1,
   ULTRASONIC_ECHO_PIN1,
   TIMER_A0_BASE,
   &sensor1InterruptCount
};

UltrasonicSensorConfiguration sensor2Config = {
   ULTRASONIC_TRIGGER_PORT2,
   ULTRASONIC_TRIGGER_PIN2,
   ULTRASONIC_ECHO_PORT2,
   ULTRASONIC_ECHO_PIN2,
   TIMER_A1_BASE,
   &sensor2InterruptCount
};

UltrasonicSensorConfiguration sensor3Config = {
   ULTRASONIC_TRIGGER_PORT3,
   ULTRASONIC_TRIGGER_PIN3,
   ULTRASONIC_ECHO_PORT3,
   ULTRASONIC_ECHO_PIN3,
   TIMER_A2_BASE,
   &sensor3InterruptCount
};

UltrasonicSensorConfiguration sensor4Config = {
   ULTRASONIC_TRIGGER_PORT4,
   ULTRASONIC_TRIGGER_PIN4,
   ULTRASONIC_ECHO_PORT4,
   ULTRASONIC_ECHO_PIN4,
   TIMER_A3_BASE,
   &sensor4InterruptCount
};

/*************************************************************
 * FUNCTIONS
 */

bool Ultrasonic_checkFront() {
    printf("Front -> ");
    return checkSensorDetectObject(&sensor1Config);
}

bool Ultrasonic_checkLeft() {
    printf("Left -> ");
    return checkSensorDetectObject(&sensor2Config);
}

bool Ultrasonic_checkRight() {
    printf("Right -> ");
    return checkSensorDetectObject(&sensor3Config);
}

bool Ultrasonic_checkBack() {
    printf("Back -> ");
    return checkSensorDetectObject(&sensor4Config);
}

float Ultrasonic_getDistanceFromFrontSensor() {
    UltrasonicSensorConfiguration* sensorConfig = &sensor1Config;
    return getDistance(sensorConfig);
}


void Delay(uint32_t loop)
{
    volatile uint32_t i;

    for (i = 0 ; i < loop ; i++);
}

bool checkSensorDetectObject(UltrasonicSensorConfiguration* sensorConfig) {

    bool hasObject = false;

    // Get distance from object.
    float distance = getDistance(sensorConfig);

    // If distance is lower then threshold, object is near.
    hasObject = distance < ULTRASONIC_THRESHOLD;

    printf("Distance: %.2f\n", distance);

//    if (hasObject) {
//        GPIO_setOutputHighOnPin(LED1_PORT, LED1_PIN);
//    } else {
//        GPIO_setOutputLowOnPin(LED1_PORT, LED1_PIN);
//    }

    return hasObject;
}

void triggerUltrasonicSensor(UltrasonicSensorConfiguration* sensorConfig) {

    GPIO_setOutputHighOnPin(sensorConfig->triggerPort, sensorConfig->triggerPin);
    Delay(1000);
    GPIO_setOutputLowOnPin(sensorConfig->triggerPort, sensorConfig->triggerPin);

}

uint32_t getDuration(UltrasonicSensorConfiguration* sensorConfig) {

    uint32_t pulseTime = 0;

    pulseTime = *(sensorConfig->sensorInterruptCount) * TIMER_A_TICKPERIOD;
    pulseTime += Timer_A_getCounterValue(sensorConfig->timer);

    // Clear Timer
    Timer_A_clearTimer(sensorConfig->timer);

    Delay(3000);

    return pulseTime;
}

float getDistance(UltrasonicSensorConfiguration* sensorConfig) {
    uint32_t pulseDuration = 0;
    float distance = 0;

    triggerUltrasonicSensor(sensorConfig);

    // Wait for positive edge from sensor...
    while(GPIO_getInputPinValue(sensorConfig->echoPort, sensorConfig->echoPin) == 0);

    // Restart global interrupt count for sensor
    *(sensorConfig->sensorInterruptCount) = 0;

    // Start timer.
    Timer_A_clearTimer(sensorConfig->timer);
    Timer_A_startCounter(sensorConfig->timer, TIMER_A_UP_MODE);

    // Detect Negative edge
    while(GPIO_getInputPinValue(sensorConfig->echoPort, sensorConfig->echoPin) == 1);

    // Stop timer.
    Timer_A_stopTimer(sensorConfig->timer);

    // Obtain Pulse Width in microseconds...
    pulseDuration = getDuration(sensorConfig);

    // Calculate distance in terms of cm
    distance = (float)pulseDuration / 58.0f;

    return distance;
}

void TA0_0_IRQHandler(void)
{

    // Increase global interrupt count
    sensor1InterruptCount++;

    /* Clear interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

void TA1_0_IRQHandler(void)
{

    // Increase global interrupt count
    sensor2InterruptCount++;

    /* Clear interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

void TA2_0_IRQHandler(void)
{

    // Increase global interrupt count
    sensor3InterruptCount++;

    /* Clear interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}


void TA3_0_IRQHandler(void)
{

    // Increase global interrupt count
    sensor4InterruptCount++;

    /* Clear interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

