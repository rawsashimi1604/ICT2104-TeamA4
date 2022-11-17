// MPU6060 Register DataSheet...
// https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf

/*************************************************************
 * INCLUDES
 */
#include "accel.h"

/*************************************************************
 * VARIABLEs
 */
bool detectedHump = false;

/*************************************************************
 * FUNCTIONS
 */

void resetMPU6050() {
    // Write 0x00 to 0x6B on MPU6050 PWR_MGMT_1, resetting MPU6050's power settings...
    I2C_write8(RESET_ADDRESS, 0x00);
}

void recalibrateMPU6050() {
    // We can call this function in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
    // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
    // Read accelerometer values RECALIBRATION_COUNT times

    // Delay variable
    volatile uint32_t ii;

    // Loop variable
    int16_t i;

    // Sum of all accel values
    float accValueXSum = 0;
    float accValueYSum = 0;

    // Sum of all gyro values
    float gyroValueXSum = 0;
    float gyroValueYSum = 0;
    float gyroValueZSum = 0;


    for (i = 0; i < RECALIBRATION_COUNT; i++) {

        // Delay between Transmissions...
        for (ii = 0; ii < 500; ii++);

        // Read values from MPU6050
        float accValueX = readMPU6050AccelX();
        float accValueY = readMPU6050AccelY();
        float accValueZ = readMPU6050AccelZ();

        float gyroValueX = readMPU6050GyroX();
        float gyroValueY = readMPU6050GyroY();
        float gyroValueZ = readMPU6050GyroZ();

        // Sum all accel values
        accValueXSum += ((atan((accValueY) / sqrt(pow((accValueX), 2) + pow((accValueZ), 2))) * 180 / M_PI));
        accValueYSum += ((atan(-1 * (accValueX) / sqrt(pow((accValueY), 2) + pow((accValueZ), 2))) * 180 / M_PI));

        // Sum all gyro values
        gyroValueXSum += gyroValueX;
        gyroValueYSum += gyroValueY;
        gyroValueZSum += gyroValueZ;

        printf("count: %d/%d\n", i, RECALIBRATION_COUNT);
    }

    // Calculate the errors
    accErrorX = accValueXSum / RECALIBRATION_COUNT;
    accErrorY = accValueYSum / RECALIBRATION_COUNT;

    gyroErrorX = gyroValueXSum / RECALIBRATION_COUNT;
    gyroErrorY = gyroValueYSum / RECALIBRATION_COUNT;
    gyroErrorZ = gyroValueZSum / RECALIBRATION_COUNT;

    printf("accErrorX: %.2f\n", accErrorX);
    printf("accErrorY: %.2f\n", accErrorY);

    printf("gyroErrorX: %.2f\n", gyroErrorX);
    printf("gyroErrorY: %.2f\n", gyroErrorY);
    printf("gyroErrorZ: %.2f\n", gyroErrorZ);

}

static float readMPU6050AccelX() {
    // Read 16 bits from 0x3B (ACCEL_XOUT_H, ACCEL_XOUT_L), get MPU6050 Accelerometer X measurement...
    // Divide by 16384.0 according to datasheet
    int16_t readValue = I2C_read16(ACCEL_ADDRESS);
    return readValue / 16384.0;
}

static float readMPU6050AccelY() {
    // Read 16 bits from 0x3D (ACCEL_YOUT_H, ACCEL_YOUT_L), get MPU6050 Accelerometer Y measurement...
    // Offset ACCEL_X_OUT_H by 2
    // Divide by 16384.0 according to datasheet
    int16_t readValue = I2C_read16(ACCEL_ADDRESS + 2);
    return readValue / 16384.0;
}

static float readMPU6050AccelZ() {
    // Read 16 bits from 0x3F (ACCEL_ZOUT_H, ACCEL_ZOUT_L), get MPU6050 Accelerometer Z measurement...
    // Offset ACCEL_X_OUT_H by 4
    // Divide by 16384.0 according to datasheet
    int16_t readValue = I2C_read16(ACCEL_ADDRESS + 4);
    return readValue / 16384.0;
}

static float readMPU6050GyroX() {
    // Read 16 bits from 0x43 (GYRO_XOUT_H, GYRO_XOUT_L), get MPU6050 Gyroscope X measurement...
    // Divide by 131.0 according to datasheet
    int16_t readValue = I2C_read16(GYRO_ADDRESS);
    return readValue / 131.0;
}

