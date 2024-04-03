# Directories
SRC_DIR := src
BUILD_DIR := build
DIST_DIR := dist
ASM_DIR := $(SRC_DIR)/impl/x86_64
C_DIR := $(SRC_DIR)/impl/x86_64
KERNEL_C_DIR := $(SRC_DIR)/impl/kernel
ISO_DIR := targets/x86_64/iso

# Toolchain
AS := nasm
CC := x86_64-elf-gcc
LD := x86_64-elf-ld
GRUB_MKRESCUE := grub-mkrescue

# Flags
ASMFLAGS := -f elf64
CFLAGS := -I$(SRC_DIR)/intf -ffreestanding -MMD -MP
LDFLAGS := -n -T targets/x86_64/linker.ld

# Source files
ASM_SRC := $(shell find $(ASM_DIR) -name '*.asm')
CPP_SRC := $(shell find $(C_DIR) $(KERNEL_C_DIR) -name '*.cpp')

# Object files
OBJ := $(ASM_SRC:$(ASM_DIR)/%.asm=$(BUILD_DIR)/%.o) $(CPP_SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Dependency files
DEP := $(OBJ:.o=.d)

# Target
TARGET := $(DIST_DIR)/x86_64/kernel.bin
ISO := $(DIST_DIR)/x86_64/kernel.iso

# Rules
all: $(ISO)

$(ISO): $(TARGET)
	@mkdir -p $(ISO_DIR)/boot
	cp $(TARGET) $(ISO_DIR)/boot/kernel.bin
	$(GRUB_MKRESCUE) /usr/lib/grub/i386-pc -o $(ISO) $(ISO_DIR)

$(TARGET): $(OBJ)
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) -o $@ $^

# Assembly source
$(BUILD_DIR)/%.o: $(ASM_DIR)/%.asm
	@mkdir -p $(@D)
	$(AS) $(ASMFLAGS) $< -o $@

# C source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Include dependency files
-include $(DEP)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)