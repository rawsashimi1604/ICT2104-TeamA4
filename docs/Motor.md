# Motor Module

[Back to Home](../README.md)

## Motor Documentation

### Overview
We have designed and developed the motor controller using PWM to control the two DC motors. We also design and develop a PID controller to mitigate the differences in the DC Motor behaviour.

### MotorGPIOConfiguration

`void Wheel_init(void)`
- Initializes GPIO ports and pins to low output for both the left (P4.1 and P4.2) and right (P4.6 and P4.7) motors.

`void Set_forward(void)` <br>
- Set the right motor output to low (P4.1) and high (P4.2) and the left motor output to high (P4.3) and low (P4.4) to move forward.

`void Set_reverse(void)` <br>
- Set the right motor output to high (P4.1) and low (P4.2) and the left motor output to low (P4.3) and high (P4.4) to reverse.

`void Set_leftFrontRightBack(void)` <br>
- Set the left motor output to high (P4.3) and low (P4.4) and the right motor output to high (P4.1) and low (P4.2) to turn right.

`Set_leftBackRightFront(void)` <br>
- Set the left motor output to low (P4.3) and high (P4.4) and the right motor output to low (P4.1) and high (P4.2) to turn right.

`void Set_stop(void)` <br>
- Set the left motor output to low (P4.3 and P4.4) and the right motor output to low (P4.1 and P4.2) to stop.

### MotorPWMConfiguration

`void Motor_init(void)`
- Initializes `Wheel_init()`, PWM ports and pins, and timers for both the left (P2.5) and right (P2.4) motors.

`static void generatePWM(void)`
- Set the duty cycles for both the left and right motors and generate the PWM signal.

`static void Motor_start(int left_duty_cycle, int right_duty_cycle)`
- Set duty cycle of left and right wheels and start Timer A3 to trigger ISR more accurately.

`void Motor_stop(void)` <br>
-  Set the PWM duty cycle for both the motors to 0, initalizes `generatePWM()` and `Set_stop()` and cleaer the timer.

`void Motor_driveForward(void)` <br>
- Initalizes `Set_forward()` and `Motor_start(int left_duty_cycle, int right_duty_cycle)` to set the PWM duty cycle for both the motors to move forward.

`void Motor_reverse(void)` <br>
- Initializes `Set_reverse()` and `Motor_start(int left_duty_cycle, int right_duty_cycle)` to set the PWM duty cycle for both the motors to reverse.

`void Motor_turnLeft(void)` <br>
- Initializes `Set_leftBackRightFront()` and `Motor_start(int left_duty_cycle, int right_duty_cycle)` to set the PWM duty cycle for both the motors to turn left.

`void Motor_turnRight(void)` <br>
- Initializes `Set_leftFrontRightBack()` and `Motor_start(int left_duty_cycle, int right_duty_cycle)` to set the PWM duty cycle for both the motors to turn right.

`void Motor_turn45Left(void)` <br>
- Initalizes `Set_forward()` and `Motor_start(int left_duty_cycle, int right_duty_cycle)` to set the PWM duty cycle for both the motors to pivot 45 degrees to the right.

`void Motor_turn45Right(void)` <br>
- Initalizes `Set_forward()` and `Motor_start(int left_duty_cycle, int right_duty_cycle)` to set the PWM duty cycle for both the motors to pivot 45 degrees to the left.

`void Motor_pivotLeft(void)` <br>
- Initalizes `Set_forward()` and `Motor_start(int left_duty_cycle, int right_duty_cycle)` to set the PWM duty cycle for both the motors to pivot 90 degrees to the right.

`void Motor_pivotRight(void)` <br>
- Initalizes `Set_forward()` and `Motor_start(int left_duty_cycle, int right_duty_cycle)` to set the PWM duty cycle for both the motors to pivot 90 degrees to the left.

`static void Set_PID(void)` <br>
- Set the PID of the car to the desired value - between 0 and 1.
- The PID values is the percentage of the duty cycle.

### Demo
https://user-images.githubusercontent.com/90103591/204335812-b665ed6e-7d8a-4791-aab8-9aeb983cd710.mp4
