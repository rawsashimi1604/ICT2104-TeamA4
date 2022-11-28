# Ultrasonic and Accelerometer Module

[Back to Home](../README.md)

## Ultrasonic Documentation

### Overview
We have used the HR-SR04 Sensor for our project. We monuted 4 sensors on the car. A trigger signal will be sent to the sensor, and the MSP432 will start calculating the time elapsed for the sensor to return a signal, thus calculating the exact distance of the object. 

*SENSOR 1*: FRONT<br>
*SENSOR 2*: LEFT<br>
*SENSOR 3*: RIGHT<br>
*SENSOR 4*: BACK<br>

The files are split into `init` and `ultrasonic`, with `init` containing intialization codes, and `ultrasonic` containg ultrasonic logic codes.

### UltrasonicSensorConfiguration
To increase code reusability, we have decided to create a struct to pass our configurations to each sensor. This ensures we can create 1 function and reuse it for 4 sensors.

```c
struct UltrasonicSensorConfiguration {
    uint_fast8_t triggerPort;         // Trigger Port
    uint_fast16_t triggerPin;         // Trigger Pin
    uint_fast8_t echoPort;            // Echo Port
    uint_fast16_t echoPin;            // Echo Pin
    uint32_t timer;                   // Timer for measuring duration
    uint32_t* sensorInterruptCount;   // Count to measure distance
    uint8_t bufferIndex;              // Stores which buffer index to store data in...
};
```


### Init Functions
```c
// Public functions
void Ultrasonic_init();

// Private functions
static void initPortsAndPins();
static void initTimers();
static void initInterrupts();
static void initFilters();
```

`void Ultrasonic_init()`
- Initalizes ultrasonic sensors ports and pins, timers, interrupts and filters.
- Function to be called in main.

`static void initPortsAndPins()` <br>
- Initializes ports and pins for MSP432 for ultrasonic sensor.

`static void initTimers()` <br>
- Initializes timers for MSP432 for ultrasonic sensor.

`static void initInterrupts()` <br>
- Initializes interrupts for MSP432 for ultrasonic sensor.

`static void initFilters()` <br>
- Initializes filters, creating SMA and EMA `FloatQueue` for MSP432 for ultrasonic sensor.

<br>

### Ultrasonic Functions

`bool Ultrasonic_checkFront();`
- Checks the front sensor if an object is within the threshold. Threshold is defined as `ULTRASONIC_THRESHOLD` <br>

`bool Ultrasonic_checkLeft();`
- Checks the left sensor if an object is within the threshold. Threshold is defined as `ULTRASONIC_THRESHOLD` <br>

`bool Ultrasonic_checkRight();`
- Checks the right sensor if an object is within the threshold. Threshold is defined as `ULTRASONIC_THRESHOLD` <br>

`bool Ultrasonic_checkBack();`
- Checks the back sensor if an object is within the threshold. Threshold is defined as `ULTRASONIC_THRESHOLD` <br>


`float Ultrasonic_getDistanceFromFrontSensor();`
- After getting distance from front sensor, it will store distance in the cache which is index by `UltrasonicSensorConfiguration.bufferIndex`. Returns the value stored at the global caches 0th index. <br>

`float Ultrasonic_getDistanceFromLeftSensor();`
- After getting distance from left sensor, it will store distance in the cache which is index by `UltrasonicSensorConfiguration.bufferIndex`. Returns the value stored at the global caches 0th index. <br>

`float Ultrasonic_getDistanceFromRightSensor();`
- After getting distance from right sensor, it will store distance in the cache which is index by `UltrasonicSensorConfiguration.bufferIndex`. Returns the value stored at the global caches 0th index. <br>

`float Ultrasonic_getDistanceFromBackSensor();`
- After getting distance from back sensor, it will store distance in the cache which is index by `UltrasonicSensorConfiguration.bufferIndex`. Returns the value stored at the global caches 0th index. <br>


`void TA2_0_IRQHandler(void);`
- Interrupt handler for Timer A2. Interrupt happens during ultrasonic distance calculation, it is used to measure the total time taken for the signal to rebound back to echo pin. By calculating the total elapsed time, we can get an accurate distance measured by the ultrasonic sensor.



`static bool checkSensorDetectObject(UltrasonicSensorConfiguration* sensorConfig);`
- This function returns if an object was detected. To measure whether an object was detected, we have decided to implement a constant threshold `ULTRASONIC_THRESHOLD ` 
- We are capturing 20 readings from the sensor and calculating the average values. This is done by using simple moving average fliter, as well as the limit fliter which helps to remove anamolies in the data. In between each reading, we have used a delay of 60ms according to the HC-SR04 datasheet.
- Ultrasonic sensor configuration struct object passed in each sensors configuration value.<br>

`static void triggerUltrasonicSensor(UltrasonicSensorConfiguration* sensorConfig);`
-  This function will set a high on output pin to HC-SR04 then we use a delay of 1ms to wait for the reply from HR-SC04, then we will set back low on outpin
- Ultrasonic sensor configuration struct object passed in each sensors configuration value.<br>

`static uint32_t getDuration(UltrasonicSensorConfiguration* sensorConfig);`
- Gets total time elapsed from HC-SR04 to finish sending signal from  echo pin. calculating it by using a global interrupt count combined with TimerA2 current register value.
- Ultrasonic sensor configuration struct object passed in each sensors configuration value.<br>

