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
 * VARIABLES
 */
float latestSensorDistances[4];

/*************************************************************
 * STRUCTS
 */
UltrasonicSensorConfiguration sensor1Config = {
   ULTRASONIC_TRIGGER_PORT1,
   ULTRASONIC_TRIGGER_PIN1,
   ULTRASONIC_ECHO_PORT1,
   ULTRASONIC_ECHO_PIN1,
   ULTRASONIC_TIMER_MODULE,
   &sensor1InterruptCount,
   ULTRASONIC_BUFFER_FRONT_INDEX
};

UltrasonicSensorConfiguration sensor2Config = {
   ULTRASONIC_TRIGGER_PORT2,
   ULTRASONIC_TRIGGER_PIN2,
   ULTRASONIC_ECHO_PORT2,
   ULTRASONIC_ECHO_PIN2,
   ULTRASONIC_TIMER_MODULE,
   &sensor1InterruptCount,
   ULTRASONIC_BUFFER_LEFT_INDEX
};

UltrasonicSensorConfiguration sensor3Config = {
   ULTRASONIC_TRIGGER_PORT3,
   ULTRASONIC_TRIGGER_PIN3,
   ULTRASONIC_ECHO_PORT3,
   ULTRASONIC_ECHO_PIN3,
   ULTRASONIC_TIMER_MODULE,
   &sensor1InterruptCount,
   ULTRASONIC_BUFFER_RIGHT_INDEX
};

UltrasonicSensorConfiguration sensor4Config = {
   ULTRASONIC_TRIGGER_PORT4,
   ULTRASONIC_TRIGGER_PIN4,
   ULTRASONIC_ECHO_PORT4,
   ULTRASONIC_ECHO_PIN4,
   ULTRASONIC_TIMER_MODULE,
   &sensor1InterruptCount,
   ULTRASONIC_BUFFER_BACK_INDEX
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
    return latestSensorDistances[ULTRASONIC_BUFFER_FRONT_INDEX];
}

float Ultrasonic_getDistanceFromLeftSensor() {
    return latestSensorDistances[ULTRASONIC_BUFFER_LEFT_INDEX];
}

float Ultrasonic_getDistanceFromRightSensor() {
    return latestSensorDistances[ULTRASONIC_BUFFER_RIGHT_INDEX];
}

float Ultrasonic_getDistanceFromBackSensor() {
    return latestSensorDistances[ULTRASONIC_BUFFER_BACK_INDEX];
}

static bool checkSensorDetectObject(UltrasonicSensorConfiguration* sensorConfig) {

    bool hasObject = false;

    // Reset sensor interrupt count
    *(sensorConfig->sensorInterruptCount) = 0;

    // Get distance from object.
    float distance = getDistance(sensorConfig);

    // Filter values
    // KALMAN FILTER!
//    filteredValue = Filter_KalmanFilter(distance, &KALMAN_DATA);
//    printf("FilteredValue: %.2f\n", filteredValue);

    // SMA FILTER!
    // Add to queue
    enqueue(smaQueue, distance);

    // Get the SMA
    float smaFilterVal = Filter_SMAFilter(smaQueue);
    printf("\nSMA: %.2f\n", smaFilterVal);

    // EMA FILTER!
    // Add to queue
    enqueue(emaQueue, distance);

    // Get the EMA
    float emaFilterVal = Filter_EMAFilter(emaQueue, distance);
    printf("EMA: %.2f\n", emaFilterVal);

    // Sets global distance buffer that stores latest distances captured from sensor.
    latestSensorDistances[sensorConfig->bufferIndex] = distance;

    // If distance is lower then threshold, object is near.
    hasObject = distance < ULTRASONIC_THRESHOLD;

    printf("Distance: %.2fcm\n", distance);

    return hasObject;
}

static void triggerUltrasonicSensor(UltrasonicSensorConfiguration* sensorConfig) {

    GPIO_setOutputHighOnPin(sensorConfig->triggerPort, sensorConfig->triggerPin);

    // Set delay of 1ms to trigger ultrasonic sensor
    UtilityTime_delay(1);

    GPIO_setOutputLowOnPin(sensorConfig->triggerPort, sensorConfig->triggerPin);

}

static uint32_t getDuration(UltrasonicSensorConfiguration* sensorConfig) {

    uint32_t pulseTime = 0;

    pulseTime = *(sensorConfig->sensorInterruptCount) * TIMER_A_TICKPERIOD;
    pulseTime += Timer_A_getCounterValue(sensorConfig->timer);

    // Clear Timer
    Timer_A_clearTimer(sensorConfig->timer);

    return pulseTime;
}

static float getDistance(UltrasonicSensorConfiguration* sensorConfig) {
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

void TA2_0_IRQHandler(void)
{

    // Increase global interrupt count
    sensor1InterruptCount++;

    /* Clear interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(ULTRASONIC_TIMER_MODULE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}


