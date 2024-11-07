CC = gcc
CXX = g++
#Checks the compiler version and associate corresponding flag
# eg : for [version > 11] _zicsr flag is required
GCC_VERSION := $(shell riscv64-unknown-elf-gcc --version | head -n 1 | awk '{print $$3}')
GCC_MAJOR := $(word 1, $(subst ., ,$(GCC_VERSION)))
GCC_MINOR := $(word 2, $(subst ., ,$(GCC_VERSION)))

TARGET_EXEC := slash_sim

# Directory structure
BUILD_DIR := build
OUT_DIR := out
SRC_DIRS := src
ASMDIR ?= rv32_asm

# Locate all source files (C, C++, Assembly)
SRCS := $(shell find $(SRC_DIRS) -type f \( -name '*.cpp' -o -name '*.c' -o -name '*.s' \))

# Convert source paths to object paths (e.g., src/foo.c -> build/src/foo.c.o)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Include directories
INC_DIRS := $(wildcard inc/*)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Compilation flags
CPPFLAGS := $(INC_FLAGS) -MMD -MP

ifeq ($(shell test $(GCC_MAJOR) -ge 11 && echo 1), 1)
    MARCH_FLAG := -march=rv32i_zicsr
else
    MARCH_FLAG := -march=rv32i
endif

ASM ?=
ARGS ?=
LD_FILE ?= rv32.ld

# Default target
.PHONY: all
all: $(BUILD_DIR)/$(TARGET_EXEC)

# Link final executable
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Pattern rule for building C and C++ files
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Run target
.PHONY: run
run: all
	@mkdir -p $(OUT_DIR)/$(ASM)
	./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

# Debug target
.PHONY: debug
debug: all
	@mkdir -p $(OUT_DIR)/$(ASM)
	gdb --args ./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

# Assembly build target
.PHONY: asm
asm:
	mkdir -p ./$(BUILD_DIR)
	riscv64-unknown-elf-gcc $(MARCH_FLAG) -mabi=ilp32 -static -nostdlib -T$(ASMDIR)/$(LD_FILE) $(ASMDIR)/$(ASM).s -o $(BUILD_DIR)/${ASM}.elf
	riscv64-unknown-elf-objcopy -O binary $(BUILD_DIR)/$(ASM).elf $(BUILD_DIR)/$(ASM).bin

# Clean up build files
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(OUT_DIR)

# Include generated dependency files
-include $(DEPS)
