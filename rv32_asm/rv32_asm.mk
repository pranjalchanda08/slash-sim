# rv32_asm.mk
ASM_DIR := rv32_asm
# Variables
ASM_SRCS := $(wildcard $(ASM_DIR)/*.s)

# Assembly build target: Only build when ASM variable is set
ifeq ($(ASM),)
  # If ASM is not set, do not do any assembly build
else
asm: $(ASM_SRCS)
	@mkdir -p $(BUILD_DIR)
	riscv64-unknown-elf-gcc $(MARCH_FLAG) -mabi=ilp32 -static -nostdlib -T$(ASM_DIR)/$(LD_FILE) $(ASM_DIR)/$(ASM).s -o $(BUILD_DIR)/$(ASM).elf
	riscv64-unknown-elf-objcopy -O binary $(BUILD_DIR)/$(ASM).elf $(BUILD_DIR)/$(ASM).bin
endif