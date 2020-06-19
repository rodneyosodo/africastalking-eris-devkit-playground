Bare Metal STM32 Programming – LED BlinkPosted in Embedded By Grig On December 18, 2019
Do you want to start STM32 programming? Get the Blue Pill which is featuring the powerful & cheap ARM Cortex-M3 32-bit microcontroller then, let’s go!



We are going to build a LED blink app from scratch, using only the manual of the STM32F103C8T6 chip. The host platform used for this guide is Linux Mint 19.3. You can use as well, Ubuntu, Raspbian, and even Microsoft Windows.

Prerequisites
The hardware: the blue pill and for uploading the firmware we need at least a ST-Link v2 device.

The software: need to have installed the ST-Link tool and the cross compiler.

Prior knowledge: basic Linux operation, not being afraid of command line, C language, basic embedded stuff.

Steps to do
We create in the same directory four files named:
main.c
linker.ld
crt.s
makefile
Make a test build
Connect the ST-Link
Explain the schematic and documentation
Write the code in main.c
registers and bits definitions
infinite loop that turns on and off the LED
Build and upload to the board
Dummy main.c
void main (void)
{
    while (1);
}
For now it is just an infinite loop that does nothing.

C run-time assembly file crt.s
.cpu cortex-m3
.thumb
// end of 20K RAM
.word 0x20005000
.word _reset
.thumb_func
_reset:
    bl main
    b .
If you are not familiar with the ARM/thumb assembly and you don’t want to be, just skip the explanations below.

The first two lines are instructing the assembler to generate thumb code for the Cortex-M3 core. The thumb code is more compact in terms of memory footprint, but is a bit slower at run time. The value written at first .word line will be placed at the very beginning of the binary file at address 0x0000_0000 and represents the end of RAM. The second .word line is the reset vector and is placed at 0x0000_0004. The .thumb_func directive instructs the assembler to generate a thumb executable code from the subsequent lines. The last two lines are the jump to main() and the infinite loop, in case someone wants to exit from the main function.

Linker script file linker.ld
MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
  RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
}
This instructs the linker how to place different sections of data into the binary. It usually refers to ROM/FLASH and RAM. This data can be found in the datasheet at page 34.

Let’s build everything: makefile
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
STL = st-flash
CFLAGS = -mthumb -mcpu=cortex-m3
all: app.bin
crt.o: crt.s
    $(AS) -o crt.o crt.s
main.o: main.c
    $(CC) $(CFLAGS) -c -o main.o main.c
app.elf: linker.ld crt.o main.o
    $(LD) -T linker.ld -o app.elf crt.o main.o
app.bin: app.elf
    $(BIN) -O binary app.elf app.bin
clean:
    rm -f *.o *.elf *.bin
flash: app.bin
    $(STL) write app.bin 0x8000000
erase:
    $(STL) erase
This makefile can be used for multiple things: for build, clean, upload or erase target. If you copy&paste the content from above, be careful at tab characters, could be replaced by spaces and make utility does not like this.

For build, cd to the files directory, then use any of these, there is no difference:

make
make all
When need the removal of the output files, use:

make clean
Connect the ST-Link v2
Before uploading the code into the board, you need to seat both jumpers towards the micro-USB connector, then connect the debugger to the Blue Pill like in the picture:

Blue Pill  debugger ST Link v2 Connections
Caution! Do not use two USB connection at the same time (on debugger and on-board) because you risk to fry the board or PC’s USB ports. Disconnect one before plugging-in the other one.

For uploading the binary to the Blue Pill, respectively complete target erase, use either:

make flash
make erase
Let’s make the LED blinking
The Blue Pill has two LEDs, one for power and one driven by MCU, connected to port C, bit 13, as we can see in the schematic got from https://vcc-gnd.world.taobao.com/

Schematic and documentation
For making the LED to blink we need to configure GPIO port C13 and add some delay. We see in the schematic that LED turns on when GPIO pin is ‘0’ logic and turns off when ‘1’.

The real story is a bit longer, but I’ll tell you here the short version. To make a GPIO toggling in STM32, you need to work with two peripherals: RCC (reset and clock control) and GPIOx (general purpose input/output). The RCC is necessary because the GPIO has disabled clock by default.

In the STM32F10xxx reference manual at page 51 is the memory map, where we find out the GPIO Port C has the base address at 0x4001-1000 and the RCC is at 0x4002-1000.

First, we are going to activate the clock for GPIOC port. For that, we set bit IOPCEN in register RCC_APB2ENR (offset 0x18, bit 4).

Next, we need to configure the port C as push-pull output. We do this by writing into register GPIOC_CRH (offset 0x04), CNF = 0b00 (push-pull output) and MODE = 0b10 (low speed). Thus, for port C13 bits 23:20 are going to be set to 0x2.

The code
So, at the beginning of main.c let’s add the registers and bits definitions:

#include <stdint.h>
// register address
#define RCC_BASE      0x40021000
#define GPIOC_BASE    0x40011000
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)
#define GPIOC_CRH     *(volatile uint32_t *)(GPIOC_BASE + 0x04)
#define GPIOC_ODR     *(volatile uint32_t *)(GPIOC_BASE + 0x0C)
// bit fields
#define RCC_IOPCEN   (1<<4)
#define GPIOC13      (1UL<<13)
And finally, to turn ‘1’ and ‘0’ the pin we set/reset the bit 13 in GPIOC_ODR (at offset 0x0C). So, we replace the main() function in main.c:

