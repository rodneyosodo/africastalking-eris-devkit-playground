//! Blinks an LED
//! The onboard LED is connected to PC13 as is the case on the blue pill board.

#![deny(unsafe_code)]
#![no_std]
#![no_main]

use panic_halt as _;
// use nb::block;
use cortex_m_rt::entry;
use embedded_hal::digital::v2::OutputPin;
use stm32f1xx_hal::{pac, prelude::*, delay::Delay};
// use cortex_m::asm;
// use stm32f1xx_hal::serial::{Config, Serial};

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
    // `clocks`
    let clocks = rcc.cfgr.freeze(&mut flash.acr);

    // Acquire the GPIOA peripheral
    let mut gpioa = dp.GPIOA.split(&mut rcc.apb2);

    // Configure gpio A pin 7 as a push-pull output. The `crh` register is passed to the function
    // in order to configure the port. For pins 0-7, crl should be passed instead.
    let mut led = gpioa.pa6.into_push_pull_output(&mut gpioa.crl);
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
