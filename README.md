# Smart Automatic Water Tank Management and IoT Monitoring System

## 📌 Project Overview

The **Smart Automatic Water Tank Management and IoT Monitoring System** is an embedded system project based on the **LPC2129 ARM7 microcontroller**.

The main objective of this project is to automate water tank management by monitoring the water level and other parameters, controlling the water pump and valve automatically, displaying the information on an LCD, storing important data in EEPROM, and transmitting monitoring information to the cloud using an **ESP-01 Wi-Fi module**.

The system reduces manual intervention, prevents water overflow, and provides remote monitoring through IoT.

---

## 🎯 Objectives

* Monitor the water level in the tank.
* Measure the distance between the ultrasonic sensor and water surface.
* Measure temperature using an LM35 sensor.
* Automatically control the water pump.
* Control the water flow/valve using a servo motor.
* Display sensor information on a 16×2 LCD.
* Store important parameters in EEPROM.
* Maintain date and time using an RTC.
* Send monitoring data to the cloud using ESP-01 Wi-Fi.
* Provide CAN communication support for parameter transmission.
* Reduce water wastage and manual effort.

---

## 🧠 System Architecture

```text
                     ┌──────────────────────┐
                     │    Water Level       │
                     │       Sensor         │
                     └──────────┬───────────┘
                                │
                                │ ADC
                                ↓
                     ┌──────────────────────┐
                     │                      │
                     │      LPC2129         │
                     │      ARM7 MCU        │
                     │                      │
                     └──────┬─────┬─────┬───┘
                            │     │     │
             ┌──────────────┘     │     └──────────────┐
             │                    │                    │
             ↓                    ↓                    ↓
          Relay                 Servo                LCD
             │                 Motor                  │
             ↓                    │                    │
         Water Pump              Valve            Display
             
             ┌─────────────────────────────────────────┐
             │                                         │
             ↓                                         ↓
          EEPROM / RTC                              ESP-01
             │                                         │
             │ I²C                                    │ UART
             │                                         ↓
             │                                      Wi-Fi
             │                                         │
             │                                         ↓
             │                                    ThingSpeak
             │
             └─────────────────────────────────────────┘
```

---

# ⚙️ Hardware Components

| Component          | Purpose                                   |
| ------------------ | ----------------------------------------- |
| LPC2129 ARM7       | Main microcontroller                      |
| Water Level Sensor | Measures water level                      |
| LM35               | Measures temperature                      |
| Ultrasonic Sensor  | Measures distance to water surface        |
| Relay              | Controls water pump                       |
| Water Pump         | Pumps water into the tank                 |
| Servo Motor        | Controls valve mechanism                  |
| 16×2 LCD           | Displays system information               |
| ESP-01             | Provides Wi-Fi connectivity               |
| EEPROM             | Non-volatile data storage                 |
| DS1307 RTC         | Maintains date and time                   |
| CAN Interface      | CAN-based parameter communication         |
| Push Button        | Mode selection through external interrupt |

---

# 🔌 Pin Connections

## LPC2129 Pin Mapping

| Module / Signal    | LPC2129 Pin   |
| ------------------ | ------------- |
| UART0 TX           | P0.0 / TXD0   |
| UART0 RX           | P0.1 / RXD0   |
| I²C SCL            | P0.2          |
| I²C SDA            | P0.3          |
| Servo              | P0.7 / PWM2   |
| Ultrasonic TRIG    | P0.10         |
| Ultrasonic ECHO    | P0.11         |
| External Interrupt | P0.16 / EINT0 |
| Relay              | P0.17         |
| CAN1               | P0.25         |
| Water Level Sensor | AD0.0         |
| LM35               | AD0.2         |
| LCD RS             | P1.17         |
| LCD RW             | P1.18         |
| LCD EN             | P1.19         |
| LCD D4             | P1.20         |
| LCD D5             | P1.21         |
| LCD D6             | P1.22         |
| LCD D7             | P1.23         |

---

# 🔹 Water Level Monitoring

The water level sensor provides an analog signal to the LPC2129.

The sensor is connected to:

```text
AD0.0
```

The LPC2129 contains a **10-bit ADC**, so the ADC output ranges from:

```text
0 to 1023
```

