#ifndef ULTRASONICMODULE_INIT_H_
#define ULTRASONICMODULE_INIT_H_

/*************************************************************
 * INCLUDES
 */

#include <stdio.h>
#include <stdbool.h>

/*************************************************************
 * DEFINITIONS
 */
// Debug mode...
#define DEBUG_ULTRASONIC

// Pins used
#define LED1_PORT GPIO_PORT_P1
#define LED1_PIN GPIO_PIN0

#define ULTRASONIC_TRIGGER_PORT1 GPIO_PORT_P3
#define ULTRASONIC_TRIGGER_PIN1 GPIO_PIN2
#define ULTRASONIC_ECHO_PORT1 GPIO_PORT_P6
#define ULTRASONIC_ECHO_PIN1 GPIO_PIN1

#define ULTRASONIC_TRIGGER_PORT2 GPIO_PORT_P3
#define ULTRASONIC_TRIGGER_PIN2 GPIO_PIN3
#define ULTRASONIC_ECHO_PORT2 GPIO_PORT_P4
#define ULTRASONIC_ECHO_PIN2 GPIO_PIN0

#define ULTRASONIC_TRIGGER_PORT3 GPIO_PORT_P4
#define ULTRASONIC_TRIGGER_PIN3 GPIO_PIN1
#define ULTRASONIC_ECHO_PORT3 GPIO_PORT_P4
#define ULTRASONIC_ECHO_PIN3 GPIO_PIN2

#define ULTRASONIC_TRIGGER_PORT4 GPIO_PORT_P4
#define ULTRASONIC_TRIGGER_PIN4 GPIO_PIN3
#define ULTRASONIC_ECHO_PORT4 GPIO_PORT_P4
#define ULTRASONIC_ECHO_PIN4 GPIO_PIN4

// Timers
#define TIMER_A_TICKPERIOD 1000

// Threshold in cm
#define ULTRASONIC_THRESHOLD 30.0f

/*************************************************************
 * FUNCTIONS
 */
// Public functions
void Ultrasonic_initUltrasonicSensor();

// Private functions
void initPortsAndPins();
void initTimers();
void initInterrupts();

#endif /* ULTRASONICMODULE_INIT_H_ */