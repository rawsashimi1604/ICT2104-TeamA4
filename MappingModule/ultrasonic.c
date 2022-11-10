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

/*************************************************************
 * FUNCTIONS
 */

bool Ultrasonic_checkFront()
{
    // printf("UltrasonicModule: Ultrasonic_checkFront()\n");
    return true;
}

bool Ultrasonic_checkLeft()
{
    // printf("UltrasonicModule: Ultrasonic_checkLeft()\n");
    return true;
}

bool Ultrasonic_checkRight()
{
    // printf("UltrasonicModule: Ultrasonic_checkRight()\n");
    return true;
}

bool Ultrasonic_checkBack()
{
    // printf("UltrasonicModule: Ultrasonic_checkBack()\n");
    return true;
}

float Ultrasonic_getDistanceFromFrontSensor()
{
    return 12.34;
}

float Ultrasonic_getDistanceFromLeftSensor()
{
    return 12.34;
}

float Ultrasonic_getDistanceFromRightSensor()
{
    return 12.34;
}

float Ultrasonic_getDistanceFromBackSensor()
{
    return 12.34;
}