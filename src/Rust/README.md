Rust is a fairly new language that has gotten to be very popular in recent years. And as the language matures, it has started to support a wider set of features, including compilation and linking for bare-metal targets. There is an excellent “Embedded Rust” ebook being written which covers the concepts that I’ll talk about here, but it’s still in-progress and there aren’t many turn-key code examples after the first couple of chapters.

The Rust language is less than 10 years old and still evolving, so some features which might change in the future are only available on the nightly branch at the time of writing; this post is written for rustc version 1.36. And the language’s documentation is very good, but it can also be a little bit scattered in these early days. For example, after I had written most of this post I found a more comprehensive “Discovery ebook” which covers hardware examples for an STM32F3 “Discovery Kit” board. That looks like a terrific resource if you want to learn how to use the bare-metal Rust libraries from someone who actually knows what they’re talking about.

As a new Rustacean, I’ll admit that the syntax feels little bit frustrating at times. But that’s normal when you learn a new language, and Rust is definitely growing on me as I learn more about its aspirations for embedded development. Cargo looks promising for distributing things like register definitions, HALs, and BSPs. And there’s an automated svd2rust utility for generating your own register access libraries from vendor-supplied SVD files, which is useful in a language that hasn’t had time to build up an extensive set of well-proven libraries. So in this post I’ll talk about how to generate a “Peripheral Access Crate” for a simple STM32L031K6 chip, and how to use that crate to blink an LED.


It’s kind of fun when languages have mascots, especially when they’re CC0-licensed.

The target hardware will be an STM32L031K6 Nucleo-32 board, but this should work with any STM32L0x1 chip. I also tried the same process with an STM32F042 board and the STM32F0x2 SVD file, which worked fine. It’s amazing how easy it is to get started with a new chip compared to C, although you do still need to read the reference manuals to figure out which registers and bits you need to modify. This post will assume that you know a little bit about how to use Rust, but only the very basics – I’m still new to the language and I don’t think I would do a good job of explaining anything. The free Rust ebook is an excellent introduction, if you need a quick introduction.


Step 1: Toolchain Setup
Like with the bare-metal C examples, our first step is to set up a toolchain. The Rust maintainers have made installing and updating the toolchain fairly painless, so I’ll just point you to the “Installation” page of the Rust ebook.

Are you back? Great – the next step is to install the core “embedded Rust” dependencies, and you should also switch to the nightly branch of Rust’s toolchain. Like I mentioned earlier, some useful features haven’t made it into the stable branch quite yet. You’ll see an example in the next section, but for now just switch branches with this command:

rustup install nightly
rustup default nightly
You can switch back by replacing nightly with stable in the above commands. You can also use rustup update instead of rustup install to download and build a newer version of the toolchain for a given branch. Anyways, now that you’re on the nightly branch, follow both the “Tooling” and “Installation” instructions in the embedded Rust ebook.

And finally, you’ll need to install a few extra utilities for auto-generating and -formatting a “Peripheral Access Crate” from the SVD files that most microcontroller vendors distribute:

rustup component add rustfmt
cargo install svd2rust form
This would also be a good time to read through the first couple of chapters of the embedded Rust ebook. Chapter 2 is pretty concise, and it’ll help you get a feel for how the language intends to handle core embedded concepts like register definitions, bitfields, interrupts, HALs, and BSPs.

If you run into problems building the example cortex-m-quickstart template, it might be because Rust’s linker is not distributed on some platforms, like aarch64. Hopefully that won’t be an issue for much longer, but for now you can use GCC’s linker instead by un-commenting this line in .cargo/config:

# "-C", "linker=arm-none-eabi-ld",
You should be able to create projects for other architectures as well, but it looks like the svd2rust utility currently only supports SVD files describing an ARM Cortex-M, MSP430, or RISC-V core. Also, some chips such as the popular ESP32 do not currently have a free toolchain which supports LLVM. As far as I know, you cannot build Rust programs for those chips.

Step 2: Generate a Peripheral Access Crate
Now that you can write, flash, and debug a ‘Hello World’ program for a microcontroller (thanks to chapter 2 of the ’embedded Rust’ book), you’ll need a way to access the microcontroller’s peripheral registers before you can make it do anything interesting. The ‘Memory-mapped Registers‘ section of the book goes over the basic idea of how this works, but the examples are written for a TM4C123 chip which I do not have.

