#include "encoder.h"

volatile int TIME_MS = 0;
volatile int notchesdetected = 0;
volatile int pulses_p_min = 0;
volatile int prevRPM = 0;
volatile int newRPM = 0;
volatile int last_rev_interval = 0;
volatile int start_time = 0;
volatile int end_time = 0;

volatile int notchCounting = 0;
volatile int notchCounter;

void Encoder_startNotchesCount(void);
int Encoder_stopNotchesCount(void);

void initTimer(void)
{
    /* Timer_A UpMode Configuration Parameter */
    const Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_20,     // SMCLK/20 = 3MHz / 20 = 150000Hz
        TIMER_PERIOD,                       // 150 tick period, every tick 0.001s
        TIMER_A_TAIE_INTERRUPT_DISABLE,     // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                    // Clear value
    };

    // Timer Config
    Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);
}

void initPins(void)
{
    // Sensor
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN5);

    // Select edge that triggers the interrupt
    GPIO_interruptEdgeSelect(GPIO_PORT_P2, GPIO_PIN5,
                             GPIO_LOW_TO_HIGH_TRANSITION);
}

void initInterrupts(void)
{
    // Clear pin's interrupt flag for P2.5
    GPIO_clearInterruptFlag(GPIO_PORT_P2, GPIO_PIN5);

    // Enable interrupt bit of the specific GPIO pin
    GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN5);

    // Set interrupt enable (IE) bit of corresponding interrupt source
    Interrupt_enableInterrupt(INT_TA1_0);
    Interrupt_enableInterrupt(INT_PORT2);
}

void Encoder_init(void)
{
    initTimer();
    initPins();
    initInterrupts();
}

void Encoder_main(void) // Call once in main
{
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
}

void PORT2_IRQHandler(void)
{
    uint32_t status;

    status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);

    if (status)
    {
        notchesdetected++;
                if (notchCounting)
        {
            notchCounter++;
        }
    }

    // this is 1 pulse
    if (notchesdetected == 4)
    { // hit 5 time = 1 revolution
        end_time = TIME_MS;
        last_rev_interval = end_time - start_time;
        start_time = end_time; // basically means the start time for next pulse
        printf("1 pulse took %d ms\n", last_rev_interval);
        notchesdetected = 0;
    }

    GPIO_clearInterruptFlag(GPIO_PORT_P2, GPIO_PIN5);
}

// only prints speed for now
void getSpeed(void)
{
    float meterPerSecond = newRPM * 0.22 / 60.0;
    printf("RPM: %d\n", newRPM);
    printf("\t Linear Speed = %.2f m/s\n", meterPerSecond);
    // TODO:
    //  return whatever unit they want
}

void Encoder_startNotchesCount(void)
{
    if (!notchCounting)
    {
        notchCounting = 1;
    }
}

int Encoder_stopNotchesCount(void)
{
    if (notchCounting)
    {
        notchCounting = 0;
        int results = notchCounter;
        notchCounter = 0;
        return results;
    }
}

void TA1_0_IRQHandler(void) // interrupts every 0.001 seconds
{
    TIME_MS += 1;

    if (TIME_MS % 500 == 0)
    {
        float meterPerSecond = newRPM * 0.22 / 60.0;
        printf("RPM: %d\n", newRPM);
        printf("\t Linear Speed = %.2f m/s\n", meterPerSecond);
    }

    // calculate RPM
    if (TIME_MS % 1000 == 0)
    {
        if ((TIME_MS - start_time) > 2000)
        {
            pulses_p_min = 0;
            newRPM = 0;
        }
        else
        {
            pulses_p_min = 60000 / last_rev_interval;
            newRPM = pulses_p_min / 5;
        }
        if (newRPM != prevRPM)
        {
            prevRPM = newRPM;
            printf("RPM: %d\n", newRPM);
        }
    }
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
                                         TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

// int main(void)
// {

//     while (1)
//     {
//         PCM_gotoLPM3();
//     }
// }