/*
 * motor.h
 *
 *  Created on: 20 Sep 2022
 *      Author: lyc
 */

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

void Motor_init(void);
void Motor_start(int, int);
void Motor_stop(void);
void Motor_direction(char);

#endif /* MOTOR_H_ */
