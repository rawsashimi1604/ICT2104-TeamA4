
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "barcode.h"

/* Statics */
static volatile uint16_t curADCResult;

/* Variables for barcode reading */
volatile int max;
volatile int max2; // placeholder to deconflict max reading when in track and not barcode
volatile int min = 20000;
volatile int threshold;

volatile int avCounter = 1;
volatile int avCulmulative;
volatile int average;

volatile int barCodeFound = 0;
volatile int startTimer = 1;
volatile int lookingForBlack = 1;

volatile int widthCounter;
int barCodesScanned = 0;
int readThicknessFlag = 0;

int indexBar = 0;
int indexSpace = 0;
int maxBar = 0;
int maxSpace = 0;

int bars[16] = {0};
int spaces[15] = {0};
int combined[31] = {0};
int currentInt;
int currentascii;

int firstChar[10];
int secondChar[9];
int thirdChar[10];

void readThickness(void);
void mergeArray(int[], int[], int[]);
void printArray(int[]);

void initTimer(void)
{
    Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK, // SMCLK Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        50,                                 // SMCLK /1 = 3MHz, 750 tick period means interrupt every 0.000015625s
        TIMER_A_TAIE_INTERRUPT_DISABLE,     // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                    // Clear value
    };

    // Set timer as UP mode
    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
}

void initADC(void)
{
    /* Setting Flash wait state */
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
    /* Setting DCO to 48MHz  */
    MAP_PCM_setPowerState(PCM_AM_LDO_VCORE1);
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    /* Enabling the FPU for floating point operation */
    MAP_FPU_enableModule();
    MAP_FPU_enableLazyStacking();
    /* Initializing ADC (MCLK/1/4) */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_4,
                         0);
    /* Configuring P1.0 as output */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    /* Configuring GPIOs (5.5 A0) */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5,
                                                   GPIO_TERTIARY_MODULE_FUNCTION);
    /* Configuring ADC Memory */
    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                        ADC_INPUT_A0, false);
    /* Configuring Sample Timer */
    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
    /* Enabling/Toggling Conversion */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();
}

void initInterrupts(void)
{
    /* Enabling interrupts */
    MAP_ADC14_enableInterrupt(ADC_INT0);
    MAP_Interrupt_enableInterrupt(INT_ADC14);

    /* Enabling timer interrupt */
    Interrupt_enableInterrupt(INT_TA0_0);
}

/* Timer Interrupt Handler*/
void TA0_0_IRQHandler(void) // This is basically polling every 250us via an
                            // interrupt from the timer lol
{
    if ((indexBar + indexSpace) ==
        31)
    { // barcode fully read, stop timer, clear timer and clear isr flag

        // printf("TIMER STOPPED\n");
        barCodesScanned++;
        readThicknessFlag = 1;


        // send to comms here(idk how???)

        // reset the values for the next barcode
//        max = 0;
//        max2 = 0;
//        min = 20000;
        indexBar = 0;
        indexSpace = 0;
        lookingForBlack = 1;
        startTimer = 1;
//        barCodeFound = 0;
        // what else?

        Timer_A_stopTimer(TIMER_A0_BASE);
        Timer_A_clearTimer(TIMER_A0_BASE);
        Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
                                             TIMER_A_CAPTURECOMPARE_REGISTER_0);
    }

    if (lookingForBlack == 1)
    { // scanning black
        if (average < threshold)
        {
            bars[indexBar] = widthCounter;
            if (widthCounter > maxBar)
            {
                maxBar = widthCounter;
            }
            if (indexBar < 16)
            {
                indexBar++;
            }
            widthCounter = 0;
            lookingForBlack = 0;
        }
        else
        {
            widthCounter++;
        }
    }
    else if (lookingForBlack == 0)
    { // scanning white
        if (average > threshold)
        {
            spaces[indexSpace] = widthCounter;
            if (widthCounter > maxSpace)
            {
                maxSpace = widthCounter;
            }
            if (indexSpace < 15)
            {
                indexSpace++;
            }
            widthCounter = 0;
            lookingForBlack = 1;
        }
        else
        {
            widthCounter++;
        }
    }

    /* Clear interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
                                         TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

void Barcode_init(void)
{
    initTimer();
    initADC();
    initInterrupts();
}

void Barcode_main_while(void) // Need to Keep calling this in the main file while(1)
{
    if ((max / min) < 1.5f)
    {
        // meaning it still on the way to find the barcode
    }
    else
    {
        barCodeFound = 1;
//        ADC14_toggleConversionTrigger();
        printf("READING BARCODE\n");
        if (startTimer == 1)
        {
            startTimer = 0;
            max = 0;
//            max2 = 0;
            min = 20000;
            //               threshold =0;
            Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE); // start timer
        }
        //          lookingForBlack = 1;
    }

    if (readThicknessFlag)
    {
//        ADC14_disableConversion();
        // printf("ADC STOPPED\n");
        // printf("TRANSLATING THICCNESS\n");
        readThickness();
        // printf("MERGING ARRAYS\n");
        mergeArray(bars, spaces, combined);
        readThicknessFlag = 0;
        // decode part
        breakDownBarcode(combined);
        printArray(combined);
        currentInt = decodeSChar(secondChar);
        currentascii = getChar(currentInt);
        min = 20000;
        max = 0;
        // printf("RESULT = \n");
//        printArray(combined);
    }
    // if whiteCounter exceeds certain threshold reset everything?
}

int Barcode_getChar(void)
{
    return currentascii;
}

/* ADC Interrupt Handler. This handler is called whenever there is a conversion
 * that is finished for ADC_MEM0. */
