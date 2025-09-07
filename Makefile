AR := arm-none-eabi-ar
CC := arm-none-eabi-gcc

COMMON_WARN := -W -Wall -Wextra -Werror
COMMON_OPT  := -O2 -ffunction-sections -fdata-sections -fshort-enums
COMMON_CPU  := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

CFLAGS := $(COMMON_WARN) $(COMMON_OPT) $(COMMON_CPU) -std=c11 -MMD -MP \
          -Iinclude

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)
LIB  := libf446re.a

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(LIB)

.PHONY: all clean
-include $(DEPS)
