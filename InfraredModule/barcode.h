
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

void readThickness(void);
void mergeArray(int bars[], int spaces[], int combined[]);
void printArray(int array[]);
int getChar(int dec);
int decodeSChar(char *bin);
void breakDownBarcode(char *wholeBarcode);
void ADC14_IRQHandler(void);
void TA0_0_IRQHandler(void);

#endif /* BARCODE_H_ */
