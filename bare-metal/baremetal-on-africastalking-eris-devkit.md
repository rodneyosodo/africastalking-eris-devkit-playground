# Baremetal on Africastalking-Eris-Devkit

**tags: eris-devkit**

## INTRODUCTION

We are going to build a LED blink app from scratch, using only the manual of the STM32F103C8T6 chip.

## Prerequisites

1. The hardware: the eris-dev-kit and for uploading the firmware we need USB cable.
2. The software: need to have installed the stm32flash and the cross compiler.
3. Prior knowledge: basic Linux operation, not being afraid of command line, C language, basic embedded stuff.

## PROGRAMMING

We create a directory with the followinf four files named:

* `main.c`
* `linker.ld`
* `crt.s`
* `makefile`

### main.c

Lets make the on board LED blink. The eris-dev-kit has two LEDs, one for power and one driven by MCU, connected to port C, bit 13.

For making the LED to blink we need to configure GPIO port C13 and add some delay.

To make a GPIO toggling in eris-dev-kit, you need to work with two peripherals: RCC \(reset and clock control\) and GPIOx \(general purpose input/output\). The RCC is necessary because the GPIO has disabled clock by default.

In the STM32F10xxx reference manual at page 51 is the memory map, where we find out the GPIO Port C has the base address at 0x4001-1000 and the RCC is at 0x4002-1000.

First, we are going to activate the clock for GPIOC port. For that, we set bit IOPCEN in register RCC\_APB2ENR \(offset 0x18, bit 4\).

```text
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
```

And finally, to turn ‘1’ and ‘0’ the pin we set/reset the bit 13 in GPIOC\_ODR \(at offset 0x0C\). So, we replace the main\(\) function in main.c:

### crt.s

C run-time assembly file crt.s

```cpp
// Instruct the assembler to generate thumb code
// for the cortex-M3 core
.cpu cortex-m3
.thumb

// end of 20k RAM
.word 0x20005000
.word _reset
.thumb_func
_reset:
    bl main
    b .
```

The first two lines are instructing the assembler to generate thumb code for the Cortex-M3 core.On ARM a word is 4 bytes.

The thumb code is more compact, but is a bit slower at run time.

As we know by now the processor in the STM32F103 belongs to the Cortex M3 series which uses the ARMv7-M architecture. This means that these processors do not use the full ARM instruction set but rather the optimized Thumb instruction set.

The value written at first .word line will be placed at the very beginning of the binary file at address 0x20005000 and represents the end of RAM. When we flash the binary this will become position 0x0000 0000 in flash memory.

The second .word line is the reset vector and is placed at 0x00000004.

The .thumb\_func directive instructs the assembler to generate a thumb executable code from the subsequent lines.

The last two lines are the jump to main\(\) and the infinite loop, in case someone wants to exit from the main function. You will often encounter endless loops in microprocessor programming. They are basically the microprocessor equivalent to exit\(0\). When you want the program to end or there is some failure you enter an endless loop to stop the control flow. In this state a reset is required to restart the program.

### linker.ld

Memory Map The diagram below shows how the main areas are mapped out:

![](https://i.imgur.com/vpboaLS.png)

Linker script file linker.ld

```cpp
MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
  RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
}
```

This instructs the linker how to place different sections of data into the binary. It usually refers to ROM/FLASH and RAM. As you can see the flash memory starts at 0x0800 0000 and the SRAM starts at 0x2000 0000. I could also take from the datasheet that my processor has 64 KiB of flash and 20 KiB of SRAM.

### Makefile

```cpp
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
AOC = arm-none-eabi-objcopy

KERNEL_BIN = kernel.bin
KERNEL_ELF = kernel.elf
MAIN_FILE = main.c

CFLAGS = -mthumb -mcpu=cortex-m3
AFLAGS = -O binary
FFLAGS = -v -g 0x8000000
USB_PORT = /dev/ttyUSB0

all: $(KERNEL_BIN)

crt.o: crt.s
    $(AS) -o crt.o crt.s

main.o: $(MAIN_FILE)
    $(CC) $(CFLAGS) -c -o main.o $<

$(KERNEL_ELF): linker.ld crt.o main.o
    $(LD) -T $< -o $(KERNEL_ELF) crt.o main.o

$(KERNEL_BIN): $(KERNEL_ELF)
    $(AOC) $(AFLAGS) $< $(KERNEL_BIN)

flash: $(KERNEL_BIN)
    stm32flash -w $< $(FFLAGS) $(USB_PORT)

clean:
    rm -f *.o *.elf *bin

# Dependecy chains
main.o : $(MAIN_FILE)
crt.o : crt.s
```

![](https://i.imgur.com/PUcekmO.png)

The ELF format is designed for binaries that run on an operating system. We need to extract the actual raw binary executable, starting with the eight bytes we set in the assembly code. The objcopy utility can do that.

This `makefile` can be used for multiple things: for build, clean or upload target. Be careful at tabs.

### Boot Sequence

When power is applied to the MCU the Program Counter \(PC\) value will be 0 and will therefore start at address 0x0. The Program Counter holds the address of the next instruction to be performed. The value it expects at this address \(0x0\) would be the address that marks the top of the Stack memory \(which is part of the SRAM\). This address is then copied to the Stack Pointer \(SP\) register for later use. The Program Counter then steps to the next address which is 0x0000 0004 and expects the address of the reset handler at this location.

Our program normally lives in the Flash memory which starts at 0x0800 0000. So how do we set the values of address 0x0000 0004 to point to the start of our program? Well by selecting \(with BOOT0 and BOOT1 pins\) to boot from the Flash memory the Flash memory is actually mapped to the Aliased memory. This means that when the Program Counter looks at address 0x0 it’s actually looking at address 0x0800 0000.

#### Building

```text
make
make all
```

#### Flashing

Before uploading the code into the board, you need to be in BOOT 0 and press reset pin.

```text
make flash
```

#### Cleaning

```text
make clean
```