static float readMPU6050GyroY() {
    // Read 16 bits from 0x45 (GYRO_YOUT_H, GYRO_YOUT_L), get MPU6050 Gyroscope Y measurement...
    // Offset GYRO_XOUT_H by 2
    // Divide by 131.0 according to datasheet
    int16_t readValue = I2C_read16(GYRO_ADDRESS + 2);
    return readValue / 131.0;
}

static float readMPU6050GyroZ() {
    // Read 16 bits from 0x47 (GYRO_ZOUT_H, GYRO_ZOUT_L), get MPU6050 Gyroscope Z measurement...
    // Offset GYRO_XOUT_H by 4
    // Divide by 131.0 according to datasheet
    int16_t readValue = I2C_read16(GYRO_ADDRESS + 4);
    return readValue / 131.0;
}

static float readMPU6050Temp() {
    // Read 16 bits from 0x41 (TEMP_OUT_H, TEMP_OUT_L), get MPU6050 Temperature measurement...
    int16_t readValue = I2C_read16(TEMP_ADDRESS);

    // Convert raw temp data to temperature in degrees C
    // Temperature in degrees C = (TEMP_OUT Register Value as a signed quantity)/340 + 36.53

    return (readValue/340.0) + 36.53;
}

void PORT1_IRQHandler(void) {
    uint32_t pinActivated;
    printf("button interrupt\n");
    pinActivated = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, pinActivated);
}

float getPitchAngle() {

    // Get values from MPU6050 using I2C
    float accX = readMPU6050AccelX();
    float accY = readMPU6050AccelY();
    float accZ = readMPU6050AccelZ();
//    printf("accel Value \t-> X: %.2f, Y: %.2f, Z: %.2f\n", accX, accY, accZ);

    // Formula to get accAngleX and accAngleY
    float accAngleX = (atan(accY / sqrt(pow(accX, 2) + pow(accZ, 2))) * 180 / M_PI) - accErrorX;
    float accAngleY =  (atan(-1 * accX / sqrt(pow(accY, 2) + pow(accZ, 2))) * 180 / M_PI) - accErrorY;

//    printf("accel Angles \t-> X: %.2f, Y: %.2f\n", accAngleX, accAngleY);

    // Stop the timer and get total elapsed time...
    Timer_A_stopTimer(TIMER_MODULE);
    float durationInSeconds = getTimerDuration();

    // Get Gyro values, and use calibrated error values to calculate accurate gyro values
    float gyroX = readMPU6050GyroX() - gyroErrorX;
    float gyroY = readMPU6050GyroY() - gyroErrorY;
    float gyroZ = readMPU6050GyroZ() - gyroErrorZ;

//    printf("gyro Value \t-> X: %.2f, Y: %.2f, Z: %.2f\n", gyroX, gyroY, gyroZ);

    // *0.04 due to gyroscope error drifting
    float gyroAngleY = gyroY * durationInSeconds;
    float pitch = (0.96 * gyroAngleY) + (0.04 * accAngleY);

    // Restart the timer
    Timer_A_clearTimer(TIMER_MODULE);
    Timer_A_startCounter(TIMER_MODULE, TIMER_A_UP_MODE);

    return pitch;
}

void calculateHumpAngle() {

    // Calculate the current pitch angle...
    float pitchAngle = getPitchAngle();

    printf("pitch \t\t-> %.2f deg\n", pitchAngle);

    // pitch angle has detected a hump
    if (pitchAngle > HUMP_PITCH_ANGLE_THRESHOLD) {
        printf("Hump detected!\n");

        detectedHump = true;

        // Start measuring distance...
    }

    // Detect hump end
    if (detectedHump && pitchAngle < NEG_HUMP_PITCH_ANGLE_THRESHOLD) {

       printf("Hump finish!\n");
       detectedHump = false;

       // Get distance
       // Perform calculations here...
       printf("Hump angle: %.2f\n", humpAngle);
       // Send hump to comms / mapping
   }

    // Detected hump... add angle
    else if (detectedHump) {
        humpAngle += pitchAngle;
    }



}

// Get number of seconds...
float getTimerDuration() {

    int16_t totalTimerTicks;

    // Add the remaining value in TIMER_MODULE's register
    totalTimerTicks = Timer_A_getCounterValue(TIMER_MODULE);

    // get time elapsed In seconds
    return totalTimerTicks * (1 / (CS_getACLK() / (TIMER_DIVIDER)/1.0));
}

