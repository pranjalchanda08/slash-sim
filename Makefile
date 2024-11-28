# Compiler definitions
CC = gcc
CXX = g++

# Function to normalize version strings into a numeric format for easier comparison
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

# Include subdirectory build.mk files
include $(wildcard src/**/build.mk)
include $(ASMDIR)/rv32_asm.mk

OBJS := $(OBJ_FILES)
DEPS := $(OBJS_FILES:.o=.d)
ASM ?=
SM ?=
ARGS ?=
LD_FILE ?= rv32.ld

export BUILD_DIR
export MARCH_FLAG
export LD_FILE
export ASMDIR
export ASM

# Default target
all: $(BUILD_DIR)/$(TARGET_EXEC)

# Link the final executable
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Run target (optional: for execution after build)
run: all
	@mkdir -p $(OUT_DIR)/$(ASM)
	./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

# Debug target (optional: to run with gdb)
debug: all
	@mkdir -p $(OUT_DIR)/$(ASM)
	gdb --args ./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

# Clean up build files
clean:
	@rm -rf $(BUILD_DIR) $(OUT_DIR)
	

# Include generated dependency files
-include $(DEPS)
