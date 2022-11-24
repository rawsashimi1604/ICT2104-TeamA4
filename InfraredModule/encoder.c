#include "encoder.h"

volatile int TIME_MS = 0;
volatile int notchCounting = 0;
volatile int notchCounter;

typedef struct EncoderStruct
{
    volatile int notchesdetected;
    volatile int notchCounter;
    volatile int pulses_p_min;
    volatile int prevRPM;
    volatile int newRPM;
    volatile int last_pulse_interval;
    volatile int start_time;
    volatile int end_time;
} Encoder;

volatile Encoder L_E = { 0 };
volatile Encoder R_E = { 0 };

volatile Encoder *L_Encoder = &L_E;
volatile Encoder *R_Encoder = &R_E;

void Infrared_startNotchesCount(void);
int Infrared_stopNotchesCount(void);
float Infrared_getCarSpeed(void);

void detectPulse(volatile Encoder *e);
void updateRPM(volatile Encoder *e);

void initTimer(void)
{
    WDT_A_holdTimer();

    // Set up Timer_A2, used for interrupt count check, (1MHz clock).
    const Timer_A_UpModeConfig upConfig =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_3,          // SMCLK/3 = 1MHz
        TIMER_A_TICKPERIOD,                     // 1000 tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,     // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
    };

    Timer_A_configureUpMode(TIMER_A2_BASE, &upConfig);
    Timer_A_clearTimer(TIMER_A2_BASE);
}

void initPins(void)
{
    // Left Encoder & Right Encoder
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5|GPIO_PIN7);

    // Select edge that triggers the interrupt
    GPIO_interruptEdgeSelect(GPIO_PORT_P3, GPIO_PIN5|GPIO_PIN7,
                             GPIO_LOW_TO_HIGH_TRANSITION);

}

void initInterrupts(void)
{
    // Clear pin's interrupt flag for P3.5 & P3.7
    GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5|GPIO_PIN7);

    // Enable interrupt bit of P3.5 & P3.7
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5|GPIO_PIN7);

    // Set interrupt enable (IE) bit of corresponding interrupt source
    Interrupt_enableInterrupt(INT_TA2_0);
    Interrupt_enableInterrupt(INT_PORT3);
}

void Encoder_init(void)
{
    initTimer();
    initPins();
    initInterrupts();
}

void Encoder_main(void) // Call once in main
{
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);
}

void PORT3_IRQHandler(void)
{
    uint32_t status;

    status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);

    if (status & GPIO_PIN5) // P3.5 interrupt
    {
        L_Encoder->notchesdetected++;
        if (notchCounting)
        {
            L_Encoder->notchCounter++;
        }
    }

    if (status & GPIO_PIN7) // P3.7 interrupt
    {
        R_Encoder->notchesdetected++;
        if (notchCounting)
        {
            R_Encoder->notchCounter++;
        }
    }

    detectPulse(L_Encoder);
    detectPulse(R_Encoder);

    GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5 | GPIO_PIN7);
}

void TA2_0_IRQHandler(void) // interrupts every 0.001 seconds
{
    TIME_MS += 1;

    // print carspeed
    if (TIME_MS % 500 == 0)
    {
        Infrared_getCarSpeed();
    }

    // calculate RPM
    if (TIME_MS % 1000 == 0)
    {
        updateRPM(L_Encoder);
        updateRPM(R_Encoder);
    }

    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,
    TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

void Infrared_startNotchesCount(void)
{
    if (!notchCounting)
    {
        notchCounting = 1;
    }
}

int Infrared_stopNotchesCount(void)
{
    int results;
    if (notchCounting)
    {
        notchCounting = 0;
        results = (L_Encoder->notchCounter + R_Encoder->notchCounter) / 2;
        L_Encoder->notchCounter = 0;
        R_Encoder->notchCounter = 0;
    }
    return results;
}

float Infrared_getCarSpeed(void)
{
    float cmPerSecond = ((L_Encoder->newRPM + R_Encoder->newRPM) / 2) * 22
            / 60.0;
    return cmPerSecond;
}

void detectPulse(volatile Encoder *encoder)
{
    // this is 1 pulse
    if (encoder->notchesdetected == 4)
    { // hit 5 time = 1 revolution
        encoder->end_time = TIME_MS;
        encoder->last_pulse_interval = encoder->end_time - encoder->start_time;
        encoder->start_time = encoder->end_time; // basically means the start time for next pulse
        encoder->notchesdetected = 0;
    }
}

void updateRPM(volatile Encoder *encoder)
{
    // if last pulse was 2 seconds ago
    if ((TIME_MS - encoder->start_time) > 2000)
    {
        encoder->pulses_p_min = 0;
        encoder->newRPM = 0;
    }
    else
    {
        // calculate pulse/min by taking intervals between pulses
        encoder->pulses_p_min = 60000 / encoder->last_pulse_interval;

        // calculate new RPM as every 4 pulse is 1 revolution
        encoder->newRPM = encoder->pulses_p_min / 5;
    }

    // update prevRPM if different
    if (encoder->newRPM != encoder->prevRPM)
    {
        encoder->prevRPM = encoder->newRPM;
    }
}