# Arduino on Africastalking-Eris-Devkit

#### tags: `eris-devkit`

## INTRODUCTION

Arduino IDE is a prototype platform which is used to write and upload the computer code to the physical board.

## ERIS DEV KIT

Africa's Talking Eris Development Kits are a series of microcontroller boards compatible with STM32Cube IDE and Arduino IDE. Eris v1 kits and boards ship with an STM32F103 Chip packaged in a manner that is both beginner friendly and also feature-packed for intermediate hobbyists. At the core is 72MHz ARM Cortex M3 with 64Kb of flash memory. The board ships with an inbuilt FTDI chip making it easy to program and SWD interface for easy debugging

### Specifications

* 32-bit ARM Cortex M3 clocked at 72MHz 3.3v logic
* 64 Kb of flash memory
* 20 Kb of SRAM
* 8MHz clock and 32kHz clock for RTC
* 2 USART interfaces
* 7-channel DMA
* 5VDC input voltage
* On-board SIM Module with antenna \(for R2 series\)
* On-board WiFi Module \(for R1 series\)
* USB Support \(MAX3421E\)
* On-board FTDI
* SWD Interface

The key features are

1. The dev kit are able to read analog input signals from different sensors and turn it into an output such as activating a motor, turning LED on/off, connect to the cloud and many other actions.
2. Unlike most previous programmable circuit boards, Eris Dev kit does not need an extra piece of hardware \(called a programmer\) in order to load a new code onto the board. You can simply use a USB cable.

## LAYOUT

### 1. **Power USB**

Eris Dev kit can be powered by using the USB cable from your computer. All you need to do is connect the USB cable to the USB connection

### 2. Power \(Barrel Jack\)

Eris Dev kits can be powered directly from the AC mains power supply by connecting it to the Barrel Jack, but its hould be 5V output.

### 3. Voltage Regulator

The function of the voltage regulator is to control the voltage given to the Eris Dev kit and stabilize the DC voltages used by the processor and other elements.

### 4. Crystal Oscillator

The crystal oscillator helps Eris Dev kit in dealing with time issues.

### 5. Arduino Reset

You can reset your Eris Dev kit, i.e., start your program from the beginning. You can reset the board by using the reset button on the board. When you reset the board while in BOOT 0 the code that was initially there will start to run. If you reset in BOOT 1 the code will be erase ready to receive new code.

### 6. Pins

* **3.3V** PB0, PB1, PA6, PA7, PB8, PB9 − Supply 3.3 output volt
* **5V** PB10, PB11 5Vpin − Supply 5 output volt

Most of the components used with Eris Dev kit works fine with 3.3 volt and 5 volt.

* **GND \(Ground\)** − There are several GND pins on the Eris Dev kit, any of which can be used to ground your circuit.
* **ICSP pin**

Mostly consisting of MOSI, MISO, SCK, RESET, VCC, and GND. It is often referred to as an SPI \(Serial Peripheral Interface\), which could be considered as an "expansion" of the output. Actually, you are slaving the output device to the master of the SPI bus.

* **Power LED indicator**

This LED D7 should light up when you plug your Arduino into a power source to indicate that your board is powered up correctly. If this light does not turn on, then there is something wrong with the connection.

* **TX and RX LEDs**

On your board, you will find two labels: TX \(transmit\) and RX \(receive\). They indicate the pins responsible for serial communication. Second, the TX and RX led \(13\). The TX led flashes with different speed while sending the serial data. The speed of flashing depends on the baud rate used by the board. RX flashes during the receiving process.

## COMMUNICATION

### Parallel Communication

Parallel connection between the Eris Dev kit and peripherals via input/output ports is the ideal solution for shorter distances up to several meters. However, in other cases when it is necessary to establish communication between two devices for longer distances it is not possible to use parallel connection. Parallel interfaces transfer multiple bits at the same time.

### Serial Communication Modules

Serial communication is almost the defacto on most microcontroller boards.

Types of Serial Communications

* Synchronous − Devices that are synchronized use the same clock and their timing is in synchronization with each other.
* Asynchronous − Devices that are asynchronous have their own clocks and are triggered by the output of the previous state.

It is easy to find out if a device is synchronous or not. If the same clock is given to all the connected devices, then they are synchronous. If there is no clock line, it is asynchronous. For example, UART \(Universal Asynchronous Receiver Transmitter\) module is asynchronous.

Baud Rate: The term baud rate is used to denote the number of bits transferred per second \[bps\].

### Arduino UART

The following code will make Arduino send hello world when it starts up.

```cpp
void setup() {
   Serial.begin(9600); //set up serial library baud rate to 9600
   Serial.println("hello world"); //print hello world
}

void loop() {

}
```

## SETUP

1. Launch Arduino IDE. Click on "File" menu and then "Preferences".

The "Preferences" dialog will open, then add the following link to the "Additional Boards Managers URLs" field:

"[http://dan.drown.org/stm32duino/package\_STM32duino\_index.json](http://dan.drown.org/stm32duino/package_STM32duino_index.json)"

Click "Ok"

1. Click on "Tools" menu and then "Boards &gt; Boards Manager"

The board manager will open and you will see a list of installed and available boards.

1. Select the "STM32 F103Cxxx" and click on install.

After installation is complete an "INSTALLED" tag appears next to the core name.

Now you can find the STM32 boards package in the "Board" menu.

1. Select the desired boards series : STM32F103Cxxx

