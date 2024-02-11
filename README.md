# Modular Andon System

An improvisation of the previous Andon version utilizing a master and slave communication system.

## A. Introduction

#### Master

The Master serves as the brain or bridging connection between Slave 1 and Slave 2. The Master also handles requests and responses from the server. It receives input from RFID and utilizes an Ethernet module to establish an internet connection.

#### Slave

The Slave functions as a controller that receives input from sensors and actuators. Slave 1 receives input from TFT, Proximity, and relay sensors, and displays output on TFT. Meanwhile, Slave 2 provides output on the RGB DMD Panel to display status and calls.

## B. Components List

### 1. Master (ESP)
    - ESP32
    - RFID
    - Ethernet ENC28J60
    - Step Down 5V DC to 3.3 V DC
    - Push Button

### 2. Slave 1 (ESP)
    - ESP32
    - TFT LCD 3.5 Inch
    - Sensor Proximity Capacitive
    - Step UP 5V DC to 24V DC
    - Relay 24V

### 3. Slave 2 (ESP)
    - ESP32
    - Panel DMD RGB (HUB 75)

## C. System Architecture

The following is the system architecture that has been created.

![system](/images/system_architecture.PNG)

In the above system architecture, there is 1 Master unit and 2 slaves. The Master and slaves communicate serially through Rx and Tx pins on each device. The data transmitted via serial is in JSON format. For example, when a user initiates a call action to the maintenance person, Slave 1 will send JSON {"state": "call", "type": "Maintenance"} to the Master. The Master then saves the received JSON data from Slave 1 and will resend it to both slaves if it successfully receives a 200 response from the server. Subsequently, because both slaves have received the JSON data from the Master, state updates or displays are performed on each slave. This condition applies only to call, serve, end, and add mp operations. For the change operation, which is a status change, the previously saved JSON will be overwritten by the JSON originating from the server.

### 1. Master Wiring Diagram

![system](/images/wiring_master.png)

### 2. Slave 1 Wiring Diagram

![system](/images/wiring_slave1.png)

### 3. Slave 2 Wiring Diagram

![system](/images/wiring_slave2.png)

### 4. Power Wiring Diagram

![sytem](/images/wiring_power.png)

## D. Requirements

- Download [Arduino IDE](https://www.arduino.cc/en/software).
- Download [ESP32 Board Manager](https://github.com/iotechbugs/esp32-arduino) or download it through the board manager.
- Download [QElectro Tech](https://qelectrotech.org/download.php) for creating wiring diagrams.
- Components as previously mentioned.
- Download required libraries.

## E. Libraries

- ArduinoJson
- SPI
- EthernetENC
- MFRC522
- Adafruit_GFX
- MCUFRIEND_kbv
- TouchScreen
- qrcode
- ESP32-HUB75-MatrixPanel-I2S-DMA

**Libraries can be downloaded through the Arduino IDE library manager.**

## F. Folder and File

#### 1. main (Master)
    - main.ino
        Main program including library imports, global variables, setup, and loop.
    - ethernet_config.ino
        Ethernet configuration.
    - api_call.ino
        Functions to make requests and receive responses from the server.
    - rfid.ino
        Functions for RFID reading and other RFID operations.
    - slave1.ino
        Serial communication between Master and Slave 1.
    - slave2.ino
        Serial communication between Master and Slave 2.

#### 2. slave1 (Slave 1)
    - slave1.ino
        Main program including library imports, global variables, setup, and loop.
    - serial.ino
        Serial communication between Slave 1 and Master.
    - json.ino
        Processing received JSON data and sending JSON to Master.
    - view.ino
        Page display program on TFT.
    - component.ino
        Components used in the view.
    - touch.ino
        Program to receive user input through touch on the TFT screen to perform actions.
    - shot_count.ino
        Product shot count.
    - utils.ino
        Helper functions to process data.

#### 3. slave2 (Slave 2)
    - slave_2.ino
        Main program including library imports, global variables, setup, and loop.
    - serial.ino
        Serial communication between Slave 2 and Master.
    - json.ino
        Processing received JSON data.
    - p10.ino
        Displaying running text on P10.
    - utils.ino
        Helper functions to process data.

## G. Important Notes

### 1. Program / Sintaks

- In serial communication with JSON, always include the "state" and "type" keys as identifiers for the operation to be performed.
- Note the first key received from the response body; ensure that the response from the server has the same structure as the data processing by the master.

```c++

...
char* body = strstr(bodyStart, "{\"data\":{\"work_center_id\":");
...

```

- On Slave 1, use the qrcode and TouchScreen_kbv libraries in the same folder as the program file to avoid collisions with global Arduino libraries.

### 2. Wiring

- Pay attention to the input voltage on the Ethernet module; ensure that the voltage is always stable in the range of 3.0V - 3.3V. If it exceeds or falls below, it will cause the Ethernet to shut down or lose connection.
- Due to the use of many pins, ensure that there are no pins that are mistakenly connected.