The controller reads the ADC value and processes it to determine the water level.

### Flow

```text
Water Level Sensor
        ↓
   Analog Signal
        ↓
      AD0.0
        ↓
      ADC
        ↓
 Digital Value
        ↓
 Water Level
```

---

# 🌡️ Temperature Monitoring

An **LM35 temperature sensor** is used to measure temperature.

The LM35 is connected to:

```text
AD0.2
```

The LM35 generates an analog voltage proportional to temperature.

The LPC2129 ADC converts this analog voltage into a digital value, which is then processed to obtain the temperature.

---

# 📏 Ultrasonic Distance Measurement

An ultrasonic sensor is used to measure the distance between the sensor and the water surface.

### Connections

```text
TRIG → P0.10
ECHO → P0.11
```

The controller generates a **10 µs trigger pulse**.

The ultrasonic sensor sends ultrasonic waves toward the water surface. The reflected wave produces the ECHO signal.

The LPC2129 measures the ECHO pulse duration and calculates the distance.

```text
LPC2129
   │
   │ Trigger Pulse
   ↓
Ultrasonic Sensor
   │
   │ Ultrasonic Wave
   ↓
Water Surface
   │
   │ Reflected Wave
   ↓
Ultrasonic Sensor
   │
   │ Echo Pulse
   ↓
LPC2129
```

The implementation converts the measured time into distance in centimeters.

---

# 🚰 Automatic Pump Control

A relay is used to control the water pump.

The relay is connected to:

```text
P0.17
```

The LPC2129 controls the relay based on the water-level condition.

### Connection

```text
LPC2129
   ↓
P0.17
   ↓
Relay Driver
   ↓
Relay
   ↓
Water Pump
```

The relay driver in the current code is active-low:

```c
IOCLR0 = RELAY;    // Relay ON
IOSET0 = RELAY;    // Relay OFF
```

This allows the controller to automatically switch the pump according to the required water level.

---

# 🔄 Servo Motor Control

A servo motor is used to control the valve.

The servo is connected to:

```text
P0.7 / PWM2
```

The LPC2129 generates a PWM signal to control the servo position.

The project provides three servo positions:

```text
0°   → 1000 µs
90°  → 1500 µs
180° → 2000 µs
```

### Flow

```text
LPC2129
   ↓
PWM2
   ↓
Servo Motor
   ↓
Valve Control
```

---

# 🖥️ LCD Display

A **16×2 LCD** is interfaced with the LPC2129 in **4-bit mode**.

### Control Connections

```text
RS → P1.17
RW → P1.18
EN → P1.19
```

### Data Connections

```text
D4 → P1.20
D5 → P1.21
D6 → P1.22
D7 → P1.23
```

The LCD is used to display parameters such as:

* Water level
* Temperature
* Distance
* Time
* System status

Using 4-bit mode reduces the number of GPIO pins required.

---

# 📡 UART Communication

UART0 is used for serial communication between the LPC2129 and the ESP-01 Wi-Fi module.

### Connections

```text
LPC2129              ESP-01

P0.0 / TXD0  ──────→ RX
P0.1 / RXD0  ←────── TX
```

The UART communication is configured for:

```text
Baud Rate : 9600 bps
Data      : 8 bits
Parity    : None
Stop Bit  : 1
```

Therefore the UART configuration is:

```text
9600, 8N1
```

---

# 📶 ESP-01 Wi-Fi and IoT

The ESP-01 module provides Wi-Fi connectivity to the system.

The LPC2129 communicates with ESP-01 using UART AT commands.

### Main communication sequence

```text
AT
   ↓
AT+CWMODE=1
   ↓
Connect to Wi-Fi
   ↓
Get IP information
   ↓
Connect to ThingSpeak
   ↓
Send HTTP request
   ↓
Upload parameters
```

The system establishes a TCP connection with:

```text
api.thingspeak.com
```

Sensor parameters can then be uploaded to the ThingSpeak IoT platform.

### IoT Data Flow

```text
Sensors
   ↓
LPC2129
   ↓
UART
   ↓
ESP-01
   ↓
Wi-Fi
   ↓
Internet
   ↓
ThingSpeak
```

