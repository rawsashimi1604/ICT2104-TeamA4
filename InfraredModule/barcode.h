
#ifndef BARCODE_H_
#define BARCODE_H_

/*************************************************************
 * INCLUDES
 */

/* DriverLib Includes */
#include "driverlib.h"

#include <stdint.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

/*************************************************************
 * FUNCTIONS
 */

// PRIVATE FUNCTIONS
void initTimer(void);
void initADC(void);
void initInterrupts(void);
void readThickness(void);
void mergeArray(int bars[], int spaces[], int combined[]);
void printArray(int array[]);
int getChar(int dec);
int decodeSChar(char *bin);
void breakDownBarcode(char *wholeBarcode);
void ADC14_IRQHandler(void);
void TA0_0_IRQHandler(void);

// PUBLIC FUNCTIONS
void Barcode_init(void);
void Barcode_main_while(void);

#endif /* BARCODE_H_ */
