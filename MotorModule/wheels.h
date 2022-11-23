
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#ifndef WHEEL_H_
#define WHEEL_H_

void Set_forward(void);
void Set_reverse(void);

void set_leftFrontRightBack(void);
void set_leftBackRightFront(void);

#endif /* MOTOR_H_ */
