/*
 * init.c
 *
 *  Created on: 11 Nov 2022
 *      Author: loowe
 */

/*************************************************************
 * INCLUDES
 */
#include "init.h"

/*************************************************************
 * FUNCTIONS
 */
static void initPortsAndPins()
{
    // Set up I2C Pins
    // Set up SDA PIN...
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            ACCEL_SDA_PORT,
            ACCEL_SDA_PIN,
            GPIO_PRIMARY_MODULE_FUNCTION
    );

    // Set up SCL PIN...
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            ACCEL_SCL_PORT,
            ACCEL_SCL_PIN,
            GPIO_PRIMARY_MODULE_FUNCTION
    );

    // --------------------------------------------
    // Set up button pin for testing
    // Set Button 1 as pull up resistor
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

static void initInterrupts() {
    // Clear the interrupt flags for I2C
    I2C_clearInterruptFlag(
            I2C_MODULE,
            EUSCI_B_I2C_TRANSMIT_INTERRUPT0 |
            EUSCI_B_I2C_RECEIVE_INTERRUPT0
    );

    // DEBUG --------------------------------------------
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    Interrupt_enableInterrupt(INT_PORT1);
    // --------------------------------------------

    // Enable interrupts for TIMER_A1
    Interrupt_enableInterrupt(INT_TA1_0);

    // Enable interrupts globally...
    Interrupt_enableMaster();

}

static void initTimers() {

    // Halt watchdog timer so that the microcontroller does not restart.
    WDT_A_holdTimer();

    // Enable timer needed for roll and pitch measurement (1MHz clock)
    const Timer_A_UpModeConfig upConfig =
    {
        TIMER_A_CLOCKSOURCE_ACLK,              // ACLK 32768Hz Clock Source
        TIMER_DIVIDER,                         // ACLK/10 = 3276.8 Hz
        TIMER_TICKPERIOD,                      // 10 sec interrupt
        TIMER_A_TAIE_INTERRUPT_DISABLE,        // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,   // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                       // Clear value
    };


    // Configure and reset timer...
    Timer_A_configureUpMode(TIMER_MODULE, &upConfig);
    Timer_A_clearTimer(TIMER_MODULE);
}

static void initI2CConfigs(void)
{

    // I2C Config
    const eUSCI_I2C_MasterConfig i2cConfig =
    {
            EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
            3000000,                                // SMCLK = 3MHz
            EUSCI_B_I2C_SET_DATA_RATE_400KBPS,      // Desired I2C Clock of 400khz
            0,                                      // No byte counter threshold
            EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
    };

    // Init I2C Master Block for specified I2C_MODULE
    I2C_initMaster(I2C_MODULE, &i2cConfig);

    // Specify slave address
    I2C_setSlaveAddress(I2C_MODULE, SLAVE_ADDRESS);

    // reset I2C module
    I2C_disableModule(I2C_MODULE);

    /* Enable I2C Module to start operations */
    I2C_enableModule(I2C_MODULE);

    return;
}

void Accelerometer_init() {

    // Init GPIO Pins required for MPU6050
    initPortsAndPins();

    // Init I2C Configurations required for MPU6050 communication
    initI2CConfigs();

    // Init Timers
    initTimers();

    // Enable interrupts for I2C Protocol and Timers
    initInterrupts();

    // Reset MPU6050 on program start
    resetMPU6050();

}


