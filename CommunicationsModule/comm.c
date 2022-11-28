/* DriverLib Includes */
#include <CommunicationsModule/comm.h>
#include "driverlib.h"

/* Standard Includes */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const eUSCI_UART_Config uartConfig =
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,                 // 3Mhz System Clock
    1,                                              // BRDIV = 1
    10,                                             // UCxBRF = 10
    0,                                              // UCxBRS = 0
    EUSCI_A_UART_NO_PARITY,                         // No Parity
    EUSCI_A_UART_LSB_FIRST,                         // LSB First
    EUSCI_A_UART_ONE_STOP_BIT,                      // One stop bit
    EUSCI_A_UART_MODE,                              // UART mode
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
};

void uPrintf(unsigned char * TxArray);

char g_userInput[5];
int g_inputCounter = 0;
int g_inputX, g_inputY = 0;

void Communication_init(void){
    /* Selecting P1.2 and P1.3 in UART mode */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Selecting P3.2 and P3.3 in UART mode */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* LED PIN Indicator*/
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    /* SysTick Timer*/
    SysTick_enableModule();
    SysTick_setPeriod(3000000);
    Interrupt_enableSleepOnIsrExit();
    SysTick_enableInterrupt();

    /* Configuring UART Modules */
    UART_initModule(EUSCI_A0_BASE, &uartConfig);
    UART_initModule(EUSCI_A2_BASE, &uartConfig);

    /* Enable UART modules */
    UART_enableModule(EUSCI_A0_BASE);
    UART_enableModule(EUSCI_A2_BASE);

    /* Enabling interrupts (Rx) */
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA0);
    Interrupt_enableInterrupt(INT_EUSCIA2);
    Interrupt_enableMaster();
    uPrintf("Comms init");
}

/*To print on serial terminal*/
void uPrintf(unsigned char * TxArray){
    unsigned short i = 0;
    while(*(TxArray+i))
    {
        UART_transmitData(EUSCI_A0_BASE, *(TxArray+i));  // Write the character at the location specified by pointer
        i++;                                             // Increment pointer to point to the next character
    }

}

/*Function for modules to call to send to M5*/
void Communication_sendData(unsigned short identifier, unsigned long data){
    unsigned char tmp_char_array[5];
    unsigned short i = 0;
    switch (identifier){
    case 1:
        tmp_char_array[0] = 0x53; //Speed
        break;
    case 2:
        tmp_char_array[0] = 0x48; //Hump detection
        if(data == 1){
            data = 0x736559;
        }else{
            data = 0x6F4E;
        }
        break;
    case 3:
        tmp_char_array[0] = 0x42; //Barcode encoder
        break;
    default:
        return;
    }

    tmp_char_array[4] = (data & 0xff000000) >> 24;
    tmp_char_array[3] = (data & 0x00ff0000) >> 16;
    tmp_char_array[2] = (data & 0x0000ff00) >>  8;
    tmp_char_array[1] = (data & 0x000000ff)      ;

    while(*(tmp_char_array+i)){
        UART_transmitData(EUSCI_A2_BASE, *(tmp_char_array+i));  // Write the character at the location specified by pointer
        i++;                                                    // Increment pointer to point to the next character
    }
    UART_transmitData(EUSCI_A2_BASE, 0x0a);
}

/*Function for mapping to call to send to M5*/
void Communication_sendMap(unsigned char * TxArray){
    unsigned short i = 0;
    UART_transmitData(EUSCI_A2_BASE, 0x4d);
    while(*(TxArray+i)){
        UART_transmitData(EUSCI_A2_BASE, *(TxArray+i));  // Write the character at the location specified by pointer
        i++;                                             // Increment pointer to point to the next character
    }
    UART_transmitData(EUSCI_A2_BASE, 0x0a);
}

void EUSCIA0_IRQHandler(void) //Interrupt handler for MSP432
{
    unsigned char received = 0;
    received = UART_receiveData(EUSCI_A0_BASE);
    UART_transmitData(EUSCI_A2_BASE, received);
    uPrintf(&received);
}

void EUSCIA2_IRQHandler(void) //Interrupt handler for M5
{
    unsigned char received = 0;
    char xInput[1] = "";
    char yInput[1] = "";
    received = UART_receiveData(EUSCI_A2_BASE);
    if(g_inputCounter <= 4){
        strncat(g_userInput, &received, 1);
        g_inputCounter ++;
    }else{
        g_inputCounter = 0;
        strncat(xInput,&g_userInput[1],1);
        strncat(yInput,&g_userInput[3],1);
        g_inputX = atoi(xInput);
        g_inputY = atoi(yInput);
    }
    uPrintf(&received);
}

void SysTick_Handler(void) //Toggle LED every 1s
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
