# Communications Module

[Back to Home](../README.md)

<!--
### Pre-requisite
1. You have [VSCode](https://code.visualstudio.com/) installed on your device.
2. Have the [vscode-m5stack-mpy](https://marketplace.visualstudio.com/items?itemName=curdeveryday.vscode-m5stack-mpy) extension installed on your VSCode.
3. Set M5Stack(UIFlow) in USB Mode.
4. Click "Add M5Stack device" and select the correct serial port of M5Stack.

### How to get the M5 Ready
1. With VSCode open, load the car_comm_mod.py into the M5Stack device
2. Run the python file in the M5Stack
3. 
-->
### Pre-requisite
1. Download the [M5 Burner](https://docs.m5stack.com/en/download)
2. Ensure that you have choose the correct port and set the baud rate to 115200
3. Burn your M5 StickCPlus with UIFlow_StickC_Plus v1.9.8-plus filmware
4. Configure your M5 in Internet Mode
5. Enter your SSID and Password (Ensure that your PC is on the same network as well)
6. Note down your API key

### How to connect the MSP to M5
MSP GPIO 3.2 &rarr; M5 G26  
MSP GPIO 3.3 &rarr; M5 G0

### How to get the M5 Ready
1. Proceed to https://flow.m5stack.com/
2. Enter your API key (retrievable from pre-requisite) and choose the correct device
3. Load the car_comm_mod.py file
4. Run the python Code 
5. You can view the data being sent from MSP to M5 on the M5 itself. Click on Button B to generate a QR code to use Remote+.

### Remote+
- On Remote+ you will be able to see the data being sent from MSP432
- You can key in Input to sent the data over to MSP

### On MSP432
1. Open the terminal console
2. Choose the correct serial port, Baud rate: 115200, Data size: 8, Parity: None, Stop bits: 1 
3. Data sent from Remote+ will be display here

### Button legend
A = Clear the data  
B = Show QR code to scan for remote+
