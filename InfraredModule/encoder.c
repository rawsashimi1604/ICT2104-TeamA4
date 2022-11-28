/** @file module.c
 *
 * @brief This module is able to
 * 1) calculate linear velocity of the car
 * 2) calculate linear distance travelled by the car
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2018 Barr Group. All rights reserved.
 */

/* DriverLib Includes */
#include "driverlib.h"

// for MAP_ prefix functions
#include "rom_map.h"
#include "encoder.h"

#include "../MotorModule/motor.h"

#define PORT          GPIO_PORT_P5
#define LEFT_ENCODER  GPIO_PIN5
#define RIGHT_ENCODER GPIO_PIN4

extern Car              car;
extern volatile int16_t g_counter;
extern volatile int16_t g_left_counter;
extern volatile int16_t g_right_counter;

volatile int g_time_ms          = 0;
volatile int g_b_notch_counting = 0;

volatile Encoder left_enc_obj  = { 0 };
volatile Encoder right_enc_obj = { 0 };

volatile Encoder *p_left_encoder  = &left_enc_obj;
volatile Encoder *p_right_encoder = &right_enc_obj;

/*!
 * @brief Initialise TIMER_A2_BASE
 */
void
initTimer(void)
{
    WDT_A_holdTimer();

    // Set up Timer_A2, used for interrupt count check, (1MHz clock).
    const Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_3,      // SMCLK/3 = 1MHz
        TIMER_A_TICKPERIOD,                 // 1000 tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,     // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                    // Clear value
    };

    Timer_A_configureUpMode(TIMER_A2_BASE, &upConfig);
    Timer_A_clearTimer(TIMER_A2_BASE);
}

/*!
 * @brief Initialise pins P5.5, P5.4
 */
void
initPins(void)
{
    // Left Encoder & Right Encoder
    GPIO_setAsInputPinWithPullUpResistor(PORT, LEFT_ENCODER | RIGHT_ENCODER);

    // Select edge that triggers the interrupt
    GPIO_interruptEdgeSelect(
        PORT, LEFT_ENCODER | RIGHT_ENCODER, GPIO_LOW_TO_HIGH_TRANSITION);
}

/*!
 * @brief Initialise interrupt for P5.5, P5.4 and TIMER_A2_BASE
 */
void
initInterrupts(void)
{
    // Clear pin's interrupt flag for P5.5, P5.4
    GPIO_clearInterruptFlag(PORT, LEFT_ENCODER | RIGHT_ENCODER);

    // Enable interrupt bit of P5.5, P5.4
    GPIO_enableInterrupt(PORT, LEFT_ENCODER | RIGHT_ENCODER);

    // Set interrupt enable (IE) bit of corresponding interrupt source
    Interrupt_enableInterrupt(INT_TA2_0);
    Interrupt_enableInterrupt(INT_PORT5);
}

void
Encoder_init(void)
{
    initTimer();
    initPins();
    initInterrupts();
}

void
Encoder_main(void) // Call once in main
{
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);
}

/*!
 * @brief Handle Port 5 interrupts.
 * If P5.5 interrupts,
 * a) counts notches for left encoder
 * b) calculates time taken for one pulse
 *
 * If P5.4 interrupts,
 * a) counts notches for right encoder
 * b) calculates time taken for one pulse
 *
 */
void
PORT5_IRQHandler(void)
{
    uint32_t status;

    status = GPIO_getEnabledInterruptStatus(PORT);

    if (status & LEFT_ENCODER) // P5.5 interrupt
    {
        p_left_encoder->notches_detected++;
        if (g_b_notch_counting)
        {
            p_left_encoder->notch_counter++;
        }
    }

    if (status & RIGHT_ENCODER) // P5.4 interrupt
    {
        p_right_encoder->notches_detected++;
        if (g_b_notch_counting)
        {
            p_right_encoder->notch_counter++;
        }
    }

    detectPulse(p_left_encoder);
    detectPulse(p_right_encoder);

    if (status & LEFT_ENCODER)
    {
        g_left_counter++;
        car.right_wheel_count++;
    }
    if (status & RIGHT_ENCODER)
    {
        g_right_counter++;
        car.left_wheel_count++;
    }

    // Take average of the left and right wheel count
    g_counter = (car.left_wheel_count + car.right_wheel_count) / 2;

    GPIO_clearInterruptFlag(PORT, LEFT_ENCODER | RIGHT_ENCODER);
}

/*!
 * @brief timer that interrupts every 1 milliseconds
 * a) updates RPM for left and right encoder every second
 */
// void
// TA2_0_IRQHandler(void)
//{
//     g_time_ms += 1;
//
//     if (g_time_ms % 1000 == 0)
//     {
//         updateRPM(p_left_encoder);
//         updateRPM(p_right_encoder);
//     }
//
//     Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,
//                                          TIMER_A_CAPTURECOMPARE_REGISTER_0);
// }

/*!
 * @brief Start counting notches
 */
void
Infrared_startNotchesCount(void)
{
    if (!g_b_notch_counting)
    {
        g_b_notch_counting = 1;
    }
}

/*!
 * @brief Stop counting notches
 * @returns number of notches since notch counting starts
 */
int
Infrared_stopNotchesCount(void)
{
    int results;
    if (g_b_notch_counting)
    {
        g_b_notch_counting = 0;
        results
            = (p_left_encoder->notch_counter + p_right_encoder->notch_counter)
              / 2;
        p_left_encoder->notch_counter  = 0;
        p_right_encoder->notch_counter = 0;
    }
    return results;
}

/*!
 * @brief Calculates the average linear velocity of car
 * a) using Hall Effect Sensor from updateRPM()
 * b) average RPM of both encoders
 *
 * @returns average linear velocity of car in cmPerSecond
 */
float
Infrared_getCarSpeed(void)
{
    float cmPerSecond
        = ((p_left_encoder->new_RPM + p_right_encoder->new_RPM) / 2) * 22
          / 60.0;
    return cmPerSecond;
}

/*!
 * @brief detects one pulses, where one pulse is 4 notches.
 * One revolution (20 notches) has 5 pulses.
 */
void
detectPulse(volatile Encoder *encoder)
{
    // this is 1 pulse
    if (encoder->notches_detected == 4)
    {
        encoder->end_time            = g_time_ms;
        encoder->last_pulse_interval = encoder->end_time - encoder->start_time;
        encoder->start_time          = encoder->end_time;
        encoder->notches_detected    = 0;
    }
}

/*!
 * @brief updates RPM of each encoder using Hall Effect Sensor
 * a) if start time of last pulse is more than 2 seconds ago, then encoder
 * stopped moving
 * b) calculates RPM using intervals between pulses
 */
void
updateRPM(volatile Encoder *encoder)
{
    // if last pulse was 2 seconds ago
    if ((g_time_ms - encoder->start_time) > 2000)
    {
        encoder->pulses_p_min = 0;
        encoder->new_RPM      = 0;
    }
    else
    {
        // calculate pulse/min by taking intervals between pulses
        encoder->pulses_p_min = 60000 / encoder->last_pulse_interval;

        // calculate new RPM as every 4 pulse is 1 revolution
        encoder->new_RPM = encoder->pulses_p_min / 5;
    }

    // update prev_RPM if different
    if (encoder->new_RPM != encoder->prev_RPM)
    {
        encoder->prev_RPM = encoder->new_RPM;
    }
}

/*** end of file ***/
