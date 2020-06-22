# RUST WITH AFRICASTALKING-ERIS-DEVKIT

#### tags: `eris-devkit`

## INTRODUCTION

Rust is a new language that has been loved by its developers in recent years. And as the language grows, it has started to support a wider set of features, compilation and linking for bare-metal targets. As a new Rustacean, the syntax feels little bit hard to grasp.

I will assume that you know a little bit of Rust. I’m still new to the language and I wont do a good job in explaining core concepts. Feel free to read more other documentation to support my claim

## INSTALLATION

### 1. Toolchain Setup

Like any other language we will setup a toolchain. Installation doesn't require too much effort to get things going. Install rust by reading their [Official Documentaion](https://www.rust-lang.org/tools/install)

### 2. Install the embedded Rust dependencies

* Install the nightly version

  ```text
  rustup install nightly
  rustup default nightly
  ```

* Install the `rust-std` component `thumbv7m-none-eabi` to cross-compile for ARM Cortex-M3 \(the processor\):

```text
rustup target add thumbv7m-none-eabi
```

* Install the arm gdb for debugging

```text
sudo apt install pkg-config cmake libssl-dev zlib1g-dev gdb-multiarch curl git

sudo ln -s /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb
```

### 3. Install ARM Cross-Compiler and Linker

```text
sudo apt install binutils-arm-none-eabi gcc-arm-none-eabi
```

Check ARM Installation

```text
arm-none-eabi-gcc -v
```

### 4. Install OpenOCD For Debugging

```text
sudo apt install openocd
```

### 5. Install stm32flash