void main(void)
{
    RCC_APB2ENR |= RCC_IOPCEN;
    GPIOC_CRH   &= 0xFF0FFFFF;
    GPIOC_CRH   |= 0x00200000;
    while(1)
    {
        GPIOC_ODR |=  GPIOC13;
        for (int i = 0; i < 500000; i++); // arbitrary delay
        GPIOC_ODR &= ~GPIOC13;
        for (int i = 0; i < 500000; i++); // arbitrary delay
    }
}
Build, upload & blink
Now, we build and flash the board:

make flash
After flashing is complete, if nothing happens, press the RESET button. The LED is blinking at ~1Hz.

If does not work, try cleaning up, erasing the target, then re-upload:

make clean
make erase
make flash
That’s all. You can find the code on github. Enjoy!




Bare Metal STM32 Programming and a Quadcopters Awakening
2019-03-05 - Tim Schumacher
QUADCOPTER PROGRAMMING
Last year I got the Crazepony Mini quadcopter, and just recently I figured out how to program it. I will show my progress in this post, and it will also serve as a getting started guide for programming STM32 microcontrollers. We will build a minimal working example to blink an LED with only the GNU ARM compiler (gcc) and without any library dependencies.

Crazepony Mini quadcopter

If you are just here for the STM32 programming you can skip this part.

The Crazepony Mini #
You can get the quadcopter on eBay for around 100 €. It ships with a remote control that wirelessly connects to the drone. Like the drone the remote control has no casing, which I find for the drone looks really good, but unfortunately makes holding the remote control difficult. Both devices have firmware by Crazepony installed for which they published the source on GitHub. They seem to be using the Keil IDE. Although it’s mainly made for educational purpose you can totally fly the quadcopter, and I had fun with it for a while.

Quadcopter remote control

Mine came with two extra motors for replacement, and you can get super cheap replacement propellers from AliExpress. The flight time is supposed to be up to 6 minutes, so I got some extra batteries and a charger so that I can quickly switch batteries when power runs out.

Batteries and charger

So let’s get into the technical details of the drone. The chip on the quadcopter is a 32-bit ARM Cortex-M processor with 64 KiB flash memory. On board are a wireless module, a 3-axis digital compass which detects orientation using the earth’s magnetic field, an altimeter that measures height by air pressure and an accelerometer and gyro sensor combined into one chip. In this post we will be using the LEDs on one of the 4 arms and most importantly the integrated USB to serial bridge to flash our program. You can get the schematic for the quadcopter here.

The bootloader #
There are a few ways to flash firmware onto STM32 microcontrollers. You can use one of the debugging interfaces JTAG or Serial Wire Debug (SWD) which also have support for on-chip debugging. Note that SWD despite it’s name does not use the standard serial port of the chip. In fact, using the serial connection also known as UART connection is another and the most basic way to flash the controller. That’s what we will be using.

If you want to follow along you should get the reference manual, which is meant for application developers, as well as the datasheet for your processor. For the processor on my drone the STM32F103 reference manual and the STM32F103x8 datasheet apply. You can find them by searching for your processor name together with “reference manual” or “datasheet”.

Boot modes

The STM32 processors have three boot modes as shown in this table from the reference manual. Booting from flash is the default mode and what we will be using to run our program once we uploaded it to flash memory. To flash the microcontroller over UART we will have to boot the processor in system memory mode. System memory refers to a part of ROM on the chip which contains a bootloader since the manufacturing. That means there’s no way to brick the controller. We will be speaking to that bootloader over UART and tell it to write our firmware to flash.

USB to serial converter #
If your computer doesn’t have a serial port which is likely nowadays you need a USB to serial bridge to connect the board to your PC. Fortunately most STM32 boards like the popular Blue Pill board have a USB to UART bridge with the CP2102 chip on board. Below you can see the bridge in the schematic of my drone. If your board has an USB port it most likely connects to a bridge as well. Otherwise you can get a dongle with the CP2102 chip for around 8 € on Amazon. The chip connects to the 5 volts and ground pins to power the microcontroller. And the RXD and TXD serial lines connect to pins A9 and A10 (TXD_BT and RXD_BT in the schematic) on the processor respectively.

Drone schematic with CP2102 chip

From the pin table from the processors datasheet:

Serial pins on processor

Don’t get confused with the RX reciever and TX transmitter pins. The pins on the processor are named from the processors perspective and the pins on the bridge are named from the computers perspetive.

Flashing the processor #
Before we can start writing to flash we need to boot the processor in system mode. As you can tell from the boot modes table we need to set the B00T0 pin to high. On my microcontroller the BOOT0 pin is actually connected to the CP2102 chip as shown in the schematic so this happens automatically when I upload firmware. On some boards like the Blue Pill you can set the BOOT0 and BOOT1 pins using those yellow jumpers.

Blue Pill board

Once the BOOT0 pin is set correctly you can connect the board to your computer to power it. The microcontroller boots and a serial device should appear at /dev.

$ ls /dev | grep USB
ttyUSB0
We will use the stm32loader Python script to upload our program. It requires the pySerial library. A normal user can’t write to /dev/ttyUSB0 so you either need to run it as root or add your user to the dialout or uucp group depending on your distro which gives a user access to the serial ports. You might need to login again afterwards. If you are having issues with the script on Arch Linux consider trying my patch.

sudo adduser tim dialout
The stm32loader.py script can download the current firmware so you might want to do that before flashing your own. To upload your own firmware you would pass it the path to the device and the binary file to write. -e erases the previous binary -w means write and -v will download after uploading and verify that the upload worked.

./stm32loader.py -p /dev/ttyUSB0 -ewv firmware.bin
Startup assembly code #
For now we will start with a C program that does nothing. We call it main.c and it will just stay in an endless while loop.

