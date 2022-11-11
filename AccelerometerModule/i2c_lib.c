/*
 * i2c.c
 *
 *  Created on: 11 Nov 2022
 *      Author: loowe
 */

/*************************************************************
 * INCLUDES
 */
#include <i2c_lib.h>

/*************************************************************
 * FUNCTIONS
 */
int8_t I2C_read8(int8_t readAddress)
{
    int val = 0;

    // Transmit the slave register address that we wish to read from (transmit mode)
    /* Set master to transmit mode PL */
    I2C_setMode(I2C_MODULE, EUSCI_B_I2C_TRANSMIT_MODE);

    /* Clear any existing interrupt flag PL */
    I2C_clearInterruptFlag(I2C_MODULE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    /* Wait until ready to write */
    while (I2C_isBusBusy(I2C_MODULE));

    /* Initiate start and send address to read from...  */
    I2C_masterSendMultiByteStart(I2C_MODULE, readAddress);

    /* Wait for TX to finish */
    while(!(I2C_getInterruptStatus(I2C_MODULE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0)));

    /* Initiate stop only */
    I2C_masterSendMultiByteStop(I2C_MODULE);

    /* Wait for Stop to finish */
    while(!I2C_getInterruptStatus(I2C_MODULE, EUSCI_B_I2C_STOP_INTERRUPT));

    // Stop the transmit of the slave register address that we wish to read from
    // Start reading from the slave register (receive mode)

    /*
     * Generate Start condition and set it to receive mode.
     * This sends out the slave address and continues to read
     * until you issue a STOP
     */
    I2C_masterReceiveStart(I2C_MODULE);

    /* Wait for RX buffer to fill */
    while(!(I2C_getInterruptStatus(I2C_MODULE, EUSCI_B_I2C_RECEIVE_INTERRUPT0)));

    /* Read from I2C RX register */
    /* Receive byte then send STOP condition */
    val = I2C_masterReceiveMultiByteFinish(I2C_MODULE);

    /* Return value */
    return (int8_t)val;
}

int16_t I2C_read16(int8_t readAddress)
{
    int val = 0;
    int valScratch = 0;

    // Transmit the slave register address that we wish to read from (transmit mode)
    /* Set master to transmit mode PL */
    I2C_setMode(I2C_MODULE, EUSCI_B_I2C_TRANSMIT_MODE);

    /* Clear any existing interrupt flag PL */
    I2C_clearInterruptFlag(I2C_MODULE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    /* Wait until ready to write */
    while (I2C_isBusBusy(I2C_MODULE));

    /* Initiate start and send address to read from...  */
    I2C_masterSendMultiByteStart(I2C_MODULE, readAddress);

    /* Wait for TX to finish */
    while(!(I2C_getInterruptStatus(I2C_MODULE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0)));

    /* Initiate stop only */
    I2C_masterSendMultiByteStop(I2C_MODULE);

    /* Wait for Stop to finish */
    while(!I2C_getInterruptStatus(I2C_MODULE, EUSCI_B_I2C_STOP_INTERRUPT));

    // Stop the transmit of the slave register address that we wish to read from
    // Start reading from the slave register (receive mode)

    /*
     * Generate Start condition and set it to receive mode.
     * This sends out the slave address and continues to read
     * until you issue a STOP
     */
    I2C_masterReceiveStart(I2C_MODULE);

    /* Wait for RX buffer to fill */
    while(!(I2C_getInterruptStatus(I2C_MODULE, EUSCI_B_I2C_RECEIVE_INTERRUPT0)));

    /* Read from I2C RX register */
    val = I2C_masterReceiveMultiByteNext(I2C_MODULE);

    /* Receive second byte then send STOP condition */
    valScratch = I2C_masterReceiveMultiByteFinish(I2C_MODULE);

    /* Shift val to top MSB */
    val = (val << 8);

    /* Read from I2C RX Register and write to LSB of val */
    val |= valScratch;

    /* Return value */
    return (int16_t)val;
}

void I2C_write8 (int8_t writeAddress, int8_t writeByte)
{
    /* Set master to transmit mode PL */
    I2C_setMode(I2C_MODULE, EUSCI_B_I2C_TRANSMIT_MODE);

    /* Clear any existing interrupt flag PL */
    I2C_clearInterruptFlag(I2C_MODULE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    /* Wait until ready to write PL */
    while (I2C_isBusBusy(I2C_MODULE));

    /* Initiate start and send first character */
    I2C_masterSendMultiByteStart(I2C_MODULE, writeAddress);

    /* Send the MSB to SENSOR */
    I2C_masterSendMultiByteNext(I2C_MODULE, (unsigned char)(writeByte>>8));
    I2C_masterSendMultiByteFinish(I2C_MODULE, (unsigned char)(writeByte&0xFF));

    printf("reset MPU6050!\n");
}

void I2C_write16 (int8_t writeAddress, int16_t writeBytes)
{
    /* Set master to transmit mode PL */
    I2C_setMode(I2C_MODULE, EUSCI_B_I2C_TRANSMIT_MODE);

    /* Clear any existing interrupt flag PL */
    I2C_clearInterruptFlag(I2C_MODULE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    /* Wait until ready to write PL */
    while (I2C_isBusBusy(I2C_MODULE));

    /* Initiate start and send first character */
    I2C_masterSendMultiByteStart(I2C_MODULE, writeAddress);

    /* Send the MSB to SENSOR */
    I2C_masterSendMultiByteNext(I2C_MODULE, (unsigned char)(writeBytes>>8));
    I2C_masterSendMultiByteFinish(I2C_MODULE, (unsigned char)(writeBytes&0xFF));

    printf("reset MPU6050!\n");
}


