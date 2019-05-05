# Makefile for blink demo

#TOOLS = arm-linux-gnu
TOOLS = arm-none-eabi

# Assembling with gcc makes it want crt0 at link time.
#AS = $(TOOLS)-gcc
AS = $(TOOLS)-as
# Use the -g flag if you intend to use gdb
#CC = $(TOOLS)-gcc -mcpu=cortex-m3 -mthumb
CC = $(TOOLS)-gcc

#LD = $(TOOLS)-gcc
LD = $(TOOLS)-ld.bfd
OBJCOPY = $(TOOLS)-objcopy
DUMP = $(TOOLS)-objdump -d
GDB = $(TOOLS)-gdb

CFLAGS = -DUSE_FULL_LL_DRIVER -mcpu=cortex-m3 -mthumb -g

C_INCLUDES =  \
-Iinc \
-IDrivers/STM32F1xx_HAL_Driver/inc \
-IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
-IDrivers/CMSIS/Include \
-IDrivers/CMSIS/Include

all: blink.bin lib

lib:
	$(MAKE) -C Drivers/

startup.o : startup_stm32f103xb.s
	$(AS) startup_stm32f103xb.s -o startup.o

system_stm32f1xx.o : system_stm32f1xx.c
	$(CC) $(CFLAGS) $(C_INCLUDES) -c system_stm32f1xx.c

mersenne.o : ./src/mersene.c
	$(CC) $(CFLAGS) $(C_INCLUDES) -c src/mersene.c

uart.o : ./src/uart.c
	$(CC) $(CFLAGS) $(C_INCLUDES) -c src/uart.c

interrupts.o : ./src/interrupts.c
	$(CC) $(CFLAGS) $(C_INCLUDES) -c src/interrupts.c

blink.o:	blink.c
	$(CC) $(CFLAGS)  $(C_INCLUDES) -c blink.c

blink.elf: 	startup.o blink.o system_stm32f1xx.o mersenne.o interrupts.o uart.o
	$(LD) -LDrivers/ -LDrivers/CMSIS/Lib/GCC/  -T STM32F103CBTx_FLASH.ld -o blink.elf startup.o blink.o uart.o system_stm32f1xx.o mersene.o interrupts.o -larm_cortexM3l_math -lstm32f103c8

blink.bin: blink.elf
	$(OBJCOPY) blink.elf blink.bin -O binary

flash :  blink.bin
	st-flash write blink.bin 0x08000000

clean:
	rm *.elf *.bin *.o build/*.o
