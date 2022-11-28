#ifndef ULTRASONICMODULE_ULTRASONIC_H_
#define ULTRASONICMODULE_ULTRASONIC_H_

/*************************************************************
 * INCLUDES
 */
#include "init.h"

/*************************************************************
 * DEFINITIONS
 */
#define ULTRASONIC_BUFFER_FRONT_INDEX 0
#define ULTRASONIC_BUFFER_LEFT_INDEX 1
#define ULTRASONIC_BUFFER_RIGHT_INDEX 2
#define ULTRASONIC_BUFFER_BACK_INDEX 3

/*************************************************************
 * VARIABLES
 */
float latestSensorDistances[4]; // Stores the latest distances captured from sensor.

unsigned int sensor1InterruptCount;
unsigned int sensor2InterruptCount;
unsigned int sensor3InterruptCount;
unsigned int sensor4InterruptCount;

/*************************************************************
 * STRUCTS
 */
struct UltrasonicSensorConfiguration
{
    unsigned char triggerPort;
    unsigned short triggerPin;
    unsigned char echoPort;
    unsigned short echoPin;
    unsigned int timer;
    unsigned int *sensorInterruptCount;
    unsigned char bufferIndex; // Stores which buffer index to store data in...
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

// Private functions
void Delay(unsigned int loop);

bool checkSensorDetectObject(UltrasonicSensorConfiguration *sensorConfig);
void triggerUltrasonicSensor(UltrasonicSensorConfiguration *sensorConfig);
unsigned int getDuration(UltrasonicSensorConfiguration *sensorConfig);
float getDistance(UltrasonicSensorConfiguration *sensorConfig);

// Interrupts
void TA0_0_IRQHandler(void);
void TA1_0_IRQHandler(void);
void TA2_0_IRQHandler(void);
void TA3_0_IRQHandler(void);

#endif /* ULTRASONICMODULE_ULTRASONIC_H_ */
