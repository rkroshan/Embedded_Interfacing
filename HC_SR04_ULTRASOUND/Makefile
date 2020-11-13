CC= arm-none-eabi-gcc
MARCH= cortex-m4
SPECS= rdimon.specs
LFILE= stm32f4_ls.ld
CFLAGS= -c -mcpu=$(MARCH) -mthumb -mfloat-abi=soft -std=gnu11 -O0 -Wall -Werror
LDFLAGS= -mcpu=$(MARCH) -mthumb -mfloat-abi=soft --specs=$(SPECS) -T $(LFILE) -Wl,-Map=final.map
OUTPUT= final.elf

all: clean *.o $(OUTPUT) size_info launch_openocd

*.o: *.c
	$(CC) $(CFLAGS) $^

$(OUTPUT): *.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -rf *.o *.elf *.s *.i *.log 

size_info:
	arm-none-eabi-size $(OUTPUT)

launch_openocd:
	openocd -f stm32f4discovery.cfg