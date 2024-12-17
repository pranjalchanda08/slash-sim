

# Compiler definitions
CC = gcc
CXX = g++

# Function to normalize version strings
define normalize_version
$(shell echo $(1) | awk -F. '{ printf "%d%02d%02d", $$1, $$2, $$3 }')
endef

# Minimum required GCC version
GCC_MIN_VERSION := 11.0.0
GCC_VERSION := $(shell riscv64-unknown-elf-gcc --version | head -n 1 | sed 's/.* //')

# Normalize version numbers for comparison
MIN_VERSION_NUMERIC := $(call normalize_version,$(GCC_MIN_VERSION))
CURRENT_VERSION_NUMERIC := $(call normalize_version,$(GCC_VERSION))

# Target executable name
TARGET_EXEC := slash_sim

# Default variables
ASM ?=
SM ?=
ARGS ?=
LD_FILE ?= rv32.ld

# Directory structure
BUILD_DIR := build
OUT_DIR := out
SRC_DIRS := src
ASMDIR ?= rv32_asm

# Include directories
INC_DIRS := $(wildcard inc/*)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Compilation flags
CPPFLAGS := $(INC_FLAGS) -MMD -MP
CFLAGS := $(CPPFLAGS)
CXXFLAGS ?= g++

# Export common variables
export BUILD_DIR
export MARCH_FLAG
export LD_FILE
export ASMDIR
export ASM

# Version check
ifeq ($(shell [ $(CURRENT_VERSION_NUMERIC) -lt $(MIN_VERSION_NUMERIC) ] && echo 1 || echo 0), 1)
$(warning Bad toolchain version $(GCC_VERSION). Minimum required: $(GCC_MIN_VERSION))
endif


# Set MARCH_FLAG based on GCC version
ifeq ($(shell [ $(CURRENT_VERSION_NUMERIC) -lt $(MIN_VERSION_NUMERIC) ] && echo 1 || echo 0), 0)
MARCH_FLAG := -march=rv32i_zicsr
else
MARCH_FLAG := -march=rv32i
endif

# Include src.mk to collect object files
include $(SRC_DIRS)/src.mk

# Include build.mk files from subdirectories
include $(wildcard $(SRC_DIRS)/**/build.mk)
include $(ASMDIR)/rv32_asm.mk

# Default target
all: $(BUILD_DIR)/$(TARGET_EXEC)

# Link the final executable
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Compilation rules
define COMPILE_TEMPLATE
$(BUILD_DIR)/$(2)/%.o: $(3)/%.$(1)
	@mkdir -p $$(dir $$@)
	$(if $(filter c,$(1)),$(CC) $(CFLAGS),$(if $(filter cpp,$(1)),$(CXX) $(CXXFLAGS),$(AS))) -c $$< -o $$@
endef

# Generate compilation rules for all directories
$(eval $(call COMPILE_TEMPLATE,c,common,src/common))
$(eval $(call COMPILE_TEMPLATE,cpp,common,src/common))
$(eval $(call COMPILE_TEMPLATE,s,common,src/common))
$(eval $(call COMPILE_TEMPLATE,c,device,src/device))
$(eval $(call COMPILE_TEMPLATE,cpp,device,src/device))
$(eval $(call COMPILE_TEMPLATE,s,device,src/device))
$(eval $(call COMPILE_TEMPLATE,c,opcode,src/opcode))
$(eval $(call COMPILE_TEMPLATE,cpp,opcode,src/opcode))
$(eval $(call COMPILE_TEMPLATE,s,opcode,src/opcode))
$(eval $(call COMPILE_TEMPLATE,c,tools,src/tools))
$(eval $(call COMPILE_TEMPLATE,cpp,tools,src/tools))
$(eval $(call COMPILE_TEMPLATE,s,tools,src/tools))

# Run target
run: all
	@mkdir -p $(OUT_DIR)/$(ASM)
	./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

# Debug target
debug: all
	@mkdir -p $(OUT_DIR)/$(ASM)
	gdb --args ./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

# Clean up build files
clean:
	@rm -rf $(BUILD_DIR) $(OUT_DIR)

# Include generated dependency files
-include $(OBJ_FILES:.o=.d)

.PHONY: all run debug clean