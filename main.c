/*
 * Lab 4, done by
 * Gavin Loo Wei Ren (2101927)
 * Lim Jian Wei (2101391)
 */

/*************************************************************
 * INCLUDES
 */
#include "driverlib.h"
#include "UltrasonicModule/ultrasonic.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/*************************************************************
 * DEFINITIONS
 */

/*************************************************************
 * Global Variables
 */

// https://github.com/tejaChintalapati/Ultrasonic-MSP430FR5969
// https://www.youtube.com/watch?v=LzOVfDQaol8&ab_channel=drselim
// https://e2e.ti.com/support/microcontrollers/msp-low-power-microcontrollers-group/msp430/f/msp-low-power-microcontroller-forum/607119/msp432p401r-timer-a---capture-mode-for-ultrasonic-sensor-hc-sr04
// https://e2e.ti.com/support/microcontrollers/msp-low-power-microcontrollers-group/msp430/f/msp-low-power-microcontroller-forum/861680/ccs-msp432p401r-timer-a---capture-mode-for-ultrasonic-sensor-hc-sr04

int main(void)
{

    // Initialize settings for Ultrasonic Sensor HC-SR04.
    _initUltrasonicSensor();


    // put to lower power mode...
    while (1)
    {
        Delay(100000);
        _checkFront();
    }
}



