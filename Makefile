BUILD_DIR = build

CFLAGS  ?= -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
           -Wformat-truncation -fno-common -Wconversion \
           -g3 -Os -ffunction-sections -fdata-sections -Idrivers \
           -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)

LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$@.map

SOURCES = startup/startup.c src/main.c drivers/gpio.c

all: firmware.elf

$(BUILD_DIR):
	mkdir -p $@

firmware.elf: $(SOURCES) | $(BUILD_DIR)
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/$@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $(BUILD_DIR)/$< $(BUILD_DIR)/$@

flash: firmware.bin
	STM32_Programmer_CLI -c port=SWD -d $(BUILD_DIR)/$< 0x08000000 -rst

clean:
	rm $(BUILD_DIR)/firmware.*

.PHONY: build clean flash
