CROSS_COMPILE = riscv64-unknown-elf-

CC      = $(CROSS_COMPILE)gcc
AS      = $(CROSS_COMPILE)as
LD      = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

TARGET  = test


ARCH_FLAGS = -march=rv32im -mabi=ilp32
CFLAGS  = $(ARCH_FLAGS) -c -O0 -Wall -Wextra -ffreestanding
ASFLAGS = $(ARCH_FLAGS)
LDFLAGS = -m elf32lriscv -T linker.ld --no-relax

SRCS_C   = $(wildcard *.c)
SRCS_ASM = $(wildcard *.s)

OBJS     = $(SRCS_ASM:.s=.o) $(SRCS_C:.c=.o)



all: $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	@echo "Generating flat binary: $@"
	$(OBJCOPY) -O binary $< $@


$(TARGET).elf: $(OBJS) linker.ld
	@echo "Linking objects: $^ -> $@"
	$(LD) $(LDFLAGS) -o $@ $(OBJS)


%.o: %.c
	@echo "Compiling C: $<"
	$(CC) $(CFLAGS) -o $@ $<


%.o: %.s
	@echo "Assembling ASM: $<"
	$(AS) $(ASFLAGS) -o $@ $<


clean:
	@echo "Cleaning generated files..."
	rm -f *.o *.elf *.bin


info:
	@echo "Found C sources: $(SRCS_C)"
	@echo "Found ASM sources: $(SRCS_ASM)"
	@echo "Objects to generate: $(OBJS)"


.PHONY: all clean info