Checkout this [repo for installation procedure](https://github.com/ARMinARM/stm32flash)

### 6. Getting the code

```text
git clone https://github.com/0x6f736f646f/africastalking-eris-devkit-playground
cd africastalking-eris-devkit-playground/src/Rust/1BlinkingOnboardLed
```

## PROGRAMMING

**GOAL** : Blink onboard LED using rust

### Cargo

Lets look at the `Cargo.toml` file

```text
# This is information about the project
[package]
name = "a01-blink-onboard-led"
version = "0.1.0"
authors = ["0x6f736f646f <blackd0t@protonmail.com>"]
edition = "2018"


[dependencies]
# provides access to low level ARM CPU registers (used for delay
cortex-m = "0.6.2"

# provides startup code for the ARM CPU
cortex-m-rt = "0.6.11"

cortex-m-semihosting = "0.3.3"

# provies a panic-handler (halting cpu) (required when not using stdlib)

panic-halt = "0.2.0"

embedded-hal = "0.2.3"

panic-semihosting = "0.5.2"

# Gives us access to the STM32F1 registers
[dependencies.stm32f1xx-hal]
version = "0.6.0"
features = ["rt", "stm32f103", "medium"]

# this lets you use `cargo fix`!
[[bin]]
name = "a01-blink-onboard-led"
test = false
bench = false

[profile.release]
# better optimizations
codegen-units = 1
# enable debugging in release mode
debug = true
# link with link time optimization(lto)
lto = true
```

### Memory layout

The `cortex-m-rt` requires `memory.x` which specifies the memory layout of the board

```text
/* Linker script for the STM32F103C8T6 */
MEMORY
{
  FLASH : ORIGIN = 0x08000000, LENGTH = 64K
  RAM : ORIGIN = 0x20000000, LENGTH = 20K
}
```

### Cargo config

When we compile the program using rustc there are many arguments passed. We therefore create a `.cargo/config` file

```text
[target.thumbv7m-none-eabi]
runner = "arm-none-eabi-gdb"

# Cortex-M3
[build]
target = "thumbv7m-none-eabi"

rustflags = [
    # LLD (shipped with the Rust toolchain) is used as the default linker use the Tlink.x script from cortex-m-rt crate
    "-C", "link-arg=-Tlink.x",
]
```

The Rust compiler and the linker will assume that we have an underlying operating system that uses a C runtime by default. So to avoid linkwe errors we compiler for baremetal environment for thumbv7m-none-eabi target, which describes and ARM system.

We had initially installed the thumbv7m-none-eabi target which provides the standard and core library for the system

### Main file

`src/main.rs`

```text
//! Blinks an LED
//! The onboard LED is connected to PC13 as is the case on the blue pill board.

// std and main aren't available for bare metal programs
#![deny(unsafe_code)]
#![no_std]
#![no_main]

// Halt when the program panics. 
use panic_halt as _;

use cortex_m_rt::entry;

use embedded_hal::digital::v2::OutputPin;

use stm32f1xx_hal::{pac, prelude::*, delay::Delay};

#[entry]
fn main() -> ! {
    // Get access to the core peripherals from the cortex-m crate

    let cp = cortex_m::Peripherals::take().unwrap();

    // Get access to the device specific peripherals from the peripheral access crate
    let dp = pac::Peripherals::take().unwrap();

    // Take ownership over the raw flash and rcc devices and convert them into the corresponding
    // HAL structs
    let mut flash = dp.FLASH.constrain();
    let mut rcc = dp.RCC.constrain();

    // Freeze the configuration of all the clocks in the system and store the frozen frequencies in


    let clocks = rcc.cfgr.freeze(&mut flash.acr);

    // Acquire the GPIOC peripheral
    let mut gpioc = dp.GPIOC.split(&mut rcc.apb2);

    // Configure gpio C pin 13 as a push-pull output. The `crh` register is passed to the function
    // in order to configure the port. For pins 0-7, crl should be passed instead.

    let mut led = gpioc.pc13.into_push_pull_output(&mut gpioc.crh);

    // Configure the syst timer to trigger an update every second
    let mut delay = Delay::new(cp.SYST, clocks);

    // Wait for the timer to trigger an update and change the state of the LED

    loop {
        led.set_high().unwrap();
        delay.delay_ms(1_000_u16);
        led.set_low().unwrap();
        delay.delay_ms(1_000_u16);
    }
}
```

Our code does not depend on any operating system features. This means that we can't use files, the network, random numbers, standard output or any feature requiring OS abstraction. `#![no_main]` tells the compiler that we don’t use the default `main` function with the argument vector and return type. This wouldn’t make sense, since we don’t have an OS or other kind of runtime which would call the function and handle the return value. Instead, the `cortex_m_rt` crate contains a minimal runtime and the `#[entry]` macro, which specifies our custom entry function \(`fn main () -> ! {}`\), which we call `main` as well \(don’t be confused\). More information about bare metal software in Rust can be found for example in the [“Writing an OS in Rust”](https://os.phil-opp.com/) Blog.

The ! return type means that the function is diverging, i.e. not allowed to ever return. This is required because the entry point is not called by any function, but invoked directly by the operating system or bootloader. So instead of returning, the entry point should e.g. invoke the exit system call of the operating system. In our case, shutting down the machine could be a reasonable action, since there's nothing left to do if a freestanding binary returns. For now, we fulfill the requirement by looping endlessly.

The panic\_handler attribute defines the function that the compiler should invoke when a panic occurs. The standard library provides its own panic handler function, but in a no\_std environment we need to define it ourselves:

### Makefile

The `src/main.rs` is the brains of our application and this `makefile` is the brawn of the operation

`Makefile`

```text
AOC = arm-none-eabi-objcopy
DISASSEMBLE = arm-none-eabi-objdump
TARGET = thumbv7m-none-eabi

KERNEL_BIN = kernel.bin
KERNEL_HEX = kernel.hex
BUILD_DIR = target/$(TARGET)/debug
MAIN_FILE = src/main.rs

AFLAGS = -O binary
FFLAGS = -v -g 0x8000000
USB_PORT = /dev/ttyUSB0

$(KERNEL_BIN): $(BUILD_DIR) $(MAIN_FILE)
    $(AOC) $(AFLAGS) $(BUILD_DIR)/a01-blink-onboard-led $(KERNEL_BIN)

$(BUILD_DIR): $(MAIN_FILE)
    cargo build

build:
    cargo build

compile:
    $(AOC) $(AFLAGS) $(BUILD_DIR)/a01-blink-onboard-led $(KERNEL_BIN)

hex:
    $(AOC) -O ihex $(BUILD_DIR)/a01-blink-onboard-led $(KERNEL_HEX)

disassemble: $(BUILD_DIR)
    $(DISASSEMBLE) --disassemble $(BUILD_DIR) | less

flash: $(KERNEL_BIN)
    stm32flash -w $< $(FFLAGS) $(USB_PORT)

all: $(KERNEL_BIN)

clean:
    cargo clean
    [ -f $(KERNEL_BIN) ] && echo exists && rm $(KERNEL_BIN) || echo not exists
    [ -f $(KERNEL_HEX) ] && echo exists && rm $(KERNEL_HEX) || echo not exists
```

### Creating the Binary

We need an ARM toolchain installed on our system.

```text
make build
```

### Disassembly

We can now view the disassembly of the _elf_ file:

```text
make disassemble
```

So far, we only operated with the _elf_ file. This format does not only contain the binary code but also some headers and stuff. This is useful, when another software like an operating system or bootloader launches the file. However, to run the program bare-metal we don’t need an _elf_ file, but rather a _bin_ file. This is an image of the software which can be written byte-by-byte into the memory of the microcontroller.

Converting the _elf_ file to _bin_ file

```text
make compile
```

The `Kernel.bin` is now ready to flash.

### Flashing the Binary

You need to have stm32flash To flash the _bin_ file, we execute:

```text
make flash
```

**Success!**

### Summary of Commands to Build and Flash the Software:

```text
make all
make flash
```

**That’s it!** Now the LED on the board is blinking!

## CONCLUSION

This was surprisingly easy and fun!

## REFERENCES

* Embedded Rust Book: [https://github.com/rust-lang-nursery/embedded-wg/tree/master/books/embedded-rust-book](https://github.com/rust-lang-nursery/embedded-wg/tree/master/books/embedded-rust-book)
* Awesome Embedded Rust: [https://github.com/rust-embedded/awesome-embedded-rust](https://github.com/rust-embedded/awesome-embedded-rust)
* STM32F103C8 Website: [https://www.st.com/en/microcontrollers/stm32f103c8.html](https://www.st.com/en/microcontrollers/stm32f103c8.html)
* STM32F103C8 Datasheet: [https://www.st.com/resource/en/datasheet/stm32f103c8.pdf](https://www.st.com/resource/en/datasheet/stm32f103c8.pdf)
* STM32F103C8 Reference Manual: [https://www.st.com/content/ccc/resource/technical/document/reference\_manual/59/b9/ba/7f/11/af/43/d5/CD00171190.pdf/files/CD00171190.pdf/jcr:content/translations/en.CD00171190.pdf](https://www.st.com/content/ccc/resource/technical/document/reference_manual/59/b9/ba/7f/11/af/43/d5/CD00171190.pdf/files/CD00171190.pdf/jcr:content/translations/en.CD00171190.pdf)
* STM32F103C8 Flash Programming: [https://www.st.com/content/ccc/resource/technical/document/programming\_manual/10/98/e8/d4/2b/51/4b/f5/CD00283419.pdf/files/CD00283419.pdf/jcr:content/translations/en.CD00283419.pdf](https://www.st.com/content/ccc/resource/technical/document/programming_manual/10/98/e8/d4/2b/51/4b/f5/CD00283419.pdf/files/CD00283419.pdf/jcr:content/translations/en.CD00283419.pdf)
* STM32F103C8 ARM Cortex M3 Programming: [https://www.st.com/content/ccc/resource/technical/document/programming\_manual/5b/ca/8d/83/56/7f/40/08/CD00228163.pdf/files/CD00228163.pdf/jcr:content/translations/en.CD00228163.pdf](https://www.st.com/content/ccc/resource/technical/document/programming_manual/5b/ca/8d/83/56/7f/40/08/CD00228163.pdf/files/CD00228163.pdf/jcr:content/translations/en.CD00228163.pdf)

