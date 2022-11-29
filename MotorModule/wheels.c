
#include "motor.h"
#include <stdint.h>

#define MOTOR_PORT GPIO_PORT_P4
#define LEFTWHEEL_A GPIO_PIN6
#define LEFTWHEEL_B GPIO_PIN7
#define RIGHTWHEEL_A GPIO_PIN1
#define RIGHTWHEEL_B GPIO_PIN2
//#define LEFTWHEEL_A GPIO_PIN4
//#define LEFTWHEEL_B GPIO_PIN3
//#define RIGHTWHEEL_A GPIO_PIN2
//#define RIGHTWHEEL_B GPIO_PIN1
/**
 * Set default direction of motor driver
 */

void Wheel_init(void)
{
    /* Right motor GPIO */
    GPIO_setAsOutputPin(MOTOR_PORT, RIGHTWHEEL_A); // Configuring P4.1 as output
    GPIO_setAsOutputPin(MOTOR_PORT, RIGHTWHEEL_B); // Configuring P4.2 as output
    GPIO_setOutputLowOnPin(MOTOR_PORT, RIGHTWHEEL_A); // Set output LOW on P4.1
    GPIO_setOutputLowOnPin(MOTOR_PORT, RIGHTWHEEL_B); // Set output LOW on P4.2
    /* Left motor GPIO */
    GPIO_setAsOutputPin(MOTOR_PORT, LEFTWHEEL_A); // Configuring P4.6 as output
    GPIO_setAsOutputPin(MOTOR_PORT, LEFTWHEEL_B); // Configuring P4.7 as output
    GPIO_setOutputLowOnPin(MOTOR_PORT, LEFTWHEEL_A); // Set output LOW on P4.6
    GPIO_setOutputLowOnPin(MOTOR_PORT, LEFTWHEEL_B); // Set output LOW on P4.7
}

void Set_forward(void)
{
    /* Right motor GPIO Front*/
    GPIO_setOutputLowOnPin(MOTOR_PORT, RIGHTWHEEL_A); // Set output LOW on P4.1
    GPIO_setOutputHighOnPin(MOTOR_PORT, RIGHTWHEEL_B); // Set output HIGH on P4.2
    /* Left motor GPIO Front*/
    GPIO_setOutputHighOnPin(MOTOR_PORT, LEFTWHEEL_A); // Set output HIGH on P4.3
    GPIO_setOutputLowOnPin(MOTOR_PORT, LEFTWHEEL_B); // Set output LOW on P4.4
}

/**
 * Set reverse direction of motor driver
 */
void Set_reverse(void)
{
    /* Right motor GPIO Reverse */
    GPIO_setOutputHighOnPin(MOTOR_PORT, RIGHTWHEEL_A); // Set output HIGH on P4.1
    GPIO_setOutputLowOnPin(MOTOR_PORT, RIGHTWHEEL_B); // Set output LOW on P4.2
    /* Left motor GPIO Reverse*/
    GPIO_setOutputLowOnPin(MOTOR_PORT, LEFTWHEEL_A); // Set output LOW on P4.3
    GPIO_setOutputHighOnPin(MOTOR_PORT, LEFTWHEEL_B); // Set output HIGH on P4.4
}

void Set_leftFrontRightBack(void)
{
    /* Left motor GPIO */
    GPIO_setOutputHighOnPin(MOTOR_PORT, LEFTWHEEL_A); // Set output HIGH on P4.3
    GPIO_setOutputLowOnPin(MOTOR_PORT, LEFTWHEEL_B); // Set output LOW on P4.4
    /* Right motor GPIO  Reverse */
    GPIO_setOutputHighOnPin(MOTOR_PORT, RIGHTWHEEL_A); // Set output HIGH on P4.1
    GPIO_setOutputLowOnPin(MOTOR_PORT, RIGHTWHEEL_B); // Set output LOW on P4.2
}
void Set_leftBackRightFront(void)
{
    /* Left motor GPIO Reverse */
    GPIO_setOutputLowOnPin(MOTOR_PORT, LEFTWHEEL_A); // Set output LOW on P4.3
    GPIO_setOutputHighOnPin(MOTOR_PORT, LEFTWHEEL_B); // Set output HIGH on P4.4
    /* Right motor GPIO */
    GPIO_setOutputLowOnPin(MOTOR_PORT, RIGHTWHEEL_A); // Set output LOW on P4.1
    GPIO_setOutputHighOnPin(MOTOR_PORT, RIGHTWHEEL_B); // Set output HIGH on P4.2
}

void Set_stop(void)
{
    /* Right motor GPIO Reverse */
    GPIO_setOutputLowOnPin(MOTOR_PORT, RIGHTWHEEL_A); // Set output HIGH on P4.1
    GPIO_setOutputLowOnPin(MOTOR_PORT, RIGHTWHEEL_B); // Set output LOW on P4.2
    /* Left motor GPIO Reverse*/
    GPIO_setOutputLowOnPin(MOTOR_PORT, LEFTWHEEL_A); // Set output LOW on P4.3
    GPIO_setOutputLowOnPin(MOTOR_PORT, LEFTWHEEL_B); // Set output HIGH on P4.4
}