void main ()
{
    while (1);
}
Although this alone can’t run on the microcontroller yet. The processor starts executing at a very specific entry point in flash memory. We need finer memory control to set this up correctly and therefore we will use assembly. From the reference manual:

After this startup delay has elapsed, the CPU fetches the top-of-stack value from address 0x0000 0000, then starts code execution from the boot memory starting from 0x0000 0004.

The assembly code will go in a file called startup.s.

.cpu cortex-m3
.thumb

// Stack top address (end of 20K RAM)
.word 0x20005000
.word _reset

.thumb_func
_reset:
    bl main
    b .
The first two lines configure the assembler for a Cortex-M3 processor with the Thumb instruction set. On ARM a word is 4 bytes. The first .word line is writing the value 0x20005000 to the beginning of the output binary. When we flash the binary this will become position 0x0000 0000 in flash memory. Note the quote above, that’s the position where the processor reads the stack’s top address. As you might know the program stack lives at the very end of memory and grows backwards, the end of memory on my processor is 0x20005000. You might want to adjust this for your processor, I will get into memory layout later.

The next word we write is the address of the _reset function which is defined below. So this address ends up at position 0x0000 0004, you probably see the pattern now. That is where the processor starts code execution.

The .thumb_func directive is required for functions to run on a processor with the Thumb instruction set like the STM32. For our C code this will be implied for all functions by passing a compiler flag. The _reset function calls the main function defined in the C program and afterwards enters an endless loop.

You will often encounter endless loops in microprocessor programming. They are basically the microprocessor equivalent to exit(0). When you want the program to end or there is some failure you enter an endless loop to stop the control flow. In this state a reset is required to restart the program.

Linker script #
The last file required is a linker script which specifies the processor’s memory layout to the linker. You can see the relevant section of the memory map from the datasheet below.

Memory map

As you can see the flash memory starts at 0x0800 0000 and the SRAM starts at 0x2000 0000. I could also take from the datasheet that my processor has 64 KiB of flash and 20 KiB of SRAM. This information goes into the linker script called linker.ld.

MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
  RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
}
Now that we have a C program, the startup assembly code and the linker script ready, we can start compiling the final binary.

Compiling for the STM32 #
To compile for ARM processors you can get the GNU ARM toolchain. It provides a C compiler arm-none-eabi-gcc, a linker arm-none-eabi-ld, etc. to cross-compile for ARM. On Debian the package is called gcc-arm-none-eabi and on Arch Linux you need the packages arm-none-eabi-gcc and arm-none-eabi-newlib.

First we compile the assembly and C code to object files. The -mcpu=cortex-m3 and -mthumb flags are required for the C compiler to compile for STM32 processors.

arm-none-eabi-as -o startup.o startup.s
arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -c -o main.o main.c
When we link both object files we pass the linker script with the -T option.

arm-none-eabi-ld -T linker.ld -o main.elf startup.o main.o
The ELF format is designed for binaries that run on an operating system. We need to extract the actual raw binary executable, starting with the eight bytes we set in the assembly code. The objcopy utility can do that.

arm-none-eabi-objcopy -O binary main.elf main.bin
If everything compiled without errors you can now flash your first firmware and hope for the best.

./stm32loader.py -p /dev/ttyUSB0 -ewv main.bin
If everything went right nothing should happen, hurray!

Controlling output pins #
On my drone the two LEDs on the top and bottom of one of the arms are connected to pin PA11 of the processor.

LED connection to processor pin

To turn them on the pin has to be configured as an output pin first. Configuring or toggling any of the PA pins is done by writing to the GPIOA registers. PB pins would be controlled through the GPIOB registers and so on. In the system architecture figure from the reference manual below, you can see that the GPIOA registers are part of the APB2 domain.

System architecture with GPIOA register

Clock enable register #
To write to the GPIOA registers the corresponding clock has to be turned on first. As documented in the reference manual the APB2 peripheral clock enable register is accessible at address 0x4002 1018. It is 32 bits long and bit 2 controls the GPIOA clock.

Bit 2 IOPAEN: IO port A clock enable
Set and cleared by software.
0: IO port A clock disabled
1: IO port A clock enabled

So to turn on the GPIOA clock we can set the 32 bit value 0x00000004 (bit 2 set) at address 0x4002 1018.

*(volatile uint32_t *)0x40021018 = 0x00000004;
The volatile keyword tells the compiler that the value may change at any time because it’s accessed by other hardware. Without the keyword the compiler could notice that the value is never used by any of our code and optimize the line away. Thanks to dmitrygr for pointing this out in the comments.

Port configuration register #
To configure the PA pins there are the GPIOA port configuration register low for pins 0 to 7 and the GPIOA port configuration register high for pins 8 to 15. Since on my drone I want to configure pin PA11 I need the high register at address 0x4001 0804. It is 32 bits as well with four bits for each pin. Bits 12 to 15 are for pin PA11. The first two bits are specifying the MODE and the latter two bits the CNF value. From the reference manual:

CNFy[1:0]: Port x configuration bits (y= 8 .. 15)
These bits are written by software to configure the corresponding I/O port.
Refer to Table 20: Port bit configuration table.
In input mode (MODE[1:0] = 00):
00: Analog mode
01: Floating input (reset state)
10: Input with pull-up / pull-down
11: Reserved
In output mode (MODE[1:0] > 00):
00: General purpose output push-pull
01: General purpose output Open-drain
10: Alternate function output Push-pull
11: Alternate function output Open-drain