void ADC14_IRQHandler(void)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    if (ADC_INT0 & status)
    {

        curADCResult = MAP_ADC14_getResult(ADC_MEM0);

        if (curADCResult > max)
        {
            max = curADCResult;
        }
        if (curADCResult < min)
        {
            min = curADCResult;
        }
        threshold = (max - min) / 2; // midpoint of readings

        if (avCounter < 180) // return average of past 180 readings
        {
            avCulmulative += curADCResult;
            average = avCulmulative / avCounter;
            avCounter++;
        }
        else if (avCounter == 180)
        {
            avCulmulative = 0;
            avCounter = 1;
        }

        MAP_ADC14_toggleConversionTrigger();
    }
}

void readThickness(void)
{
    int i, j = 0;
    int average2 = 0;
    for (i = 0; i < 15; i++)
    {
        average2 += spaces[i];
        average2 += bars[i];
    }
    average2 += bars[15];
    average2 /= 31;
    for (i = 0; i < 16; i++)
    {
        //      printf("%d",bars)
        if (bars[i] > average2)
        {
            bars[i] = 1;
        }
        else
        {
            bars[i] = 0;
        }
    }
    for (j = 0; j < 15; j++)
    {
        if (spaces[j] > average2)
        {
            spaces[j] = 1;
        }
        else
        {
            spaces[j] = 0;
        }
    }
}

void mergeArray(int bars[], int spaces[], int combined[])
{
    int i = 0, k = 0, l = 0;
    // now we have a string which can be converted in to a single ascii value
    for (i = 0; i < 30; i++)
    {
        if (i % 2 == 0)
        {
            //      sprintf(&sChar[i], "%d", (char)bars[l]);
            combined[i] = bars[l];
            l++;
        }
        else
        {
            //      sprintf(&sChar[i], "%d", (char)spaces[k]);
            combined[i] = spaces[k];
            k++;
        }
    }
    // next step is to to convert the array into one single value
}

void printArray(int array[])
{
    int i = 0;
    for (i = 0; i < 31; i++)
    {
        printf("%d", array[i]);
    }
    printf("\n");
}

// retrun an ascii char in decimal by comparing the decoded character
int getChar(int dec)
{
    switch (dec)
    {
    case 289:
        return 49;
        break;
    case 97:
        return 50;
        break;
    case 352:
        return 51;
        break;
    case 49:
        return 52;
        break;
    case 304:
        return 53;
        break;
    case 112:
        return 54;
        break;
    case 37:
        return 55;
        break;
    case 292:
        return 56;
        break;
    case 100:
        return 57;
        break;
    case 52:
        return 48;
        break;
    case 265:
        return 65;
        break;
    case 73:
        return 66;
        break;
    case 328:
        return 67;
        break;
    case 25:
        return 68;
        break;
    case 280:
        return 69;
        break;
    case 88:
        return 70;
        break;
    case 13:
        return 71;
        break;
    case 268:
        return 72;
        break;
    case 76:
        return 73;
        break;
    case 28:
        return 74;
        break;
    case 259:
        return 75;
        break;
    case 67:
        return 76;
        break;
    case 322:
        return 77;
        break;
    case 19:
        return 78;
        break;
    case 274:
        return 79;
        break;
    case 82:
        return 80;
        break;
    case 7:
        return 81;
        break;
    case 262:
        return 82;
        break;
    case 70:
        return 83;
        break;
    case 22:
        return 84;
        break;
    case 385:
        return 85;
        break;
    case 193:
        return 86;
        break;
    case 448:
        return 87;
        break;
    case 145:
        return 88;
        break;
    case 400:
        return 89;
        break;
    case 208:
        return 90;
        break;
    case 133:
        return 45;
        break;
    case 388:
        return 46;
        break;
    case 196:
        return 32;
        break;
    case 148:
        return 42;
        break;
    case 168:
        return 36;
        break;
    case 162:
        return 47;
        break;
    case 138:
        return 43;
        break;
    case 48:
        return 37;
        break;
    default:
        return -1;
        break;
    }
    return 0;
}

// convert the array value into int
int decodeSChar(int *bin)
{
//    long binLength = strlen(bin);
    double dec = 0;
    int i = 0;
    for (i = 0; i < 9; i++)
    {
        dec += (bin[i]) * pow(2, ((9 - i) - 1));
    }
    // if the dec within the letter range, add the offset, else check the symbol
    // one by one
    //    if((int)dec>){
    //
    //    }

    return (int)dec;
}

// split the merged barcode into three different arrays.
void breakDownBarcode(int* combined)
{
    printf("run -1\n");
//    long barLength = strlen((char*)combined);
//    printf("%d\n",barLength);
    int i = 0;
    for (i = 10; i < 19; i++)
    {
//        printf("run 0");
        if (i < 10)
        {
            firstChar[i - 1] = combined[i];
        }
        else if (i >= 10 && i < 19)
        {
//            printf("run 2\n");
//            printf("%d", combined[i]);
            secondChar[i - 10] = combined[i];
        }
        else if (i >= 21 && i < 30)
        {
            thirdChar[i - 21] = combined[i];
        }
    }
}

//  int main(void)
//  {
//      Barcode_init();
//      while (1)
//      {
//          Barcode_main_while();
//          //    MAP_PCM_gotoLPM0(); // if i dont get to sleep, this bitch ass MSP
//          //    doesnt get to either
//      }
//  }