To create a new Peripheral Access Crate (PAC from now on) for the STM32L031 which I’ve used in previous tutorials, we only need an SVD file describing the microcontroller. You can download them for free from ST; on the chip’s product page, click the ‘Resources’ tab and download the “System View Description” .zip file under the “HW Model, CAD Libraries & SVD” section. Unzip it and find the STM32L0_svd_<version>/STM32L0x1.svd file.

Next, create a new ‘library crate’ using Cargo, copy the SVD file into it, and remove the default src/ directory:

cargo new stm32l0x1_pac --lib --name stm32l0x1
cd stm32l0x1_pac/
cp [...]/STM32L0x1.svd .
rm -rf src/
I should mention that there are already a couple of STM32L0x1 PACs available on crates.io, and I’m sure they work well, but this is still a useful learning exercise. The svd2rust documentation page has a good explanation of how to create a PAC from an SVD file, but before you actually generate the code, check the recommended dependencies and make sure that they are in your new crate’s Cargo.toml file. Your version numbers might look different in the future, but at the time of writing it looks something like:

[dependencies]
bare-metal = "0.2.4"
cortex-m = "0.5.8"
vcell = "0.1.0"

[dependencies.cortex-m-rt]
optional = true
version = "0.6.5"

[features]
rt = ["cortex-m-rt/device"]
In addition to utilities like svd2rust, the ‘embedded devices Working Group‘ has written packages like cortex-m and cortex-m-rt to provide ‘glue’ code like linker scripts and startup logic. Interrupt tables for each chip are actually included in the SVD files, so they will be included in your auto-generated PAC. Once you’ve added those dependencies, you can run svd2rust and format the large lib.rs output file using the commands recommended by the documentation:

svd2rust --nightly -i STM32L0x1.svd
form -i lib.rs -o src/
rm lib.rs
cargo fmt
And that’s it – your new library crate is located under src/, with a directory for each peripheral and a source file for each type of register. You can test that it builds with cargo build --target thumbv6m-none-eabi.

If you don’t use the --nightly option, svd2rust might not be able to generate mappings for registers which share the same memory address. That happens when a register’s functionality can change based on the situation, and the STM32L0x1 SVD file does have a few of those registers. This is one example of why I didn’t want to use the stable branch for this tutorial, but it also means that your crate might cause build issues if you aren’t on the nightly branch when you run cargo build. You can double-check your current branch with rustup default.

Step 3: Setup an Embedded Rust Application
Now that we have a library capable of reading and writing peripheral registers, we should be able to write a simple ‘blinking LED’ program. Let’s start by generating an empty Cortex-M project from the same cortex-m-quickstart template that the ebook used. I ran this command one directory above the stm32l0x1 crate so that both projects were in the same directory, but you can put it anywhere:

cargo generate --git https://github.com/rust-embedded/cortex-m-quickstart --name rust-blink
cd rust-blink
rm -r examples/
We’ll need to make a few changes to target a Cortex-M0+ core, because the default template targets a Cortex-M3. You can find the end result in this post’s GitHub repository, but I’ll also describe each change here.

First, look in the .cargo/config file. Un-comment the GNU linker line if you don’t have rust-lld installed, as discussed at the end of Step 1. Also un-comment the # runner = "gdb-multiarch -q -x openocd.gdb" line; that will make cargo run automatically open a debugging session in GDB. And at the end of the file, replace target = "thumbv7m-none-eabi" with target = "thumbv6m-none-eabi".

Next, add the following dependency to your Cargo.toml file:

[dependencies.stm32l0x1]
path = "../stm32l0x1_pac"
features = ["rt"]
version = "0.0.1"
The path value provides a local path to the PAC which you generated in Step 2; I put the test program in the same directory as the stm32l0x1 crate. And the "rt" feature overrides the default ARM Cortex-M interrupt table with the one defined in the stm32l0x1 crate. Our simple test program won’t use peripheral interrupts, so the "rt" feature doesn’t actually matter in this case, but that’s why it’s there. Next, delete these two lines from openocd.gdb:

break DefaultHandler
break HardFault
These chips only have two hardware breakpoints, and the default ‘run’ script sets four; GDB will give you warnings if you don’t get rid of the extras. You’ll also need to update openocd.cfg – replace target/stm32f3x.cfg with target/stm32l0.cfg since we’re using an STM32L0 chip instead of an STM32F3.

Finally, the last step before we get to the main program is to update the memory sections in memory.x:

