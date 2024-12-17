# Collect all object files from subdirectories
OBJ_FILES :=
OBJ_FILES += $(shell find src/common -name '*.o')
OBJ_FILES += $(shell find src/device -name '*.o')
OBJ_FILES += $(shell find src/opcode -name '*.o')
OBJ_FILES += $(shell find src/tools -name '*.o')

# Export OBJ_FILES to the parent Makefile
export OBJ_FILES