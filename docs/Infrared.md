# Infrared Module

[Back to Home](../README.md)

## Infrared Documentation
### Overview
For the infrared team, we are using the TCRT500 Infrared Sensor as well as the two wheel encoder given at the start of the module.

The two main objective of our module is to
1) Be able to scan a CODE39 barcode and return a valid char.
2) Be able to accurately measure the speed of the car and the distance travelled.

### Video Demonstration
[![Watch the video demonstration](https://img.youtube.com/vi/nFy2T19JbHw/hqdefault.jpg)](https://youtu.be/nFy2T19JbHw)

### Barcode

In order to understand how the barcode is scanned, we have done some research and came up with the following requirements that is in accordance with the CODE39 protocol.

- A char is 5 black bar sandwiching 4 white space.
- Bar ➜ black and Spaces ➜ white
- Bar code starts and ends with black 
- It will then alternate between black and white
- Can be both thin and thick
- Delimited by thin white space
- Must flip bit fields when detect wrong orientation (upside down *)
- Error checking can be implemented 
- Can use analog reading and digital reading
- Take note of shadows (make white spaces look darker) that will affect analog reading
- Need to take note of IR sensor's scan area. If scan area is too big will scan other bar/spaces as well.
- Maybe be affected by environmental light ➜ however can overcome by turning off the emitter and getting the environmental reading.

--- 

### Encoder 

To accurately calculate the vehicle's speed when it is moving, the team conducted research on how actual linear velocity is calculated in the industry. We learned the usage of a Hall Effect Sensor and did something similar with the notches instead of gears used in the mechanical industry. 

Instead of using each detected notch as a pulse, the team decided to use every 4 notches as a pulse. As a result, there will be 20/4 = 5 pulses for every rotation. In order to calculate the speed, the time taken between pulses is used to estimate the speed. If there is no movement for 2 seconds, it is considered that the vehicle has come to a halt. 

The area under the speed-time graph can be used to compute the distance traveled.
Furthermore, the infrared module has a function that begins counting the notches and returns the number of notches when it is stopped, which can then be used to determine the distance. The notch counting can be started and stopped at any time, allowing the vehicle to measure distance from any point A to any point B. 

--- 
### Testing Methodology

As this module has to takes in an input (barcode) that is highly volatile due to the nature of the environment (sunlight, shadow, distance, sensitivity etc). We have decided to go with blackbox testing to evaluate the validity of our functions.

The technique used for both file is Decision Table Testing.

#### barcode.c Decision Table Testing

![HKgsMx4.jpg](https://iili.io/HKgsMx4.jpg)

#### encoder.c Decision Table Testing

![HKgsEbf.jpg](https://iili.io/HKgsEbf.jpg)
![HKgs1OG.jpg](https://iili.io/HKgs1OG.jpg)
