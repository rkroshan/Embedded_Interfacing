CC= arm-none-eabi-gcc
MARCH= cortex-m4

SPECS= nano.specs
LFILE= stm32f4_ls.ld
CFLAGS= -c -mcpu=$(MARCH) -mthumb -mfloat-abi=soft -std=gnu11 -O0 -Wall -Werror 
LDFLAGS= -mcpu=$(MARCH) -mthumb -mfloat-abi=soft --specs=$(SPECS) -T $(LFILE) -Wl,-Map=final.map
OUTPUT= final.elf

SPECS_ITM= rdimon.specs
CFLAGS_ITM= -c -mcpu=$(MARCH) -mthumb -mfloat-abi=soft -std=gnu11 -O0 -Wall -Werror -DSEMIHOSTING=1
LDFLAGS_ITM= -mcpu=$(MARCH) -mthumb -mfloat-abi=soft --specs=$(SPECS_ITM) -T $(LFILE) -Wl,-Map=final.map
OUTPUT_ITM= final_itm.elf

$(OUTPUT): *.o
	$(CC) $(LDFLAGS) -o $@ $^

$(OUTPUT_ITM): *.o
	$(CC) $(LDFLAGS_ITM) -o $@ $^

clean:
	rm -rf *.o *.elf *.s *.i *.log 

size_info:
	arm-none-eabi-size $(OUTPUT)

size_info_itm:
	arm-none-eabi-size $(OUTPUT_ITM)

launch_openocd:
	openocd -f stm32f4discovery.cfg

build: 
	$(CC) $(CFLAGS) *.c ../syscalls.c

build_itm: 
	$(CC) $(CFLAGS_ITM) *.c  

run: clean build $(OUTPUT) size_info launch_openocd
all: clean build $(OUTPUT) size_info 

run_itm: clean build_itm $(OUTPUT_ITM) size_info_itm launch_openocd
all_itm: clean build_itm $(OUTPUT_ITM) size_info_itm 
