# Communication Protocols

| **Protocol** | **Clock** | **Wires** | **Speed** |
|:-------------|:----------|:----------|:----------|
| **I2C**      | Sync      | 2         |~400kHz    |
| **UART**     | Async     | 2         | ~1Mbps    |
| **SPI**      | Sync      | 4+        | 10+MHz    |

## I2C Protocol
Inter-Integrate Circuit or I2C protocol is a protocol used to allow microcontrollers and peripherals such as sensors, registers etc to communicate with each other.  
Much like the SPI, I2C is also intended for short distance communications within a single device or intra-device. I2C requires only 2 wires to exchange information, a **Serial Data Line (SDA)** and a **Serial Clock Line (SCL)**.  

On the hardware level I2C bus drivers are "open drain", which means that they can pull the signal low but they can't drive it high. This is done by design and the reason is that devices won't compete between which will drive it high and which low. That's why each signal line has its own pull-up resistor.

#TODO: Expand on topology, duplex and addressing.
#TODO: Add images of I2C signals and explain

**Resources**  
[I2C Official Documentation](https://www.nxp.com/docs/en/user-guide/UM10204.pdf)  
[I2C Tutorial (sparkfun)](https://learn.sparkfun.com/tutorials/i2c/all)

## SPI Protocol
Serial Peripheral Interface or SPI protocol is a high-speed communication protocol that allows microcontrollers and peripherals such as sensors, registers etc to communicate with each other. SPI, just like I2C, is synchronous but requires 4 or more wires to operate, **Master In Slave Out (MISO)**, **Master Out Slave In (MISO)**, **Serial Clock (SCK)** and **Chip Select (CS)**.

#TODO: Expand on duplex and CS per device
#TODO: Add images of SPI signals and explain

**Resources**  
[SPI User Guide (TI)](https://www.ti.com/lit/ug/sprugp2a/sprugp2a.pdf?ts=1760743558496&ref_url=https%253A%252F%252Fwww.google.com%252F)  
[SPI for Linux and Embedded Systems](https://www.kernel.org/doc/html/v4.14/driver-api/spi.html)  
[SPI Tutorial (sparkfun)](https://learn.sparkfun.com/tutorials/serial-communication)  
[SPI Standards and Compliance](https://www.etsi.org/deliver/etsi_ts/103700_103799/103713/15.04.01_60/ts_103713v150401p.pdf)  

# ESP32 Pin Reference Guide

## GPIO Pin Capabilities

| GPIO | Input | Output | Notes | Status |
|------|-------|--------|-------|--------|
| **0** | ✅ | ✅ | Boot button, pulled up | ⚠️ Must be HIGH at boot |
| **1** | ❌ | ❌ | TX (Serial) | ⚠️ Used for programming |
| **2** | ✅ | ✅ | Built-in LED (some boards) | ⚠️ Must be LOW at boot |
| **3** | ❌ | ❌ | RX (Serial) | ⚠️ Used for programming |
| **4** | ✅ | ✅ | Safe for general use | ✅ Good choice |
| **5** | ✅ | ✅ | Safe for general use | ✅ Good choice |
| **6-11** | ❌ | ❌ | Connected to flash memory | ❌ **NEVER USE** |
| **12** | ✅ | ✅ | Boot config pin | ⚠️ Must be LOW at boot |
| **13** | ✅ | ✅ | Safe for general use | ✅ Good choice |
| **14** | ✅ | ✅ | Safe for general use | ✅ Good choice |
| **15** | ✅ | ✅ | Boot config pin | ⚠️ Must be HIGH at boot |
| **16** | ✅ | ✅ | Safe (if not using PSRAM) | ✅ Good choice |
| **17** | ✅ | ✅ | Safe (if not using PSRAM) | ✅ Good choice |
| **18** | ✅ | ✅ | Safe for general use | ✅ Good choice |
| **19** | ✅ | ✅ | Safe for general use | ✅ Good choice |
| **21** | ✅ | ✅ | **Default I2C SDA** | ✅ Recommended for I2C |
| **22** | ✅ | ✅ | **Default I2C SCL** | ✅ Recommended for I2C |
| **23** | ✅ | ✅ | Safe for general use | ✅ Good choice |
| **25** | ✅ | ✅ | DAC1, ADC2 | ✅ Good for analog |
| **26** | ✅ | ✅ | DAC2, ADC2 | ✅ Good for analog |
| **27** | ✅ | ✅ | ADC2, Touch sensor | ✅ Good choice |
| **32** | ✅ | ✅ | ADC1, Touch sensor | ✅ Good for analog |
| **33** | ✅ | ✅ | ADC1, Touch sensor | ✅ Good for analog |
| **34** | ✅ | ❌ | **Input only**, ADC1 | ⚠️ No internal pull-up/down |
| **35** | ✅ | ❌ | **Input only**, ADC1 | ⚠️ No internal pull-up/down |
| **36 (VP)** | ✅ | ❌ | **Input only**, ADC1 | ⚠️ No internal pull-up/down |
| **39 (VN)** | ✅ | ❌ | **Input only**, ADC1 | ⚠️ No internal pull-up/down |

## Special Function Pins

| Function | Default Pins | Configurable? | Notes |
|----------|--------------|---------------|-------|
| **I2C** | SDA=21, SCL=22 | ✅ Yes | Can use any GPIO |
| **SPI** | MOSI=23, MISO=19, SCK=18, CS=5 | ✅ Yes | Can use any GPIO |
| **UART0** | TX=1, RX=3 | ⚠️ Limited | Used for Serial Monitor |
| **UART1** | TX=10, RX=9 | ✅ Yes | Can reassign |
| **UART2** | TX=17, RX=16 | ✅ Yes | Can reassign |
| **ADC1** | 32-39 | ❌ Fixed | Works with WiFi |
| **ADC2** | 0, 2, 4, 12-15, 25-27 | ❌ Fixed | **Conflicts with WiFi!** |
| **DAC** | 25, 26 | ❌ Fixed | 8-bit DAC output |
| **Touch** | 0, 2, 4, 12-15, 27, 32, 33 | ❌ Fixed | Capacitive touch |
| **PWM** | Any GPIO | ✅ Yes | 16 channels available |

## Recommended Pins by Use Case

### Digital I/O (Safe Pins)
GPIO: **4, 5, 13, 14, 16, 17, 18, 19, 23, 25, 26, 27, 32, 33**

### Analog Input (ADC)
- **With WiFi**: Use ADC1 pins (32, 33, 34, 35, 36, 39)
- **Without WiFi**: Can also use ADC2 pins (0, 2, 4, 12-15, 25-27)

### I2C Communication
- **SDA**: GPIO 21 (default)
- **SCL**: GPIO 22 (default)

### SPI Communication
- **MOSI**: GPIO 23
- **MISO**: GPIO 19
- **SCK**: GPIO 18
- **CS**: GPIO 5 (or any GPIO)

### Analog Output (DAC)
- **DAC1**: GPIO 25
- **DAC2**: GPIO 26

## Boot Mode Pins (Important!)

These pins affect boot behavior. Avoid using them if possible, or ensure correct voltage levels at boot:

| Pin | Boot Requirement | Safe to Use? |
|-----|------------------|--------------|
| GPIO 0 | Must be HIGH | ⚠️ Has pull-up, but boot button pulls LOW |
| GPIO 2 | Must be LOW | ⚠️ Usually okay, some boards have LED |
| GPIO 5 | Any | ✅ Safe |
| GPIO 12 | Must be LOW | ⚠️ MTDI strapping pin |
| GPIO 15 | Must be HIGH | ⚠️ Has pull-up |

## Pins to Avoid

| Pins | Reason |
|------|--------|
| **GPIO 6-11** | Connected to internal flash - **DO NOT USE** |
| **GPIO 1, 3** | UART0 (Serial Monitor) - avoid unless necessary |
| **ADC2 pins** | If using WiFi/Bluetooth |

## Notes
- Input-only pins (34-39) have no internal pull-up/pull-down resistors
- ADC2 is disabled when WiFi is active
- All GPIOs can be used for PWM output
- Maximum output current per pin: 12mA (40mA absolute max)
- Total current for all GPIOs: 200mA max