MEMORY
{
  FLASH : ORIGIN = 0x08000000, LENGTH = 32K
  RAM   : ORIGIN = 0x20000000, LENGTH = 8K
}
Note that the default memory.x file puts the Flash ORIGIN label at 0x00000000, and we want 0x08000000. They look very similar. Before you move on, you can run cargo build to make sure that the application builds properly with an empty main method.

Step 4: Write a Blinking-LED Program
Finally, we can move on to writing code in src/main.rs. You can replace the default contents with this simple minimal program, so we have a common starting point:

#![no_std]
#![no_main]
// Halt when the program panics.
extern crate panic_halt;
// Includes.
use cortex_m_rt::entry;
#[entry]
fn main() -> ! {
  // Main loop.
  loop {
  }
}
That should look about the same as the template, but with fewer comments. The #![no_std] attribute is similar to the -nostdlib flag from C, and #![no_main] / #[entry] tell the program that it has a non-default entry point. The fn main() -> ! syntax indicates that main should not return; ! is used instead of specifying a return type.

We can get a timed delay for our blinking LED by setting up the SysTick timer using the same syntax from the embedded Rust ebook:

// ...
// Includes.
use cortex_m::peripheral::syst::SystClkSource;
use cortex_m_rt::entry;
#[entry]
fn main() -> ! {
  // Check out the 'Cortex-M Peripherals' singleton.
  let cm_p = cortex_m::Peripherals::take().unwrap();
  // Set up the SysTick peripheral.
  let mut syst = cm_p.SYST;
  syst.set_clock_source( SystClkSource::Core );
  // ~1s period; STM32L0 boots to a ~2.1MHz internal oscillator.
  syst.set_reload( 2_100_000 );
  syst.enable_counter();
  // ...
But unlike the SysTick timer, GPIO peripherals are not the same across all Cortex-M cores. To set those up, you’ll need to use the PAC that you generated in Step 2. You can refer to the ‘Embedded Rust’ ebook for the basic syntax, but since the ebook doesn’t cover this chip, it might be easier to just look at the auto-generated .rs files to see how the names of registers and bits and things are laid out. They should look familiar from the bare-metal C examples:

// ...
// Includes.
use cortex_m::peripheral::syst::SystClkSource;
use cortex_m_rt::entry;
use stm32l0x1;
#[entry]
fn main() -> ! {
  // ...
  // Set up GPIO pin B3 as push-pull output.
  let p = stm32l0x1::Peripherals::take().unwrap();
  let rcc = p.RCC;
  rcc.iopenr.write( |w| w.iopben().set_bit() );
  let gpiob = p.GPIOB;
  unsafe { gpiob.moder.write( |w| w.mode3().bits( 0b01 ) ); }
  gpiob.otyper.write( |w| w.ot3().clear_bit() );
  // Restart the SysTick counter.
  syst.clear_current();
  // Main loop.
  loop {
    // Toggle the LED every SysTick tick.
    while !syst.has_wrapped() {};
    gpiob.odr.write( |w| w.od3().set_bit() );
    while !syst.has_wrapped() {};
    gpiob.odr.write( |w| w.od3().clear_bit() );
  }
}
It’s interesting how the writes are performed with a closure-like syntax; you can also chain them together, like w.od3().clear_bit().od4().set_bit(). But you can see that writing multiple bits at once requires an unsafe block (highlighted above), because it performs a read/modify/write sequence and the register could change in between the ‘read’ and ‘write’ steps.

Usually those sorts of operations would happen inside of a HAL crate, or be abstracted into another method. The stm32f30x crate, for example, lets you call w.moder3().output() without using an unsafe block. The operation is still technically ‘unsafe’, but there’s not much risk because it is rare for any one peripheral to be configured from two different code sections without being gated behind something like a mutex or semaphore. Plus, I’m still not clear on how Rust’s concepts of ownership and borrowing apply to values like rcc and gpiob in the above example.

Anyways, the rest of the program is pretty simple. It resets the SysTick counter after the LED pin is initialized, then toggles the pin each time that the counter ticks over in an infinite loop.

Step 5: Build, Flash, Run
Once you’ve written the program, you can build it with cargo build. It’s sort of like running make, but instead of a Makefile you have Cargo.toml and an optional .cargo/ configuration directory. There’s more to it than that, but you can find more information in the Cargo Book.

The cortex-m-quickstart template does a good job of setting up a smooth workflow, so uploading and debugging your program should be easy. Run openocd from the project directory, followed by cargo run. That should run the commands listed in the openocd.gdb file, which will upload your program and set a breakpoint at main. Enter continue a couple of times, and the LED on your board should start blinking.

If you run into problems, double-check that you updated all of the files mentioned in Step 3. The GitHub repository also has finished examples of those configuration files.

Conclusions
This was surprisingly easy and fun! I’ve been skeptical about how tightly-coupled Cargo and Rust seem to be, but the built-in dependency management and the possibility of adding things like an embedded HAL API to the language are definitely appealing. Plus, it seems like a good way to manage modular boilerplate code that you want to use in a lot of different projects. But there was also definitely some tedium and frustration involved in figuring out the right configurations and syntax to make Cargo happy, and NPM has shown us in stark terms that centralized library repositories can easily go bad.

I’m also not sure how the performance works out – are all of those chained a().set_bit().b().clear_bit()... operations collected into one operation by the compiler? I have to assume they are, but I’m not completely sure and these are still early years for embedded Rust.

One thing that I really like about Rust and Cargo is that they encourage using MIT and Apache licenses, and the existence of an online repository makes it possible to distribute and manage libraries for embedded devices similar to the library managers used by Arduino / Keil / PlatformIO / etc. Having a package manager tightly-coupled to a language can bring a whole host of gnarly problems, but you can always configure Cargo to use local offline files, and I’ll welcome anything that might help to break the stranglehold of expensive and proprietary vendor lock-in that suffocates most microcontrollers and shuts small-time developers out of the market.

Also, Rust has a habit of suffixing its documentation titles with ‘-nomicon’ (‘book of …’), and obviously I’m a fan of that. Here are a couple of other documents which I haven’t had a chance to read, but which are probably very relevant to this post:

The Discovery Book walks through some examples using the ~$15 STM32F3 Discovery Kit as target hardware. It looks like a much better resource than this post, but I didn’t know about it until I had already written everything up. It looks like it probably would have saved me a lot of time, but that’s the learning process for you.
The Embedonomicon looks like it covers some of the concepts presented in the ‘Embedded Rust’ ebook in more detail.




A Freestanding Rust Binary
Feb 10, 2018
The first step in creating our own operating system kernel is to create a Rust executable that does not link the standard library. This makes it possible to run Rust code on the bare metal without an underlying operating system.

This blog is openly developed on GitHub. If you have any problems or questions, please open an issue there. You can also leave comments at the bottom. The complete source code for this post can be found in the post-01 branch.

🔗Introduction
To write an operating system kernel, we need code that does not depend on any operating system features. This means that we can't use threads, files, heap memory, the network, random numbers, standard output, or any other features requiring OS abstractions or specific hardware. Which makes sense, since we're trying to write our own OS and our own drivers.

This means that we can't use most of the Rust standard library, but there are a lot of Rust features that we can use. For example, we can use iterators, closures, pattern matching, option and result, string formatting, and of course the ownership system. These features make it possible to write a kernel in a very expressive, high level way without worrying about undefined behavior or memory safety.

In order to create an OS kernel in Rust, we need to create an executable that can be run without an underlying operating system. Such an executable is often called a “freestanding” or “bare-metal” executable.

This post describes the necessary steps to create a freestanding Rust binary and explains why the steps are needed. If you're just interested in a minimal example, you can jump to the summary.

🔗Disabling the Standard Library
By default, all Rust crates link the standard library, which depends on the operating system for features such as threads, files, or networking. It also depends on the C standard library libc, which closely interacts with OS services. Since our plan is to write an operating system, we can not use any OS-dependent libraries. So we have to disable the automatic inclusion of the standard library through the no_std attribute.

We start by creating a new cargo application project. The easiest way to do this is through the command line:

cargo new blog_os --bin --edition 2018
I named the project blog_os, but of course you can choose your own name. The --bin flag specifies that we want to create an executable binary (in contrast to a library) and the --edition 2018 flag specifies that we want to use the 2018 edition of Rust for our crate. When we run the command, cargo creates the following directory structure for us:

blog_os
├── Cargo.toml
└── src
    └── main.rs
The Cargo.toml contains the crate configuration, for example the crate name, the author, the semantic version number, and dependencies. The src/main.rs file contains the root module of our crate and our main function. You can compile your crate through cargo build and then run the compiled blog_os binary in the target/debug subfolder.

🔗The no_std Attribute
Right now our crate implicitly links the standard library. Let's try to disable this by adding the no_std attribute:

// main.rs

#![no_std]

fn main() {
    println!("Hello, world!");
}
When we try to build it now (by running cargo build), the following error occurs:

error: cannot find macro `println!` in this scope
 --> src/main.rs:4:5
  |
4 |     println!("Hello, world!");
  |     ^^^^^^^
The reason for this error is that the println macro is part of the standard library, which we no longer include. So we can no longer print things. This makes sense, since println writes to standard output, which is a special file descriptor provided by the operating system.

So let's remove the printing and try again with an empty main function:

// main.rs

#![no_std]

fn main() {}
> cargo build
error: `#[panic_handler]` function required, but not found
error: language item required, but not found: `eh_personality`
Now the compiler is missing a #[panic_handler] function and a language item.

🔗Panic Implementation
The panic_handler attribute defines the function that the compiler should invoke when a panic occurs. The standard library provides its own panic handler function, but in a no_std environment we need to define it ourselves:

// in main.rs

use core::panic::PanicInfo;

/// This function is called on panic.
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
The PanicInfo parameter contains the file and line where the panic happened and the optional panic message. The function should never return, so it is marked as a diverging function by returning the “never” type !. There is not much we can do in this function for now, so we just loop indefinitely.

🔗The eh_personality Language Item
Language items are special functions and types that are required internally by the compiler. For example, the Copy trait is a language item that tells the compiler which types have copy semantics. When we look at the implementation, we see it has the special #[lang = "copy"] attribute that defines it as a language item.

While providing custom implementations of language items is possible, it should only be done as a last resort. The reason is that language items are highly unstable implementation details and not even type checked (so the compiler doesn't even check if a function has the right argument types). Fortunately, there is a more stable way to fix the above language item error.

The eh_personality language item marks a function that is used for implementing stack unwinding. By default, Rust uses unwinding to run the destructors of all live stack variables in case of a panic. This ensures that all used memory is freed and allows the parent thread to catch the panic and continue execution. Unwinding, however, is a complicated process and requires some OS specific libraries (e.g. libunwind on Linux or structured exception handling on Windows), so we don't want to use it for our operating system.

🔗Disabling Unwinding
There are other use cases as well for which unwinding is undesirable, so Rust provides an option to abort on panic instead. This disables the generation of unwinding symbol information and thus considerably reduces binary size. There are multiple places where we can disable unwinding. The easiest way is to add the following lines to our Cargo.toml:

[profile.dev]
panic = "abort"

[profile.release]
panic = "abort"
This sets the panic strategy to abort for both the dev profile (used for cargo build) and the release profile (used for cargo build --release). Now the eh_personality language item should no longer be required.

Now we fixed both of the above errors. However, if we try to compile it now, another error occurs:

> cargo build
error: requires `start` lang_item
Our program is missing the start language item, which defines the entry point.

🔗The start attribute
One might think that the main function is the first function called when you run a program. However, most languages have a runtime system, which is responsible for things such as garbage collection (e.g. in Java) or software threads (e.g. goroutines in Go). This runtime needs to be called before main, since it needs to initialize itself.

In a typical Rust binary that links the standard library, execution starts in a C runtime library called crt0 (“C runtime zero”), which sets up the environment for a C application. This includes creating a stack and placing the arguments in the right registers. The C runtime then invokes the entry point of the Rust runtime, which is marked by the start language item. Rust only has a very minimal runtime, which takes care of some small things such as setting up stack overflow guards or printing a backtrace on panic. The runtime then finally calls the main function.

Our freestanding executable does not have access to the Rust runtime and crt0, so we need to define our own entry point. Implementing the start language item wouldn't help, since it would still require crt0. Instead, we need to overwrite the crt0 entry point directly.

🔗Overwriting the Entry Point
To tell the Rust compiler that we don't want to use the normal entry point chain, we add the #![no_main] attribute.

#![no_std]
#![no_main]

use core::panic::PanicInfo;

/// This function is called on panic.
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
You might notice that we removed the main function. The reason is that a main doesn't make sense without an underlying runtime that calls it. Instead, we are now overwriting the operating system entry point with our own _start function:

#[no_mangle]
pub extern "C" fn _start() -> ! {
    loop {}
}
By using the #[no_mangle] attribute we disable the name mangling to ensure that the Rust compiler really outputs a function with the name _start. Without the attribute, the compiler would generate some cryptic _ZN3blog_os4_start7hb173fedf945531caE symbol to give every function an unique name. The attribute is required because we need to tell the name of the entry point function to the linker in the next step.

We also have to mark the function as extern "C" to tell the compiler that it should use the C calling convention for this function (instead of the unspecified Rust calling convention). The reason for naming the function _start is that this is the default entry point name for most systems.

The ! return type means that the function is diverging, i.e. not allowed to ever return. This is required because the entry point is not called by any function, but invoked directly by the operating system or bootloader. So instead of returning, the entry point should e.g. invoke the exit system call of the operating system. In our case, shutting down the machine could be a reasonable action, since there's nothing left to do if a freestanding binary returns. For now, we fulfill the requirement by looping endlessly.

When we run cargo build now, we get an ugly linker error.

🔗Linker Errors
The linker is a program that combines the generated code into an executable. Since the executable format differs between Linux, Windows, and macOS, each system has its own linker that throws a different error. The fundamental cause of the errors is the same: the default configuration of the linker assumes that our program depends on the C runtime, which it does not.

To solve the errors, we need to tell the linker that it should not include the C runtime. We can do this either by passing a certain set of arguments to the linker or by building for a bare metal target.

🔗Building for a Bare Metal Target
By default Rust tries to build an executable that is able to run in your current system environment. For example, if you're using Windows on x86_64, Rust tries to build a .exe Windows executable that uses x86_64 instructions. This environment is called your "host" system.

To describe different environments, Rust uses a string called target triple. You can see the target triple for your host system by running rustc --version --verbose:

rustc 1.35.0-nightly (474e7a648 2019-04-07)
binary: rustc
commit-hash: 474e7a6486758ea6fc761893b1a49cd9076fb0ab
commit-date: 2019-04-07
host: x86_64-unknown-linux-gnu
release: 1.35.0-nightly
LLVM version: 8.0
The above output is from a x86_64 Linux system. We see that the host triple is x86_64-unknown-linux-gnu, which includes the CPU architecture (x86_64), the vendor (unknown), the operating system (linux), and the ABI (gnu).

By compiling for our host triple, the Rust compiler and the linker assume that there is an underlying operating system such as Linux or Windows that use the C runtime by default, which causes the linker errors. So to avoid the linker errors, we can compile for a different environment with no underlying operating system.

An example for such a bare metal environment is the thumbv7em-none-eabihf target triple, which describes an embedded ARM system. The details are not important, all that matters is that the target triple has no underlying operating system, which is indicated by the none in the target triple. To be able to compile for this target, we need to add it in rustup:

rustup target add thumbv7em-none-eabihf
This downloads a copy of the standard (and core) library for the system. Now we can build our freestanding executable for this target:

cargo build --target thumbv7em-none-eabihf
By passing a --target argument we cross compile our executable for a bare metal target system. Since the target system has no operating system, the linker does not try to link the C runtime and our build succeeds without any linker errors.

This is the approach that we will use for building our OS kernel. Instead of thumbv7em-none-eabihf, we will use a custom target that describes a x86_64 bare metal environment. The details will be explained in the next post.

🔗Linker Arguments
Instead of compiling for a bare metal system, it is also possible to resolve the linker errors by passing a certain set of arguments to the linker. This isn't the approach that we will use for our kernel, therefore this section is optional and only provided for completeness. Click on "Linker Arguments" below to show the optional content.

Linker Arguments
🔗Summary
A minimal freestanding Rust binary looks like this:

src/main.rs:

#![no_std] // don't link the Rust standard library
#![no_main] // disable all Rust-level entry points

use core::panic::PanicInfo;

#[no_mangle] // don't mangle the name of this function
pub extern "C" fn _start() -> ! {
    // this function is the entry point, since the linker looks for a function
    // named `_start` by default
    loop {}
}

/// This function is called on panic.
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
Cargo.toml:

[package]
name = "crate_name"
version = "0.1.0"
authors = ["Author Name <author@example.com>"]

# the profile used for `cargo build`
[profile.dev]
panic = "abort" # disable stack unwinding on panic

# the profile used for `cargo build --release`
[profile.release]
panic = "abort" # disable stack unwinding on panic
To build this binary, we need to compile for a bare metal target such as thumbv7em-none-eabihf:

cargo build --target thumbv7em-none-eabihf
Alternatively, we can compile it for the host system by passing additional linker arguments:

# Linux
cargo rustc -- -C link-arg=-nostartfiles
# Windows
cargo rustc -- -C link-args="/ENTRY:_start /SUBSYSTEM:console"
# macOS
cargo rustc -- -C link-args="-e __start -static -nostartfiles"
Note that this is just a minimal example of a freestanding Rust binary. This binary expects various things, for example that a stack is initialized when the _start function is called. So for any real use of such a binary, more steps are required.
