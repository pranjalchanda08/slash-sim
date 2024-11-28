# Compiler definitions
CC = gcc
CXX = g++

# Function to normalize version strings into a numeric format for easier comparison
# Converts version like 11.0.0 to 110000
define normalize_version
  $(shell echo $(1) | awk -F. '{ printf "%d%02d%02d", $$1, $$2, $$3 }')
endef

# Minimum required GCC version
GCC_MIN_VERSION := 11.0.0
GCC_VERSION := $(shell riscv64-unknown-elf-gcc --version | head -n 1 | sed 's/.* //')  # For example: 9.3.0

# Normalize GCC version numbers for comparison
MIN_VERSION_NUMERIC := $(call normalize_version, $(GCC_MIN_VERSION)) # Converts 11.0.0 -> 110000
CURRENT_VERSION_NUMERIC := $(call normalize_version, $(GCC_VERSION))  # Converts 9.3.0 -> 90300

# Issue a warning if the GCC version is lower than the minimum required version
ifeq ($(shell [ $(CURRENT_VERSION_NUMERIC) -lt $(MIN_VERSION_NUMERIC) ] && echo 1 || echo 0), 1)
$(warning Bad toolchain version $(GCC_VERSION). Minimum required: $(GCC_MIN_VERSION))
endif

# Set target executable name
TARGET_EXEC := slash_sim

# Directory structure
BUILD_DIR := build
OUT_DIR := out
SRC_DIRS := src
ASMDIR ?= rv32_asm
INC_DIRS := $(wildcard inc/*)

# Include flags
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP
CFLAGS := $(CPPFLAGS)
CXXFLAGS ?= g++

# Set MARCH_FLAG based on GCC version comparison
ifeq ($(shell [ $(CURRENT_VERSION_NUMERIC) -lt $(MIN_VERSION_NUMERIC) ] && echo 1 || echo 0), 0)
    MARCH_FLAG := -march=rv32i_zicsr
else
    MARCH_FLAG := -march=rv32i
endif

OBJ_FILES :=
# Include subdirectory build.mk files
include $(wildcard src/**/build.mk)

# Define object files
OBJS := $(OBJ_FILES)
DEPS := $(OBJS_FILES:.o=.d)

SM ?=
ARGS ?=
LD_FILE ?= rv32.ld
# Default target
all: $(BUILD_DIR)/$(TARGET_EXEC)

# Link the final executable
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compile C files
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ files
$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile Assembly files
$(BUILD_DIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# Run target (optional: for execution after build)
run: all
	@mkdir -p $(OUT_DIR)/$(ASM)
	./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

# Debug target (optional: to run with gdb)
debug: all
	@mkdir -p $(OUT_DIR)/$(ASM)
	gdb --args ./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

# Assembly build target
asm:
	@mkdir -p ./$(BUILD_DIR)
	riscv64-unknown-elf-gcc $(MARCH_FLAG) -mabi=ilp32 -static -nostdlib -T$(ASMDIR)/$(LD_FILE) $(ASMDIR)/$(ASM).s -o $(BUILD_DIR)/$(ASM).elf
	riscv64-unknown-elf-objcopy -O binary $(BUILD_DIR)/$(ASM).elf $(BUILD_DIR)/$(ASM).bin

# Clean up build files
clean:
	@rm -rf $(BUILD_DIR) $(OUT_DIR)

# Include generated dependency files
-include $(DEPS)
