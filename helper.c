/* DriverLib Includes */
#include "driverlib.h"

#include "helper.h"

// ================================================================================
// Helper wrapper functions
// ================================================================================

// SWITCHES
void Switch1_init(void)
{
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

// returns true / false
bool is_Switch1_Pressed(void)
{
    // debouncing algo from https://www.e-tinkers.com/2021/05/the-simplest-button-debounce-solution/
    static uint16_t state = 0;
    state = (state << 1) | GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) | 0xfe00;
    return (state == 0xff00);
}

// (high = not pressed) (low = pressed)
void Switch1_interruptEnable(uint8_t transition)
{
    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1, transition);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
}

void Switch2_init(void)
{
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
}

// returns true / false
bool is_Switch2_Pressed(void)
{
    // debouncing algo from https://www.e-tinkers.com/2021/05/the-simplest-button-debounce-solution/
    static uint16_t state = 0;
    state = (state << 1) | GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) | 0xfe00;
    return (state == 0xff00);
}

// (high = not pressed) (low = pressed)
void Switch2_interruptEnable(uint8_t transition)
{
    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN4, transition);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);
}

// ================================================================================

// LED1
void LED1_init(void)
{
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

void LED1_on(void)
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

void LED1_off(void)
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

void LED1_toggle(void)
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

// ================================================================================

// LED2
void LED2_init(uint8_t colour)
{
    GPIO_setAsOutputPin(GPIO_PORT_P2, colour);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, colour);
}

void LED2_on(uint8_t colour)
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, RED + GREEN + BLUE);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, colour);
}

void LED2_off(uint8_t colour)
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, colour);
}

void LED2_toggle(uint8_t colour)
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P2, colour);
}
