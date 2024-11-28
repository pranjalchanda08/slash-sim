CC = gcc
CXX = g++

# Function to normalize version strings into a numeric format for easier comparison
# Converts version like 11.0.0 to 110000
define normalize_version
  $(shell echo $(1) | awk -F. '{ printf "%d%02d%02d", $$1, $$2, $$3 }')  
endef

GCC_MIN_VERSION := 11.0.0
GCC_VERSION := $(shell riscv64-unknown-elf-gcc --version | head -n 1 | sed 's/.* //')  # For example: 9.3.0

MIN_VERSION_NUMERIC := $(call normalize_version, $(GCC_MIN_VERSION)) # Converts 11.0.0 -> 110000
CURRENT_VERSION_NUMERIC := $(call normalize_version, $(GCC_VERSION))  # Converts 9.3.0 -> 90300

 # Compare normalized versions and issue a warning if the current version is lower than the minimum required version
ifeq ($(shell [ $(CURRENT_VERSION_NUMERIC) -lt $(MIN_VERSION_NUMERIC) ] && echo 1 || echo 0), 1)
$(warning Bad toolchain version $(GCC_VERSION). Minimum required: $(GCC_MIN_VERSION))
endif

TARGET_EXEC := slash_sim

# Directory structure
BUILD_DIR := build
OUT_DIR := out
SRC_DIRS := src
ASMDIR ?= rv32_asm

# Locate all source files (C, C++, Assembly)
#SRCS := $(shell find $(SRC_DIRS) -type f \( -name '*.cpp' -o -name '*.c' -o -name '*.s' \))

# Convert source paths to object paths (e.g., src/foo.c -> build/src/foo.c.o)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Include directories
INC_DIRS := $(wildcard inc/*)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Compilation flags
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# Set MARCH_FLAG based on normalized GCC version comparison if gcc version > min version then use rv32i_zicsr else use rv32i
ifeq ($(shell [ $(CURRENT_VERSION_NUMERIC) -lt $(MIN_VERSION_NUMERIC) ] && echo 1 || echo 0), 0)
    MARCH_FLAG := -march=rv32i_zicsr
else
    MARCH_FLAG := -march=rv32i
endif

ASM ?=
ARGS ?=
LD_FILE ?= rv32.ld
include src/src.mk

# Use SRC_OBJS in the final linking step
$(BUILD_DIR)/$(TARGET_EXEC): $(SRC_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(SRC_OBJS) -o $@ $(LDFLAGS)

# Run target
run: $(BUILD_DIR)/$(TARGET_EXEC)
	@mkdir -p $(OUT_DIR)/$(ASM)
	./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

# Debug target
debug: $(BUILD_DIR)/$(TARGET_EXEC)
	@mkdir -p $(OUT_DIR)/$(ASM)
	gdb --args ./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

# Assembly build target
asm:
	@mkdir -p ./$(BUILD_DIR)
	riscv64-unknown-elf-gcc $(MARCH_FLAG) -mabi=ilp32 -static -nostdlib -T$(ASMDIR)/$(LD_FILE) $(ASMDIR)/$(ASM).s -o $(BUILD_DIR)/${ASM}.elf
	riscv64-unknown-elf-objcopy -O binary $(BUILD_DIR)/$(ASM).elf $(BUILD_DIR)/$(ASM).bin

# Clean up build files
clean:
	@rm -rf $(BUILD_DIR) $(OUT_DIR)

# Include generated dependencies
-include $(DEPS)

# Default target
# all: $(BUILD_DIR)/$(TARGET_EXEC)

# # Link final executable
# $(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
# 	@mkdir -p $(dir $@)
# 	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# # Pattern rule for building C and C++ files
# $(BUILD_DIR)/%.c.o: %.c
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# $(BUILD_DIR)/%.cpp.o: %.cpp
# 	@mkdir -p $(dir $@)
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# # Run target
# run: all
# 	@mkdir -p $(OUT_DIR)/$(ASM)
# 	./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

# # Debug target
# debug: all
# 	@mkdir -p $(OUT_DIR)/$(ASM)
# 	gdb --args ./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

# # Assembly build target
# asm:
# 	@mkdir -p ./$(BUILD_DIR)
# 	riscv64-unknown-elf-gcc $(MARCH_FLAG) -mabi=ilp32 -static -nostdlib -T$(ASMDIR)/$(LD_FILE) $(ASMDIR)/$(ASM).s -o $(BUILD_DIR)/${ASM}.elf
# 	riscv64-unknown-elf-objcopy -O binary $(BUILD_DIR)/$(ASM).elf $(BUILD_DIR)/$(ASM).bin

# # Clean up build files
# clean:
# 	@rm -rf $(BUILD_DIR) $(OUT_DIR)

# # Include generated dependency files
# -include $(DEPS)
