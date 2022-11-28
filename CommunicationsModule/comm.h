#ifndef COMM_H_
#define COMM_H_
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Public
void Communication_init(void);
void Communication_sendData(unsigned short identifier, unsigned long data);
void Communication_sendMap(unsigned char * TxArray);

// Private
void uPrintf(unsigned char * TxArray);

// Interrupts
void EUSCIA0_IRQHandler(void);
void EUSCIA2_IRQHandler(void);
void SysTick_Handler(void);

#endif /* COMM_H_ */
