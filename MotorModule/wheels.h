
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#ifndef WHEEL_H_
#define WHEEL_H_

void Wheel_init(void);
void Set_forward(void);
void Set_reverse(void);
void Set_stop(void);

void Set_leftFrontRightBack(void);
void Set_leftBackRightFront(void);

#endif /* MOTOR_H_ */