`static float getDistance(UltrasonicSensorConfiguration* sensorConfig);`
- After calculating the total time elapsed, use formula provided by HC-SR04 datasheet to calculate the distance of the object detected. 
- Ultrasonic sensor configuration struct object passed in each sensors configuration value.<br>


<br>
<br>
<br>

## Accelerometer Documentation

### Overview
We have used MPU6050 in our project. We have mounted MPU6050 at the very front of the car as we want to detect the hump as soon as possible. MPU6050 has both 3-Axis accelerometer and 3-Axis gyroscope integrated on a single chip. Accelerometer measures the change in velocity of an object over time. Gyroscope measures the angular velocity.


The files are split into `init`, `accel` and `i2c_library`, with `init` containing intialization codes, `accel` containing accelerometer logic codes and `i2c_library` containing the i2c logic.<br><br>


### Init Functions
```c
// Public functions
void Accelerometer_init();

// Private functions
static void initPortsAndPins();
static void initI2CConfigs();
static void initTimers();
static void initInterrupts();
```


`void Accelerometer_init()`
- Initalizes accelerometer sensors ports and pins, timers, interrupts and filters.
- Function to be called in main.

`static void initPortsAndPins()` <br>
- Initializes ports and pins for MSP432 for accelerometer sensor.

`static void initTimers()` <br>
- Initializes timers for MSP432 for accelerometer sensor.

`static void initInterrupts()` <br>
- Initializes interrupts for MSP432 for accelerometer.

`static void initI2CConfigs()` <br>
-  Initializes I2C for accelerometer. Setting the slave address of the accelerometer.<br>

### Accelerometer Functions

`int8_t I2C_read8(int8_t readAddress);`
-  Reads 8 bits of data using I2C protocol from the slave address. Input values include the register to read from for the specified read address.<br>

`int16_t I2C_read16(int8_t readAddress);`
-  Reads 16 bits of data using I2C protocol from the slave address. Input values include the register to read from for the specified read address.<br>

`void I2C_write8(int8_t writeAddress, int8_t writeByte);`
-  Writes 8 bits of data using I2C protocol to the slave address. Input values include the register to write to as well as the value you want to write for the specified write address.<br>

`void I2C_write16(int8_t writeAddress, int16_t writeBytes);`
-  Writes 16 bits of data using I2C protocol to the slave address. Input values include the register to write to as well as the value you want to write for the specified write address.<br>




`void resetMPU6050();`
- By Write 0x00 to 0x6B on MPU6050 PWR_MGMT_1, resetting MPU6050's power settings<br>

`void recalibrateMPU6050();`
-  We can call this function in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
- Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
- Read accelerometer values `RECALIBRATION_COUNT`times

`float getTimerDuration();`
- Gets total elsaped time to convert gyroscope values from degree per seconds to degree.


`static float readMPU6050AccelX();`
- Read 16 bits from 0x3B (ACCEL_XOUT_H, ACCEL_XOUT_L), get MPU6050 Accelerometer X measurement
- Divide by 16384.0 according to datasheet<br>

`static float readMPU6050AccelY();`
- Read 16 bits from 0x3D (ACCEL_YOUT_H, ACCEL_YOUT_L), get MPU6050 Accelerometer Y measurement
- Offset ACCEL_X_OUT_H by 2
- Divide by 16384.0 according to datasheet<br>

`static float readMPU6050AccelZ();`
- Read 16 bits from 0x3F (ACCEL_ZOUT_H, ACCEL_ZOUT_L), get MPU6050 Accelerometer Z measurement
- Offset ACCEL_X_OUT_H by 4
- Divide by 16384.0 according to datasheet<br>


`static float readMPU6050GyroX();`
- Read 16 bits from 0x43 (GYRO_XOUT_H, GYRO_XOUT_L), get MPU6050 Gyroscope X measurement
- Divide by 131.0 according to datasheet <br>

`static float readMPU6050GyroY();`
- Read 16 bits from 0x45 (GYRO_YOUT_H, GYRO_YOUT_L), get MPU6050 Gyroscope Y measurement
- Offset GYRO_XOUT_H by 2
- Divide by 131.0 according to datasheet<br>

`static float readMPU6050GyroZ();`
- Read 16 bits from 0x47 (GYRO_ZOUT_H, GYRO_ZOUT_L), get MPU6050 Gyroscope Z measurement
- Offset GYRO_XOUT_H by 4
- Divide by 131.0 according to datasheet<br>


`float getPitchAngle();`
-  Upon getting the error from `recalibrateMPU6050()`, we can use the calibrated error values to calculate a accurate gyroscope values.
- *0.04 due to gyroscope error drifting, we would then return the pitch to perform calculation of the hump angle.

`void calculateHumpAngle();`
- Upon recieveing pitch angle from `getPitchAngle()` and detecting hump if the pitch angle is above the `HUMP_PITCH_ANGLE_THRESHOLD` then we will start incrementing the global `humpAngle` variable.
- Upon detecting the end of the hump using `NEG_HUMP_PITCH_ANGLE_THRESHOLD`, perform hump height calculations using trigonometry.
