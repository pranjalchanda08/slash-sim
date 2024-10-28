CC = gcc
CXX = g++

TARGET_EXEC := slash_sim

BUILD_DIR := build
OUT_DIR := out
SRC_DIRS := src

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := inc inc/opcode

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP

ASM ?=
ARGS ?=
LD_FILE ?= rv32.ld

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -g -c $< -o $@

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -g -c $< -o $@


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
	rm -r $(OUT_DIR)

run:
	mkdir -p $(OUT_DIR)/$(ASM)
	./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

debug:
	mkdir -p $(OUT_DIR)/$(ASM)
	gdb --args ./$(BUILD_DIR)/$(TARGET_EXEC) $(ARGS) $(ASM)

ASMDIR ?= rv32_asm
asm:
	mkdir -p ./$(BUILD_DIR)
	riscv64-unknown-elf-gcc -march=rv32i_zicsr -mabi=ilp32 -static -nostdlib -T$(ASMDIR)/$(LD_FILE) $(ASMDIR)/$(ASM).s -o $(BUILD_DIR)/${ASM}.elf
	riscv64-unknown-elf-objcopy -O binary $(BUILD_DIR)/$(ASM).elf $(BUILD_DIR)/$(ASM).bin

-include $(DEPS)
