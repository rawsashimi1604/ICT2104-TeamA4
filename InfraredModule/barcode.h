
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

// INTERRUPTS
void ADC14_IRQHandler(void);

// PRIVATE FUNCTIONS
static void initTimer(void);
static void initADC(void);
static void initInterrupts(void);
static void readThickness(void);
static void mergeArray(int bars[], int spaces[], int combined[]);
static void printArray(int array[]);
static int getChar(int dec);
static int decodeSChar(int *bin);
static void breakDownBarcode(int* combined);
static void TA0_0_IRQHandler(void);

// PUBLIC FUNCTIONS
void Barcode_init(void);       // Call in main to init all the stuff for barcode
void Barcode_main_while(void); // Call in the main while loop, this func needs to keep running
int Barcode_getChar(void);     // Call this to get decoded character

#endif /* BARCODE_H_ */