MODEy[1:0]: Port x mode bits (y= 8 .. 15)
These bits are written by software to configure the corresponding I/O port.
Refer to Table 20: Port bit configuration table.
00: Input mode (reset state)
01: Output mode, max speed 10 MHz.
10: Output mode, max speed 2 MHz.
11: Output mode, max speed 50 MHz.

We want to configure the pin as an output and the frequency doesn’t matter for an LED so we go with the lowest 2 MHz. This means the MODE value will be 10. Because our MODE value is greater than 0 the second table applies for the CNF value. We choose a push-pull output for the LED, so the CNF value is 00.

So the four bits we need for the pin are 0010. In hexadecimal the four bits of a pin are one digit. Because we don’t want to touch the other pins configuration settings we first clear the four bits of pin PA11 and then set them using a binary or.

*(volatile uint32_t *)0x40010804 &= 0xFFFF0FFF;
*(volatile uint32_t *)0x40010804 |= 0x00002000;
Port bit set/reset registers #
Finally, to turn on the pin, there is the GPIOA port bit set/reset register at address 0x4001 0810. Bits 0 to 15 correspond to the different pins.

BSy: Port x Set bit y (y= 0 .. 15)
These bits are write-only and can be accessed in Word mode only.
0: No action on the corresponding ODRx bit
1: Set the corresponding ODRx bit

*(volatile uint32_t *)0x40010810 = (1<<11);
For turning it off you can use the GPIOA port bit reset register at 0x4001 0814.

*(volatile uint32_t *)0x40010814 = (1<<11);
Putting it all together #
Here is the complete main.c program to turn the PA11 pin on and off. The stdint.h header is required for the uint32_t type. A for-loop is used as a simple way to get some delay.

#include <stdint.h>

void main ()
{
    // Enable I/O port A clock
    *(volatile uint32_t *)0x40021018 = 0x00000004;

    // Configure pin 11 as push-pull output
    *(volatile uint32_t *)0x40010804 &= 0xFFFF0FFF;
    *(volatile uint32_t *)0x40010804 |= 0x00002000;

    while (1) {
        *(volatile uint32_t *)0x40010810 = (1<<11); // Set pin 11
        for (int i = 0; i < 1000000; i++);
        *(volatile uint32_t *)0x40010814 = (1<<11); // Unset pin 11
        for (int i = 0; i < 1000000; i++);
    }
}
Once you’ve adjusted everything for your board compile it and flash it. If everything went right the output pin should be powered and if an LED is connected it should start blinking.

You can find the complete source code on GitHub, including a Makefile. All the manuals quoted in this post can also be found in the repository.

In the next post, we will learn how to use the CMSIS library. It defines pointers with meaningful names for the registeres we saw in this article, so we don’t have to hardcode all the addresses. And it provides functions for properly timed delay so we can get rid of the for-loops which lack a fixed delay.

You can leave comments on Hacker News. I’d be happy about any feedback and corrections.






Going Bare Metal on STM32
 31/05/2019  Jan
During the last couple of months I have started using my Blue Pill boards more often for even the smallest of projects and tests. When using them with STM32duino in the Arduino IDE they are just as easy to program as a normal Arduino UNO. In a previous post I gave more of an introduction if you are interested. But more recently I wanted to experiment with programming the STM32F103 without using any IDE but only a text editor and the GNU compiler for ARM processors. The main driver for this was to eventually start using other frameworks and in particular libopencm3. Going this route, the idea was also to learn a bit more about setting up the tool chain and not being bound to some specific IDE. This forced me to take a step back and take a closer look at the inner workings of how a C/C++ program is compiled, uploaded and executed on the microcontroller. After a decent amount of time spent reading up on the topic and writing test code I now have a more in depth understanding of the process. In this post I would like to share my newly gained knowledge. I will try to keep it as simple as possible but some basic understanding of programming and microcontrollers would be helpful to follow along. If you want to try out the examples you would also need a development board (would recommend the blue pill) and a STLink programmer.

I used the STM32F103 MCU and therefore most of the information in this discussion is based on it. But since it is an ARM Cortex M3 device, a lot of the details discussed, is also relevant to many other microcontrollers based on this architecture.

We will start the discussion with a bit of theory to get familiar with a few basic concepts, followed by some examples. For the theory we will look briefly at the following topics:

The memory map
Boot sequence
Vector table
Organisation of memory
Memory alignment
Instruction set
Building the program
From there we will start to configure the tools we need and create a simple example.

Memory Map
During our discussion we regularly refer to the memory areas of the Cortex M3 so it’s a good idea to start by having a quick overview of how it is laid out. Cortex M3 devices, being 32bit, has a linear addressable memory space of 4GB. The diagram below shows how the main areas are mapped out:


By reading or writing to the these addresses we have access to the different memories and peripherals of the MCU. For this discussion however the sections of interest are the two bottom blocks, the Code and SRAM sections. The Code section is where most of the flash memory we have access to is located and as it’s name suggests the SRAM section is where the RAM is located.
If we zoom in on the Code section we can see that it’s divided into even smaller sections. Here the sections of interest to us are the System, Flash and Aliased memory.
The System memory is where things like the factory bootloader are stored and to which we typically do not have write access.
The Flash memory section is where our program instructions and some constant data are typically stored and to this memory we mostly have complete write and read access. The range of this memory depends on the available flash memory of the device being used. In the case of the STM32F103CB this memory can extends up to address 0x0801 FFFF which provides 0x0801 FFFF – 0x0800 0000 = 0x001F FFFF = 131.071kB of memory (the datasheet guarantees at least 128kB).
The last section, the Aliased memory can be mapped to the Flash, System memory or the SRAM depending on the boot mode selected with the boot pins. Typically we can not read or write this memory directly, how this works and why it’s done will become more clear when we speak about the boot sequence.

