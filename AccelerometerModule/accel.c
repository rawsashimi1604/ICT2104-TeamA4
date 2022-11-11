// MPU6060 Register DataSheet...
// https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf

/*************************************************************
 * INCLUDES
 */
#include "accel.h"


/*************************************************************
 * FUNCTIONS
 */

void resetMPU6050() {
    // Write 0x00 to 0x6B on MPU6050 PWR_MGMT_1, resetting MPU6050's power settings...
    I2C_write8(RESET_ADDRESS, 0x00);
}

int16_t readMPU6050AccelX() {
    // Read 16 bits from 0x3B (ACCEL_XOUT_H, ACCEL_XOUT_L), get MPU6050 Accelerometer X measurement...
    return I2C_read16(ACCEL_ADDRESS);
}

int16_t readMPU6050AccelY() {
    // Read 16 bits from 0x3D (ACCEL_YOUT_H, ACCEL_YOUT_L), get MPU6050 Accelerometer Y measurement...
    // Offset ACCEL_X_OUT_H by 2
    return I2C_read16(ACCEL_ADDRESS + 2);
}

int16_t readMPU6050AccelZ() {
    // Read 16 bits from 0x3F (ACCEL_ZOUT_H, ACCEL_ZOUT_L), get MPU6050 Accelerometer Z measurement...
    // Offset ACCEL_X_OUT_H by 4
    return I2C_read16(ACCEL_ADDRESS + 4);
}

int16_t readMPU6050GyroX() {
    // Read 16 bits from 0x43 (GYRO_XOUT_H, GYRO_XOUT_L), get MPU6050 Gyroscope X measurement...
    return I2C_read16(GYRO_ADDRESS);
}

int16_t readMPU6050GyroY() {
    // Read 16 bits from 0x45 (GYRO_YOUT_H, GYRO_YOUT_L), get MPU6050 Gyroscope Y measurement...
    // Offset GYRO_XOUT_H by 2
    return I2C_read16(GYRO_ADDRESS + 2);
}

int16_t readMPU6050GyroZ() {
    // Read 16 bits from 0x47 (GYRO_ZOUT_H, GYRO_ZOUT_L), get MPU6050 Gyroscope Z measurement...
    // Offset GYRO_XOUT_H by 4
    return I2C_read16(GYRO_ADDRESS + 4);
}

float readMPU6050Temp() {
    // Read 16 bits from 0x41 (TEMP_OUT_H, TEMP_OUT_L), get MPU6050 Temperature measurement...
    int16_t readValue = I2C_read16(TEMP_ADDRESS);

    // Convert raw temp data to temperature in degrees C
    // Temperature in degrees C = (TEMP_OUT Register Value as a signed quantity)/340 + 36.53

    return (readValue/340.0) + 36.53;
}


int main (void) {
    // Delay var (TODO CHANGE)
    volatile uint32_t ii;

    // Disable Watchdog, so code does not reset...
    WDT_A_holdTimer();

    // Init GPIOs and EUSCI_I2C configurations...
    initAccelerometer();

    while (1)
    {
        /* Delay between Transmissions */
        for (ii = 0; ii < 40000; ii++);

        int16_t valReceivedFromAccelX = readMPU6050AccelX();
        int16_t valReceivedFromAccelY = readMPU6050AccelY();
        int16_t valReceivedFromAccelZ = readMPU6050AccelZ();
        printf("accel Value \t-> X: %d, Y: %d, Z: %d\n", valReceivedFromAccelX, valReceivedFromAccelY, valReceivedFromAccelZ);

        int16_t valReceivedFromGyroX = readMPU6050GyroX();
        int16_t valReceivedFromGyroY = readMPU6050GyroY();
        int16_t valReceivedFromGyroZ = readMPU6050GyroZ();
        printf("gyro Value \t-> X: %d, Y: %d, Z: %d\n", valReceivedFromGyroX, valReceivedFromGyroY, valReceivedFromGyroZ);

        float valReceivedFromTemp = readMPU6050Temp();
        printf("temp Value \t-> %.2f\n", valReceivedFromTemp);

        printf("\n\n");
    }

}