> **Note:** Wi-Fi credentials and ThingSpeak API keys should not be committed to a public GitHub repository. Replace them with placeholders or use a secure configuration method before publishing the project.

---

# 💾 EEPROM

EEPROM is used for **non-volatile data storage**.

Unlike RAM, EEPROM retains stored information even after power is removed.

The project stores parameters such as:

```text
Temperature
Water Level
Distance
```

The data is written to EEPROM and read back for verification.

If the verification is successful:

```text
Data saved
```

Otherwise:

```text
Data not saved
```

---

# 🕒 RTC – DS1307

The **DS1307 Real-Time Clock** is used to maintain date and time.

It communicates with the LPC2129 through the I²C bus.

```text
P0.2 → SCL
P0.3 → SDA
```

The RTC allows the system to associate monitoring information with the current date and time.

---

# 🔗 I²C Communication

The LPC2129 I²C interface uses:

```text
P0.2 → SCL
P0.3 → SDA
```

The configured I²C frequency is:

```text
100 kHz
```

I²C is used for communication with devices such as:

* EEPROM
* RTC

Multiple I²C devices can share the same SCL and SDA lines because each device has a unique slave address.

---

# 🚘 CAN Communication

CAN1 communication is implemented in the project.

The CAN driver is configured for:

```text
Baud Rate : 125 Kbps
Message ID: 0x301
DLC       : 8
```

The function:

```c
can1_send_parameter(u8 t, u8 w, u8 d);
```

is designed to transmit parameters such as:

* Temperature
* Water level
* Distance

The CAN message structure contains:

```text
ID
Data
DLC
RTR
Frame Format
```

The CAN driver is included as part of the project communication architecture.

---

# ⚡ External Interrupt

The project uses **EINT0** for external interrupt handling.

### Connection

```text
Switch → P0.16 / EINT0
```

When the switch is pressed, the external interrupt service routine is executed.

The ISR toggles the system flag:

```c
flag = !flag;
```

This can be used for changing or selecting the operating mode.

---

# ⏱️ Timer and Delay

Timer0 is used to generate delays.

The project provides three delay functions:

```c
delay_ms();
delay_sec();
delay_uc();
```

They are used by different peripherals.

For example, the ultrasonic sensor requires a microsecond-level trigger pulse, so:

```c
delay_uc(10);
```

is used to generate the required timing.

---

# 📂 Project File Structure

```text
Smart-Automatic-Water-Tank-IoT/
│
├── README.md
│
├── header.h
├── proj.c
│
├── adc.c
├── delay.c
├── extintr.c
├── servo.c
├── wifi.c
├── candriver.c
├── eeprom.c
├── i2cdriver.c
├── relay.c
├── lcd.c
├── uart.c
└── ultrasonic.c
```

---

# 🧩 Software Modules

| File           | Description                          |
| -------------- | ------------------------------------ |
| `proj.c`       | Main application logic               |
| `header.h`     | Common declarations and data types   |
| `adc.c`        | ADC initialization and reading       |
| `delay.c`      | Timer0 based delays                  |
| `extintr.c`    | EINT0 configuration and ISR          |
| `servo.c`      | Servo motor and PWM control          |
| `wifi.c`       | ESP-01 and ThingSpeak communication  |
| `candriver.c`  | CAN1 initialization and transmission |
| `eeprom.c`     | EEPROM data storage                  |
| `i2cdriver.c`  | I²C read/write operations            |
| `relay.c`      | Relay and pump control               |
| `lcd.c`        | LCD driver                           |
| `uart.c`       | UART0 driver                         |
| `ultrasonic.c` | Ultrasonic distance measurement      |

---

# 🔄 Overall Working

The complete working sequence of the system is:

1. The LPC2129 initializes all required peripherals.
2. The water-level sensor provides the water-level information through ADC.
3. The LM35 provides temperature information through ADC.
4. The ultrasonic sensor measures the distance to the water surface.
5. The LPC2129 processes the sensor values.
6. Depending on the water-level condition, the relay controls the water pump.
7. The servo motor can control the valve using PWM.
8. The LCD displays the measured parameters and system status.
9. Important parameters are stored in EEPROM.
10. The RTC provides date and time information.
11. ESP-01 connects the controller to the Wi-Fi network.
12. The ESP-01 communicates with ThingSpeak through the Internet.
13. Monitoring information can be uploaded to the IoT platform.
14. CAN communication is available for transmitting parameters to another CAN node/system.

