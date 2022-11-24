
#include "motor.h"
#include <stdint.h>
#include "wheels.h"

void delay(uint32_t);
float findMin(float, float);
float findMax(float, float);
static void Set_PID(void);
static void Check_revolution(void);

static const uint8_t TARGET = 8;

static const uint16_t DEFAULT_DC = 30000; // 100% Duty Cycle

const uint8_t TURN90_TICKS = 10;  // Number of ticks required to turn
const uint8_t TURN45_TICKS = 5;  // Number of ticks required to turn
const uint8_t STRAIGHT_TICKS = 30; // Number of ticks required to move 1 grid
const uint8_t STATION_TURN = 11;

static const float KP = 0.03;
static const float KD = 0.015;
static const float KI = 0.006;

volatile static int16_t g_counter = 0;
volatile static int16_t g_left_counter = 0;
volatile static int16_t g_right_counter = 0;
volatile static int16_t g_prev_error_left = 0;
volatile static int16_t g_prev_error_right = 0;
volatile static int16_t g_sum_error_left = 0;
volatile static int16_t g_sum_error_right = 0;

volatile static float g_pid_left = 1;
volatile static float g_pid_right = 1;

static void Motor_start(int,int);
Car car =
{
    .direction = FORWARD,
    .stop = false,
    .detected_obstacle = false,
    .is_moving = false,
    .left_dc = 0,
    .right_dc = 0,
    .left_wheel_count = 0,
    .right_wheel_count = 0
};

Timer_A_PWMConfig left_wheel_PWM_config =
{
    TIMER_A_CLOCKSOURCE_SMCLK,         // SMCLK Clock Source
    TIMER_A_CLOCKSOURCE_DIVIDER_64,    // SMCLK / 64 = 38kHz, period = 1/38khz = 0.026ns
    30000,                             // Tick Period
    TIMER_A_CAPTURECOMPARE_REGISTER_2, // CCR2
    TIMER_A_OUTPUTMODE_RESET_SET,      // Compare Output Mode
    0                                  // Duty Cycle
};

Timer_A_PWMConfig right_wheel_PWM_config =
{
    TIMER_A_CLOCKSOURCE_SMCLK,
    TIMER_A_CLOCKSOURCE_DIVIDER_64,
    30000,
    TIMER_A_CAPTURECOMPARE_REGISTER_1,
    TIMER_A_OUTPUTMODE_RESET_SET,
    0
};

// 1 Sec interrupt
Timer_A_UpModeConfig up_config =
{
    TIMER_A_CLOCKSOURCE_ACLK,           // ACLK Clock Source
    TIMER_A_CLOCKSOURCE_DIVIDER_3,      // ACLK/3 = 1MHz
    10000,                              // 10000 tick period
    TIMER_A_TAIE_INTERRUPT_DISABLE,     // Disable Timer interrupt
    TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, // Enable CCR0 interrupt
    TIMER_A_DO_CLEAR                    // Clear value
};

/*
 * Initialize all the pins used by the L298N.
 */
void Motor_init(void)
{
    Wheel_init();

    /* PWM port for right motor */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
                                                GPIO_PRIMARY_MODULE_FUNCTION); // Configuring the peripheral module function in the output direction for P2.4 for primary module function modes
    /* PWM port for left motor */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5,
                                                GPIO_PRIMARY_MODULE_FUNCTION); // Configuring the peripheral module function in the output direction for P2.5 for primary module function mod
    // Configuring Timer_A3 for Up Mode
    MAP_Timer_A_configureUpMode(TIMER_A3_BASE, &up_config);
    // Enabling interrupts and starting the timer
    MAP_Interrupt_enableInterrupt(INT_TA3_0);
    MAP_Timer_A_clearTimer(TIMER_A3_BASE);

    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

/*
Set the duty cycles of the left and right wheels
and generate the PWM signal.
*/
static void generatePWM(void)
{
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &left_wheel_PWM_config);
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &right_wheel_PWM_config);
}

/*
 * Set duty cycle of left and right wheels.
 *
 * Starts Timer A3 to trigger ISR more accurately.
 */
static void Motor_start(int left_duty_cycle, int right_duty_cycle)
{
    left_wheel_PWM_config.dutyCycle = left_duty_cycle;
    right_wheel_PWM_config.dutyCycle = right_duty_cycle;
    generatePWM();

    car.stop = false;
    car.is_moving = true;
    g_left_counter = 0;
    g_right_counter = 0;

    MAP_Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
}

/*
Stops the car.
Check if motor is moving and set
the duty cycle of the left and right wheels to 0.
Stop and clear timer A3 to prevent setting the PID
before reaching a full count when the motor starts.
*/
void Motor_stop(void)
{
//    if (left_wheel_PWM_config.dutyCycle != 0 || right_wheel_PWM_config.dutyCycle != 0)
//    {
        left_wheel_PWM_config.dutyCycle = 0;
        right_wheel_PWM_config.dutyCycle = 0;
        generatePWM();
        Set_stop();
        g_counter = 0;
        car.stop = true;
        car.is_moving = false;
        car.left_wheel_count = 0;
        car.right_wheel_count = 0;
        g_left_counter = 0;
        g_right_counter = 0;

        MAP_Timer_A_clearTimer(TIMER_A3_BASE);
//    }
}