Boot Sequence
When power is applied to the MCU the Program Counter (PC) value will be 0 and will therefore start at address 0x0. Remember that the Program Counter holds the address of the next instruction to be performed. The value it expects at this address (0x0) would be the address that marks the top of the Stack memory (which is part of the SRAM). This address is then copied to the Stack Pointer (SP) register for later use. The Program Counter then steps to the next address which is 0x0000 0004 and expects the address of the reset handler at this location. This is typically the start address of where our program is stored. The MCU will then update the Program Counter with this address which means that the next instruction to be executed will be the first instruction in our program.
In our discussion on the memory we said that our program normally lives in the Flash memory which starts at 0x0800 0000 and we also said that we can’t write to the Aliased memory (which starts at 0x0). So how do we set the values of address 0x0000 0004 (which is in Aliased memory space) to point to the start of our program? Well, this is why the Aliased memory can be mapped, by selecting (with BOOT0 and BOOT1 pins) to boot from the Flash memory the Flash memory is actually mapped to the Aliased memory. This means that when the Program Counter looks at address 0x0 it’s actually looking at address 0x0800 0000. So since we can write to the Flash memory section we can provide the starting address of our program at address 0x0800 0004 which would be the same as 0x0000 0004. Typically the part of our program located at this address is called the startup code and will perform some initialization steps and then call the main() function in our C/C++ code.

Vector Table
The vector table is a section of our flash memory that mostly holds the addresses of various handlers. In the boot sequence we said that the reset handler is located at address 0x0000 0004 and holds the address of our startup code. Well this is part of the vector table and for the STM32F103 it starts at address 0x0 and extends up to address 0x0000 014C. As a reminder, this is aliased memory, so if we are booting from Flash memory it would actually mean that the vector table extends from 0x0800 0000 to 0x0800 014C. If you have used interrupts before then this is also where the handlers (address to the code that will be run) is set. For the moment we only care about the reset handler but the datasheet gives some details about the rest if you are interested.

Organising the Memory
Earlier we talked about how the memory in the MCU is mapped out to the different regions of the FLASH and SRAM memory. When our program is compiled the compiler also organises the program into different sections which we would need to fit to these regions. The most popular of these sections are the TEXT, DATA and BSS sections. This is done to separate for example, things like the instructions which is typically in read only memory from variables which should be in read/write memory. The read only memory is typically FLASH memory but does not have to be, it’s also possible to load instructions to the SRAM and execute them from there, but in this post we will use the FLASH memory as our read only memory.
The region that holds the instructions of our program goes into the TEXT section. The TEXT section is located in the FLASH memory and is typically only written to when we upload our program to the MCU. If our program contains constants the compiler knows that these values will always stay the same and there is no need for them to be in RAM. Therefore they can also be placed in FLASH memory and forms part of the RODATA (Read Only Data) section.
If our program had global or static variables (which lives for the whole lifetime of our program) then the compiler can already assign space (addresses) in RAM for it. The addresses it assigns to these variables will be in the BSS section which is in RAM memory.
Should we have global or static variables which have also been initialized (given a value) in our program then the compiler will assign the address in the DATA section and the value itself in the LMA (Load Memory Address) DATA section. When the MCU boots-up these values should then be copied from the LMA DATA to VMA (Virtual Memory Address) DATA section.


Memory Alignment
For the processor to efficiently fetch data the address from where it fetches data should be a multiple of the data type size. For example, if we retrieve a 32bit (word) value then the address must be a multiple of 4. So valid addresses would be 0x0800 0000, 0x0800 0004, etc. When using a 16 bit (half word) value the address must be a multiple of 2 and for 8bit addresses any address can be used. This is known as aligned memory access.


Looking at the image above, imagine each block represents one byte of memory and the address of the byte is to the left of it. Green arrows would indicate valid addresses for word (32 bit) values. Yellow arrows would be valid for half word (16 bit) values and blue arrows for byte values.

If we try to read data from unaligned memory the processor would need to fetch the data twice and we would get a performance penalty. One solution to this problem is to use alignment instructions when data is assigned an address. Such an instruction would typically round the address up to the closest multiple of the data size. This works but at the cost of creating unused memory locations. In the image below we can see the effect of memory alignment.


Take as example address 0x0000 0008 which holds a 2 byte value, the next value to be stored is a 4 byte value. But since we need to align the memory we can’t store it at address 0x0000 000A, the first available address would be 0x0000 000C which creates 2 bytes of unused space.

Normally the alignment is handled by the compiler but knowing about it helps to understand some other concepts. We can also use this to improve the memory use of our programs by packing our data in such a way that we don’t fragment the memory too much.

Thumb Instructions
As we know by now the processor in the STM32F103 belongs to the Cortex M3 series which uses the ARMv7-M architecture. This means that these processors do not use the full ARM instruction set but rather the optimized Thumb instruction set. Interesting play on words there, from Arm to smaller Thumb… Anyway the Thumb instruction set supports both 16bit and 32bit instructions, the advantage of 16bit instructions are that they use less program memory. While in many cases using 32bit instructions can be faster, so by being able to use both we get the best of both worlds. Some processors supports both ARM and Thumb instruction sets and the set being used can be selected during branch (jumping to another address) instructions. If the LSB (least significant bit) of the address to which the program branches is ‘0’ then the ARM instruction set is used. If the LSB is ‘1’ then the Thumb instruction set is used. Since the instructions are always aligned to 16bit or 32bit memory boundaries, changing the LSB has does not affect the destination address.
Knowing this is important because even though our Cortex M3 processor only supports Thumb instructions we still need to follow this convention to prevent the processor from generating an error.

