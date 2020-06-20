CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
AOC = arm-none-eabi-objcopy

CFLAGS = -mthumb -mcpu=cortex-m3

KERNEL_BIN = kernel.bin
KERNEL_ELF = kernel.elf
MAIN_FILE = main.c

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