void Motor_driveForward(){
    car.direction = FORWARD;
    Set_forward();
    Motor_start(DEFAULT_DC, DEFAULT_DC);
}

void Motor_reverse(){
    car.direction = BACKWARD;
    Set_reverse();
    Motor_start(DEFAULT_DC, DEFAULT_DC);
}

void Motor_pivotLeft(){
    car.direction = LEFT;
    Set_forward();
    Motor_start(0, DEFAULT_DC);
}

void Motor_pivotRight(){
    car.direction = RIGHT;
    Set_forward();
    Motor_start(DEFAULT_DC, 0);
}

void Motor_turnLeft(){
    car.direction = S_LEFT;
    Set_leftBackRightFront();
    Motor_start(DEFAULT_DC, DEFAULT_DC);
}

void Motor_turnRight(){
    car.direction = S_RIGHT;
    Set_leftFrontRightBack();
    Motor_start(DEFAULT_DC, DEFAULT_DC);
}

/*
 * Set set duty cycles of right wheel to turn car left.
 */
void Motor_turn45Left(void)
{
    car.direction = LEFT45;
    Set_forward();
    Motor_start(0, DEFAULT_DC);
}

/**
 * Set duty cycles of left wheel to turn car right.
 */
void Motor_turn45Right(void)
{
    car.direction = RIGHT45;
    Set_forward();
    Motor_start(DEFAULT_DC, 0);
}
/**
 * Set the PID of the car to the desired value.
 *
 * PID values are kept between 0 and 1.
 * The value of the PID is the percentage of the duty cycle.
 */
static void Set_PID(void)
{
    int16_t error_left = TARGET - g_left_counter;
    int16_t error_right = TARGET - g_right_counter;

    g_pid_left += (error_left * KP) + (g_prev_error_left * KD) + (g_sum_error_left * KI);
    g_pid_right += (error_right * KP) + (g_prev_error_right * KD) + (g_sum_error_right * KI);

    g_pid_left = findMax(findMin(g_pid_left, 1), 0);
    g_pid_right = findMax(findMin(g_pid_right, 1), 0);

    uint16_t left_duty_cycle = DEFAULT_DC * g_pid_left;
    uint16_t right_duty_cycle = DEFAULT_DC * g_pid_right;

    left_wheel_PWM_config.dutyCycle = left_duty_cycle;
    right_wheel_PWM_config.dutyCycle = right_duty_cycle;
    generatePWM();

    printf("| %d | %d | %d | %d | \n",error_left, error_right, left_duty_cycle, right_duty_cycle);

    g_left_counter = 0;
    g_right_counter = 0;

    g_prev_error_left = error_left;
    g_prev_error_right = error_right;

    g_sum_error_left += error_left;
    g_sum_error_right += error_right;
}

/*
 * Check how many revolutions the wheel has made.
 */
static void Check_revolution(void)
{
    uint16_t target_counter = STRAIGHT_TICKS;

    if (car.direction == LEFT || car.direction == RIGHT)
        target_counter = TURN90_TICKS;

    if (car.direction == LEFT45 || car.direction == RIGHT45)
        target_counter = TURN45_TICKS;

    if (car.direction == S_RIGHT || car.direction == S_LEFT)
            target_counter = STATION_TURN;

    if (g_counter >= target_counter && car.direction != STRAIGHT)
        Motor_stop();
}

/*
Port5 ISR
Increment the number of revolutions when the wheel encoder is rotated
and stop the motor after set number of revolutions.
 */
void PORT5_IRQHandler(void)
{
    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    if (status & GPIO_PIN5)
    {
        g_left_counter++;
        car.right_wheel_count++;
    }
    if (status & GPIO_PIN4)
    {
        g_right_counter++;
        car.left_wheel_count++;
    }

    // Take average of the left and right wheel count
    g_counter = (car.left_wheel_count + car.right_wheel_count) / 2;

    Check_revolution();
}

/*
 * Timer A3 ISR
 *
 * Set PID values every second only if
 * car is moving and direction is either forward
 * or backwards.
 */
void TA3_0_IRQHandler(void)
{
    if (car.is_moving && (FORWARD == car.direction || BACKWARD == car.direction || STRAIGHT == car.direction))
        Set_PID();

    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

/**
 * Simple delay function using for loop.
 */
void delay(uint32_t loop)
{
    volatile uint32_t index;

    for (index = 0; index < loop; index++);
}

/**
 * Get the smaller number between two numbers.
 */
float findMin(float a, float b)
{
    return a < b ? a : b;
}

/**
 * Get the larger number between two numbers.
 */
float findMax(float a, float b)
{
    return a > b ? a : b;
}
