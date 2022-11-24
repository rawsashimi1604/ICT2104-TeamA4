/*
 * ultrasonic.h
 *
 *  Created on: 28 Sep 2022
 *      Author: loowe
 */

#ifndef ULTRASONICMODULE_ULTRASONIC_H_
#define ULTRASONICMODULE_ULTRASONIC_H_

/*************************************************************
 * INCLUDES
 */
#include <Utility/DataStructures/float_queue.h>
#include "driverlib/MSP432P4xx/driverlib.h"
#include "Utility/time_utility.h"
#include "Utility/Filters/kalman_filter.h"
#include "Utility/Filters/sma_filter.h"
#include "Utility/Filters/ema_filter.h"
#include "init.h"

/*************************************************************
 * DEFINITIONS
 */
#define ULTRASONIC_BUFFER_FRONT_INDEX   0
#define ULTRASONIC_BUFFER_LEFT_INDEX    1
#define ULTRASONIC_BUFFER_RIGHT_INDEX   2
#define ULTRASONIC_BUFFER_BACK_INDEX    3

#define SMA_PERIOD                      5
#define EMA_PERIOD                      5

/*************************************************************
 * VARIABLES
 */
float latestSensorDistances[4];      // Stores the latest distances captured from sensor.


uint32_t sensor1InterruptCount;

/*************************************************************
 * STRUCTS
 */
struct UltrasonicSensorConfiguration {
    uint_fast8_t triggerPort;
    uint_fast16_t triggerPin;
    uint_fast8_t echoPort;
    uint_fast16_t echoPin;
    uint32_t timer;
    uint32_t* sensorInterruptCount;
    uint8_t bufferIndex;              // Stores which buffer index to store data in...
};

typedef struct UltrasonicSensorConfiguration UltrasonicSensorConfiguration;


/*************************************************************
 * FUNCTIONS
 */

// Public functions to overall robot car
bool Ultrasonic_checkFront();
bool Ultrasonic_checkLeft();
bool Ultrasonic_checkRight();
bool Ultrasonic_checkBack();

float Ultrasonic_getDistanceFromFrontSensor();
float Ultrasonic_getDistanceFromLeftSensor();
float Ultrasonic_getDistanceFromRightSensor();
float Ultrasonic_getDistanceFromBackSensor();

// Interrupts
void TA2_0_IRQHandler(void);

// Private functions
static float filterUltrasonicSensorValues(float distance);
static bool checkSensorDetectObject(UltrasonicSensorConfiguration* sensorConfig);
static void triggerUltrasonicSensor(UltrasonicSensorConfiguration* sensorConfig);
static uint32_t getDuration(UltrasonicSensorConfiguration* sensorConfig);
static float getDistance(UltrasonicSensorConfiguration* sensorConfig);


#endif /* ULTRASONICMODULE_ULTRASONIC_H_ */