---

# 📊 Communication Protocols Used

| Protocol | Purpose                       | Pins              |
| -------- | ----------------------------- | ----------------- |
| ADC      | Sensor data acquisition       | AD0.0, AD0.2      |
| UART     | ESP-01 communication          | P0.0, P0.1        |
| I²C      | EEPROM and RTC                | P0.2, P0.3        |
| PWM      | Servo control                 | P0.7              |
| CAN      | Parameter communication       | CAN1 / P0.25      |
| GPIO     | Relay, ultrasonic and control | Various GPIO pins |
| EINT     | External switch interrupt     | P0.16             |

---

# 🛠️ Software Requirements

* Embedded C
* Keil µVision / ARM7-compatible toolchain
* LPC2129 device support
* Flash programming/debugging tool
* Serial terminal for UART monitoring
* ESP-01 Wi-Fi module
* ThingSpeak account for IoT monitoring

---

# 🔧 Hardware Requirements

* LPC2129 development board
* 16×2 LCD
* Water-level sensor
* LM35 temperature sensor
* HC-SR04 or compatible ultrasonic sensor
* Relay module
* Water pump
* Servo motor
* ESP-01 Wi-Fi module
* EEPROM
* DS1307 RTC
* Push button
* CAN interface/transceiver
* Power supply
* Connecting wires

---

# 🚀 Advantages

* Automatic water pump control
* Reduces water wastage
* Reduces manual intervention
* Real-time parameter monitoring
* IoT-based remote monitoring
* Non-volatile data storage
* Temperature and distance monitoring
* Multiple communication protocols
* Modular embedded software architecture

---

# 🔮 Future Enhancements

The project can be further enhanced by:

* Mobile application integration
* Automatic notifications when the tank is empty/full
* Cloud-based historical data analysis
* Water consumption monitoring
* Multiple tank monitoring
* Pump dry-run protection
* Leakage detection
* Automatic fault detection
* Secure cloud communication
* Battery backup
* Advanced web dashboard

---

# 👨‍💻 Project Skills Demonstrated

This project demonstrates practical knowledge of:

* Embedded C
* ARM7 LPC2129
* GPIO programming
* ADC
* Timers
* PWM
* UART
* I²C
* CAN
* External Interrupts
* LCD interfacing
* Sensor interfacing
* Relay control
* Servo control
* EEPROM
* RTC
* ESP-01 Wi-Fi
* IoT communication
* Modular driver development

---

# 📌 Key Specifications

```text
Microcontroller : LPC2129 ARM7
ADC Resolution  : 10-bit
UART Baud Rate  : 9600 bps
UART Format     : 8N1
I²C Frequency   : 100 kHz
CAN Baud Rate   : 125 Kbps
LCD             : 16×2, 4-bit mode
Servo           : PWM2
IoT Module      : ESP-01
Cloud Platform  : ThingSpeak
```

---

# 📜 Conclusion

The **Smart Automatic Water Tank Management and IoT Monitoring System** combines embedded control, sensor interfacing, communication protocols, automation, and IoT technologies into a single system.

The LPC2129 acts as the central controller and interfaces with sensors, LCD, relay, servo motor, EEPROM, RTC, CAN and ESP-01. The system helps automate water management while providing monitoring and connectivity through IoT.

This project demonstrates the practical implementation of an ARM7-based embedded system using multiple hardware peripherals and communication protocols.

---

## ⭐ Project Highlights

```text
✔ ARM7 LPC2129 based system
✔ Automatic water pump control
✔ Water level monitoring
✔ Temperature monitoring
✔ Ultrasonic distance measurement
✔ Servo-based valve control
✔ LCD monitoring
✔ EEPROM data storage
✔ RTC date/time
✔ ESP-01 Wi-Fi connectivity
✔ ThingSpeak IoT monitoring
✔ UART communication
✔ I²C communication
✔ CAN communication
✔ External interrupt handling
✔ Timer-based delays
```

---

## 📄 License

This project is intended for **educational and academic purposes**.

Feel free to study and modify the code for learning and development.
