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
#include "driverlib/MSP432P4xx/driverlib.h"
#include "init.h"

/*************************************************************
 * DEFINITIONS
 */

/*************************************************************
 * VARIABLES
 */
uint32_t sensor1InterruptCount;
uint32_t sensor2InterruptCount;
uint32_t sensor3InterruptCount;
uint32_t sensor4InterruptCount;

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
//uint32_t Ultrasonic_getDistanceFromLeftSensor();
//uint32_t Ultrasonic_getDistanceFromRighttSensor();
//uint32_t Ultrasonic_getDistanceFromBackSensor();

// Private functions
void Delay(uint32_t loop);

bool checkSensorDetectObject(UltrasonicSensorConfiguration* sensorConfig);
void triggerUltrasonicSensor(UltrasonicSensorConfiguration* sensorConfig);
uint32_t getDuration(UltrasonicSensorConfiguration* sensorConfig);
float getDistance(UltrasonicSensorConfiguration* sensorConfig);

// Interrupts
void TA0_0_IRQHandler(void);
void TA1_0_IRQHandler(void);
void TA2_0_IRQHandler(void);
void TA3_0_IRQHandler(void);

#endif /* ULTRASONICMODULE_ULTRASONIC_H_ */
