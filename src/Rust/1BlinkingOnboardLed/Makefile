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

run:
	qemu-system-arm --cpu cortex-m3 --machine lm3s6965evb --nographic --semihosting-config enable=on,target=native --kernel target/thumbv7m-none-eabi/debug/a01-blink-onboard-led

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
