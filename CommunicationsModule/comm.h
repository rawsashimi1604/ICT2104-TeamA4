#ifndef COMM_H_
#define COMM_H_
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
//#include <comm.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

void comms_init(void);
void uPrintf(unsigned char * TxArray);
void sendData(unsigned short identifier, unsigned long data);
void EUSCIA0_IRQHandler(void);
void EUSCIA2_IRQHandler(void);

#endif /* COMM_H_ */
