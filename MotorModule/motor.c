
#include "motor.h"
#include <stdint.h>
#include "wheels.h"

void delay(uint32_t);
float min(float, float);
float max(float, float);
static void Set_PID(void);

static void Turn_left(void);
static void Turn_right(void);
static void Turn_left45(void);
static void Turn_right45(void);
static void Station_Turn_left(void);
static void Station_Turn_right(void);
static void generatePWM(void);
static void Move_forward(void);
static void Move_backward(void);
static void Check_revolution(void);
static void Endless_forward(void);

#define LEFT 'L'
#define RIGHT 'R'
#define LEFT45 'Q'
#define RIGHT45 'P'
#define FORWARD 'F'
#define BACKWARD 'B'
#define STRAIGHT 'S'
#define S_RIGHT 'O'
#define S_LEFT 'I'

static const uint8_t COMPENSATE = 3000;
static const uint8_t TARGET = 8;         // 75% of ticks per sample
static const uint8_t TURN90_TICKS = 10;  // Number of ticks required to turn
static const uint8_t TURN45_TICKS = 5;  // Number of ticks required to turn
static const uint8_t STRAIGHT_TICKS = 30; // Number of ticks required to stop the car moving straight

static const uint16_t DEFAULT_DC = 30000; // 50% Duty Cycle

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

/**
 * Initialize all the pins used by the L298N.
 */
void Motor_init(void)
{

    /* Right motor GPIO */
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN1); // Configuring P4.1 as output
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN2); // Configuring P4.2 as output
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1); // Set output LOW on P4.1
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2); // Set output LOW on P4.2

    /* Left motor GPIO */
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN3); // Configuring P4.3 as output
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN4); // Configuring P4.4 as output
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3); // Set output LOW on P4.3
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN4); // Set output LOW on P4.4

    /* PWM port for right motor */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4,
                                                GPIO_PRIMARY_MODULE_FUNCTION); // Configuring the peripheral module function in the output direction for P2.4 for primary module function modes

    /* PWM port for left motor */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN5,
                                                GPIO_PRIMARY_MODULE_FUNCTION); // Configuring the peripheral module function in the output direction for P2.5 for primary module function mod
    // Configuring Timer_A0 for Up Mode
    MAP_Timer_A_configureUpMode(TIMER_A3_BASE, &up_config);

    // Enabling interrupts and starting the timer
    MAP_Interrupt_enableInterrupt(INT_TA3_0);
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
    MAP_Timer_A_clearTimer(TIMER_A3_BASE);
}

/**
 * Set duty cycle of left and right wheels.
 *
 * Starts Timer A3 to trigger ISR more accurately.
 */
void Motor_start(int left_duty_cycle, int right_duty_cycle)
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
    if (left_wheel_PWM_config.dutyCycle != 0 || right_wheel_PWM_config.dutyCycle != 0)
    {
        left_wheel_PWM_config.dutyCycle = 0;
        right_wheel_PWM_config.dutyCycle = 0;
        generatePWM();

        g_counter = 0;
        car.stop = true;
        car.is_moving = false;
        car.left_wheel_count = 0;
        car.right_wheel_count = 0;
        g_left_counter = 0;
        g_right_counter = 0;

        MAP_Timer_A_stopTimer(TIMER_A3_BASE);
        MAP_Timer_A_clearTimer(TIMER_A3_BASE);
    }
}

/*
Set move car according to command direction.
*/
void Motor_direction(char command)
{
    switch (command)
    {
    case FORWARD:
        Move_forward();
        break;
    case BACKWARD:
        Move_backward();
        break;
    case LEFT:
        Turn_left();
        break;
    case RIGHT:
        Turn_right();
        break;
    case LEFT45:
        Turn_left45();
        break;
    case RIGHT45:
        Turn_right45();
            break;

    default:
        Motor_stop();
        break;
    }
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
Set duty cycles for both wheels to move forward.
*/
static void Endless_forward(void)
{
    car.direction = STRAIGHT;

    Set_forward();

    Motor_start(DEFAULT_DC, DEFAULT_DC - COMPENSATE);
}


/*
Set duty cycles for both wheels to move forward.
*/
static void Move_forward(void)
{
    car.direction = FORWARD;

    Set_forward();

    Motor_start(DEFAULT_DC, DEFAULT_DC - COMPENSATE);
}

/**
 * Reverse the direction of the left wheel and the right wheel
 * to move car backwards.
 */
static void Move_backward(void)
{
    car.direction = BACKWARD;

    Set_reverse();

    Motor_start(DEFAULT_DC, DEFAULT_DC);
}

/**
 * Set set duty cycles of right wheel to turn car left.
 */
static void Turn_left(void)
{
    car.direction = LEFT;

    Set_forward();

    Motor_start(0, DEFAULT_DC);
}

/**
 * Set duty cycles of left wheel to turn car right.
 */
static void Turn_right(void)
{
    car.direction = RIGHT;

    Set_forward();

    Motor_start(DEFAULT_DC, 0);
}
/**
 * Set set duty cycles of right wheel to turn car left.
 */
static void Station_Turn_left(void)
{
    car.direction = S_LEFT;

    set_leftBackRightFront();

    Motor_start(DEFAULT_DC, DEFAULT_DC);
}

/**
 * Set duty cycles of left wheel to turn car right.
 */
static void Station_Turn_right(void)
{
    car.direction = S_RIGHT;
    set_leftFrontRightBack();

    Motor_start(DEFAULT_DC, DEFAULT_DC);
}

/**
 * Set set duty cycles of right wheel to turn car left.
 */
static void Turn_left45(void)
{
    car.direction = LEFT45;

    Set_forward();

    Motor_start(0, DEFAULT_DC);
}

/**
 * Set duty cycles of left wheel to turn car right.
 */
static void Turn_right45(void)
{
    car.direction = RIGHT45;

    Set_forward();

    Motor_start(DEFAULT_DC, 0);
}


/**
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
            target_counter = 11;

    if (g_counter >= target_counter && car.direction != STRAIGHT)
        Motor_stop();
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

    g_pid_left = max(min(g_pid_left, 1), 0);
    g_pid_right = max(min(g_pid_right, 1), 0);

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
Port1 ISR
Start motor when S1 is pressed.
Stop motor when S2 is pressed.
*/
void PORT1_IRQHandler(void)
{
    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    if (status & GPIO_PIN1)
    {
//        Move_forward();
        Turn_left45();
//        Station_Turn_right();
        Endless_forward();
//        Station_Turn_right();
    }

    if (status & GPIO_PIN4)
    {
//        Turn_right45();
//        Station_Turn_left();
        Motor_stop();
    }

    delay(5000);
}

/**
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

/**
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
float min(float a, float b)
{
    return a < b ? a : b;
}

/**
 * Get the larger number between two numbers.
 */
float max(float a, float b)
{
    return a > b ? a : b;
}
