# Collect all build.mk files from subdirectories of src/
BUILD_MK_FILES := $(wildcard src/*/build.mk)

# Include each subdirectory's build.mk
include $(BUILD_MK_FILES)

# Aggregate all objects from subdirectories into SRC_OBJS
SRC_OBJS := $(foreach mk, $(BUILD_MK_FILES), $(OBJ_EXPORTS))

# Export SRC_OBJS to the top-level Makefile
export SRC_OBJS
include src/common/build.mk
include src/device/build.mk
include src/opcode/build.mk
include src/tools/build.mk