Building our program code
If you are familiar with how to build your program from source code to binary then you can skip this section, this is only to refresh our memory on the process. Creating a binary file from our program code (C/C++ or assembly) is a multi-step process. As a first step the compiler runs a pre-processor through the code, the job of the pre-processor is to replace all the macros (like #include<…>, #define …, etc.) with the actual code. Then the source code files gets compiled into object files. Technically C/C++ code files are first compiled to assembly and from assembly to machine code. If our program also contain assembly files then they are obviously compiled directly to machine code. The compiler creates a machine code object file for each input source file. These object files contains the instructions and data for the various functions of our program. Typically these object files also reference functions and data from other object files and therefore they need to linked together, this is the task of the linker.
In addition to object files, the linker also take as input a linker script file (if none is provided it uses a default script) which is used to set some rules for the linker. Later in this post we will also take a closer look at linker scripts.
The output from the linker is typically an executable file and in our case will be a .elf (Executable and Linkable Format). Since this is an executable file it contains more than just the instructions and data of our program. When flashing our program to the MCU we want only the instructions and data used by our program. To do this we use a special tool in the GNU toolset called “objcopy” to create a binary (.bin) file which we can then flash to the MCU.


Installing GNU Tools for ARM Embedded processors
In order to start building some code examples we need a compiler and linker that can compile our code for ARM processors. One of the most popular options for doing this is the GNU Tools for ARM Embedded processors which can be downloaded from https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads.
After you downloaded and installed it you can find all the tools in the bin folder (for me it is C:\Program Files (x86)\GNU Tools ARM Embedded\6 2017-q2-update\bin).
Most C/C++ compilers provide separate compiler, assembler and linker tools. GNU C/C++ compiler also provides these tools as separate packages but they also include a combined tool, GCC. Using GCC we can provide all our source files, header files and linker script and build the binary in one go. However for the purpose of this post we will do it step by step to make sure we understand each step of the process.
If you take a look at the tools in the installed bin folder you will notice they all have the “arm-none-eabi-” prefix. This prefix helps to make sure that we cross compile for our MCU and not for our native or another system. For example, it could be that you also have GNU GCC installed for x86 and the path environment variable set, running gcc would then run the x86 compiler and not the ARM compiler. By adding the prefix we can distinguish between them. The first part of the prefix (“arm”) indicates the compiler is for ARM processor, the second part (“none”) means it does not target any specific system and the last part (“eabi”) tells us that it uses the Embedded-Application Binary Interface.

Example code
As an example we will create a basic startup program that will perform some initialization and then call the main() function. Basic startup programs are often written in assembly but to keep things simple we will be using C.

startup.c

/* 
 * Basic start file example
 * Jan Swanepoel, 2019
 * 
*/

// Create references to symbols defined in the linker script 
extern unsigned int _data_start;	
extern unsigned int _data_end;
extern unsigned int _data_load;
extern unsigned int _bss_start;
extern unsigned int _bss_end;

void startup();			// Function prototype (forward declaration) for startup function
int main();			// Function prototype for main function

// Below we create an array of pointers which would form our vector table
// We use __attribute__ ((section(".vectors"))) to tell the compiler that we want the
// array to be placed in a memory section that we call ".vectors"
unsigned int * vectors[2] __attribute__ ((section(".vectors"))) = 
{
    (unsigned int *)	0x20005000,  	// Address of top of stack. 20kB = 1024 x 20 = 20480 bytes = 0x5000 
    (unsigned int *)    startup     	// Address of the reset handler which is also our startup function
};

// The startup function, address was provided in the vector table	
void startup()
{
	volatile unsigned int *src, *dest;

	// Copy data section values from load time memory address (LMA) to their address in SRAM 
	for (src = &_data_load, dest = &_data_start; dest < &_data_end; src++, dest++) 
		*dest = *src;
	
	// Initialize all uninitialized variables (bss section) to 0
	for (dest = &_bss_start; dest < &_bss_end; dest++)
		*dest = 0;

	// Calling the main function
	main();
	
	while(1);	// Normally main() should never return, but just incase we loop infinitely
}
Then we also create a very basic main() function in main.c:

const int a = 7;		// Will be placed in rodata
int b = 3;			// Will be placed in data (LMA)
int c = 0;			// Will be placed in bss
int d;				// Will be placed in bss (but first in COMMON)

int main()
{	
	c = a + b;
	
	while (1);
	
	return 0;
}
Lets first take a look at the startup.c file.
It starts with adding some external references to symbols that are defined in the linker script. You will see them when we look at the linker script file, all that they do is point to some address as their names suggests. The “_data_start” symbol points to the address of where the DATA section in SRAM starts, “_data_end” to where it ends, “_data_load” to where the LMA for the DATA section starts and so forth.
Next up, we forward declare a function called startup() which will be our main entry point and then our main() function which will be called after initialization.
Next we create an array of pointers that will be our vector table. By adding the “attribute ((section(“.vectors”)))” we tell the compiler that we want this array to be in a section called “.vectors” in our object file. In this array we specify the top of the stack address at index 0 and the address of our startup function at index 1. Since the STM32F103C8 (used in the Blue Pill) has 20kB of SRAM which starts at address 0x2000 0000, the top of the SRAM which is where the top of the stack would be is at address 0x2000 5000.
Then we get to the startup function. The first task of this function is to copy the values of initialized variables (DATA section) from their load memory address in FLASH to their virtual memory address in RAM. This is done with a for loop and using the addresses provided by the external references. The second task of the startup function is to initialize all the un-initialized variables (BSS section), which is as simple as just setting their values to 0. Once this is done, our basic initialization is completed and we pass control over to the main function.
So as on overview, the main tasks of our startup program is to:

create the vector table to provide the address to the top of the stack and to the reset handler
copy initialization values to the initialized global variables (DATA section variables)
zero the uninitialized global variables (BSS section variables)
call the main() function
The main.c file is really simple and just declares 1 global constant and 3 global variables followed by the main function which adds a and b and places the result in c. The constant a, is global and can never have it’s value changed, therefore it will be stored in FLASH memory in the RODATA section. Variable b is initialized to a value 3, the value 3 would thus be stored in the LMA DATA section and the address of where this variable will exist in SRAM will be in the VMA DATA section. Variables c and d will both end up in the BSS section, even though c is initialized to 0 it will still go to BSS which will anyway have all its values initialized to 0.

Linker Script
The linker takes many object files as input and produces a single output executable file. We use the linker script in combination with the linker to have more control over how the output file should be compiled.

Now lets look at our linker script file:

MEMORY
{
	rom (rx) : ORIGIN = 0x08000000, LENGTH = 64K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
    .text : 				/* Define output file TEXT section */
    {
        *(.vectors) 			/* Vector table */
        *(.text)      			/* Program code */
		. = ALIGN(4);		/* Make sure data that follows are aligned to 4 byte boundary */
		*(.rodata)		/* Read only, section set asside for constants */
    } >rom
	
	
	.data :				/* Define output file DATA section */
	{
		_data_start = .;	/* Get the memory address (VMA) for start of section .data */
		*(.data)		/* Initialized static and global variable values */
		. = ALIGN(4); 
		_data_end = .;		/* Get the memory address (VMA) for end of section .data */
	} >ram AT >rom			/* After AT we specify the load-time location */
	
	_data_load = LOADADDR(.data);	/* Get the load memory address (LMA) for section .data */
	
	
	.bss :				/* Define output file BSS section */
	{
		_bss_start = .;		/* Get memory address of start of bss section */
		*(.bss)			/* Uninitialized static and global variables */
		*(COMMON)		/* Uninitialized variables are placed in COMMON section for object files */
		. = ALIGN(4);	
		_bss_end = .;		/* Get memory address of end of bss section */
	} >ram
	
}  
Our basic linker script consists of two commands, a MEMORY and a SECTIONS command.
In the MEMORY command we define the memory blocks of our MCU which consists of a ROM block starting at 0x0800 0000 with a length of 64KB and a RAM block starting at 0x2000 0000 with a length of 20KB. The syntax for defining these blocks are:

name [(attr)] : ORIGIN = origin, LENGTH = len

In our file we picked a name “rom” and provided the “rx” attributes which means this block is readable and executable to create the FLASH memory region. For the SRAM region we used “ram” as name followed by “rwx” attributes to indicate that this block is readable, writable and executable.

The SECTIONS command is used by the linker to map input sections to output sections, and describes how to place the output sections in memory. In our file we have 3 output sections named, “.text”, “.data” and “.bss”. This means that our output file will have only these 3 sections. As the linker processes through each of the input object files it will look for the sections defined within these 3 output sections and if a match is found it will be added to the relevant output file section. Take the “.vectors” section for example, this is a section we defined in our startup.c code, this means that the array of pointers we defined to be our vector table will be placed here. Another example is the “.text” section, this one we did not explicitly define but the compiler did it in the background when we compiled to an object file. So the linker will place all the “.text” sections from the different input files at this location in the output file. It is also important to note that the order in which we defined the sections matter. At the end of each section we also use the memory blocks we created with the MEMORY command to define where these output sections should live. If you look through these 3 output sections you would also recognize the sections we discussed earlier in the post. One section that might be new is the COMMON section, it refers to global variables which are not initialized to 0 and not yet allocated (like variable ‘d’). When objects are linked together, COMMON is merged with BSS which means these variables will then be allocated. The startup code will then also initialize them to 0 like the rest in the BSS section.
In all 3 sections you will find the line, “. = ALIGN(4)” which aligns the location counter (donated with “.”) to the next word. This is done to make sure that the values which are placed following this function is word aligned (remember we discussed this earlier in the post). Take the “.text” section for example, just before the start of the “.rodata” section (which will contain the constant values) we make sure that we align the location counter to the next word. This will ensure that the values to follow will be word aligned.
In the “.data” and “.bss” sections you will find some of the external references we used in the startup function in the startup.c file. Here we used the location counter (“.”) again to get the start and end addresses of the sections. To get the LMA address for the “.data” section we used the LOADADDR function.
Linker scripts can be difficult to grasp at first but as you spend time with them they slowly starts to get clearer. Luckily it’s also not something you need to write every single time, once you have it for your target MCU then you can just reuse it for every new project. The one we wrote here is pretty basic but could be okay for most simple C projects. For C++ projects it might require some tweaking to support some of the more advanced features.

Building our example
So now finally we can get to building our code. Typically we would use a make file to perform the build, but since this is a very small program and to keep it simple we will just use a batch file. This is how our file looks:

ECHO OFF
CLS 
SET PATH=%PATH%;c:\Program Files (x86)\GNU Tools ARM Embedded\6 2017-q2-update\bin\
ECHO ON

REM Compiling
arm-none-eabi-gcc -O0 -Wall -c -g -mcpu=cortex-m3 -mthumb main.c -o bin\main.o
arm-none-eabi-gcc -O0 -Wall -c -g -mcpu=cortex-m3 -mthumb startup.c -o bin\startup.o

REM Linking
arm-none-eabi-ld -o bin\prog.elf -T stm32f103.ld bin\startup.o bin\main.o 
arm-none-eabi-objcopy bin\prog.elf bin\prog.bin -O binary

REM Disassembling
arm-none-eabi-objdump -D -h bin\startup.o > bin\startup.list
arm-none-eabi-objdump -D -h bin\main.o > bin\main.list
arm-none-eabi-objdump -D -h bin\prog.elf > bin\prog.list
arm-none-eabi-nm --numeric-sort bin\prog.elf

pause
At the top we add the GNU ARM Tools path to the PATH environment variable (will only be valid for this session) so that we can easily run the different tools.
Next the actual build process starts with compiling our source files to object files. This is done with “gcc” and the following options:

Ox, which sets the optimization level to x, we used 0 here which means we want no optimization. Normally it would be good to use some optimization but in this case we want to disassemble the objects later and therefore want as little as possible optimization.
Wall, turns all compile warnings on.
c, tells GCC to only compile and not to perform the linking (remember GCC normally automatically performs linking).
g, adds extra debugging information to allow us to debug our program later.
mcpu=cortex-m3, indicates to GCC that we are compiling for a Cortex-M3 CPU.
mthumb, tells the compiler to use the Thumb instruction set.
For more details on the options you can also have a look here.

After the compiling is done the linking is performed with “ld”. We pass the linker script file after the “-T” option followed by the object files we just compiled. The output from the linker is the prog.elf executable file. Earlier we mentioned that we need a binary file when flashing to the MCU and therefore we convert the .elf to a .bin file with “objcopy”.

In the last couple of lines we disassemble the object and binary files again with “objdump” and “-D” option. The “-h” option also adds a header to the disassembled files which provides a summary of the memory layout. We only do this so that we can inspect the object and executable files to make sure everything was compiled and linked correctly.

Then finally the “nm” tool at the end just writes the symbols and their addresses in the .elf file to the console, this is also just to help with debugging our program.

If you run the batch file then there should be no warnings or errors and look something like this:


Here we can see all the steps in the building process and at the end the symbols and their addresses in our prog.elf file. As expected we see the vector table starting at address 0x0800 0000 followed by our startup function, main function, constant named “a” and the start of DATA section values (remember we defined _data_load in the linker script). All these are in the address range of our FLASH memory. Then following along we see the start of the DATA section in SRAM at address 0x2000 0000 and this also where global variable “b” will live. Next comes the address to the end of the DATA section and start of the BSS section. We can then see that uninitialized global variables “c” and “d” and the end of the BSS section.
As a first overview we can see that our linker script worked correctly. We could now also open the .list files that we created with a text editor to view them in more detail. As an example we have the first few lines of the prog.list file below:


The first part called “Sections” is the header which was added by the “-h” command line option. It lists the different sections with details about their size, address, offset and alignment. Looking at the “.data” section in the header we can again see the VMA and LMA address which off-course matches with the “nm” output we saw when building the program.
After the header we see again the vectors section which holds the vector table. As expected the value of the first address points to the address of the top of the stack (which we provided in the linker script). The value of the second address in the vector table holds the address of the startup function, but if we cross reference this it might seem to be wrong… The address of our startup function is 0x0800 0008 but the vector table points to address 0x0800 0009, the reason for this is because we are using the Thumb instruction set. Remember from our discussion regarding the use of the Thumb instruction set we said that on a branch instruction the LSB needs to be 1. This is exactly what is happening here and why we will always see this address + 1 value.
Looking at the startup function we can start to see the assembly instructions and their location in memory, here the first ones we can see are all 16 bit instructions due to their half word separation in memory.

As you can see these disassembled files can be very handy to determine if our build process went as expected or just to get a better idea of what our code is really doing. If you are interested in learning assembly these files are also very helpful.

Flashing the MCU
There are more than one way to flash your program to the microcontroller but for this project I preferred the ST-Link programmer since we can also use it to debug our program once it’s flashed. These programmer can be found really cheap online and works really well.


In combination with the ST-Link programmer I also use the ST-Link application from Texan. You might also need to install the ST-Link driver also which can be found here.

The programmer pin-out is marked clearly on the back and all we need to do is to connect them up with the board. If you look at the picture above yellow is GND, green is SWCLK, orange is SWDIO and blue 3.3V. Once you have everything setup you can use the “st-flash” utility to flash your program. For this task I also created a small batch file:

ECHO OFF
SET PATH=%PATH%;E:\Stuff\Programs\STLink\TexaneVersion\stlink-1.3.0-win64\bin\
SET PATH=%PATH%;c:\Program Files (x86)\GNU Tools ARM Embedded\6 2017-q2-update\bin\
set WorkingDir=C:\Users\JaSw\Documents\VSCodeTests\Stm32_Article_1\
ECHO ON

arm-none-eabi-size --format=berkeley %WorkingDir%bin\prog.elf
st-flash write %WorkingDir%bin\prog.bin 0x8000000

pause
The “arm-none-eabi-size” tool provides us with the sizes of the different sections and is purely for extra information. The real flashing happens with the “st-flash” tool which we call with the “write” argument and provide it with our binary file (you would need to edit the path to point to your binary) and location in memory to flash it to.

So whats next…
So now that our code is build and flashed to the microcontroller, how do we know it works. The example program is very simple and does not flash any LED or output anything, so we would not really see anything. We could fix this by just modifying the example code to flash a LED on one of the pins or to use a debugger and step through the instructions. But since this post has grown a bit longer than expected I would rather do a follow-up post in the future (maybe when using libopencm3) and show how this can be done.

This brings me to the end of this post, I hope you found this topic just as exciting and interesting as I did and that it can be useful for your current or future projects.