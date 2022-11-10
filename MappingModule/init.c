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
void initPortsAndPins()
{
    // printf("UltrasonicModule: initPortsAndPins()\n");
}

void initInterrupts()
{
    // printf("UltrasonicModule: initInterrupts()\n");
}
void initTimers()
{
    // printf("UltrasonicModule: initTimers()\n");
}
void Ultrasonic_initUltrasonicSensor()
{

    // Initialize all Ports and Pins
    initPortsAndPins();

    // Initialize all Timers
    initTimers();

    // Initialize interrupt settings
    initInterrupts();
}