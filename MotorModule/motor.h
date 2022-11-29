/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#ifndef MOTOR_H_
#define MOTOR_H_

typedef struct _Car
{
    char direction;
    bool stop;
    bool detected_obstacle;
    bool is_moving;
    uint16_t left_dc;
    uint16_t right_dc;
    uint16_t left_wheel_count;
    uint16_t right_wheel_count;
} Car;

#define LEFT 'L'
#define RIGHT 'R'
#define FORWARD 'F'
#define BACKWARD 'B'

void Motor_init(void);
void Motor_stop(void); //called in encoder interrupt to know when to stop

void Motor_driveForward(void);
void Motor_reverse(void);

void Motor_turnLeft(void);
void Motor_turnRight(void);

void Motor_pivotRight(void);
void Motor_pivotLeft(void);
void Motor_turn45Left(void);
void Motor_turn45Right(void);

void Motor_customRightTurn(int);
#endif /* MOTOR_H_ */
