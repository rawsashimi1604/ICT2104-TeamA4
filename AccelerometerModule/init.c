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
void initPortsAndPins()
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
}

void initInterrupts() {
    // Enable and clear the interrupt flags
    I2C_clearInterruptFlag(
            I2C_MODULE,
            EUSCI_B_I2C_TRANSMIT_INTERRUPT0 |
            EUSCI_B_I2C_RECEIVE_INTERRUPT0
    );
}

void initI2CConfigs(void)
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

    // Enable interrupts for I2C
    initInterrupts();

    // Reset MPU6050 on program start
    resetMPU6050();
}


