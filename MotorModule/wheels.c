
#include "motor.h"
#include <stdint.h>
/**
 * Set default direction of motor driver
 */
void Set_forward(void)
{
    /* Right motor GPIO Front*/
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1); // Set output LOW on P4.1
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN2); // Set output HIGH on P4.2
    /* Left motor GPIO Front*/
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN3); // Set output HIGH on P4.3
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4); // Set output LOW on P4.4
}

/**
 * Set reverse direction of motor driver
 */
void Set_reverse(void)
{
    /* Right motor GPIO Reverse */
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1); // Set output HIGH on P4.1
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2); // Set output LOW on P4.2
    /* Left motor GPIO Reverse*/
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3); // Set output LOW on P4.3
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN4); // Set output HIGH on P4.4
}

void set_leftFrontRightBack(void)
{
    /* Left motor GPIO */
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN3); // Set output HIGH on P4.3
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4); // Set output LOW on P4.4
    /* Right motor GPIO  Reverse */
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1); // Set output HIGH on P4.1
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2); // Set output LOW on P4.2
}
void set_leftBackRightFront(void)
{
    /* Left motor GPIO Reverse */
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3); // Set output LOW on P4.3
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN4); // Set output HIGH on P4.4
    /* Right motor GPIO */
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1); // Set output LOW on P4.1
    GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN2); // Set output HIGH on P4.2
}
