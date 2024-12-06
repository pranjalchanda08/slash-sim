# Source files
C_SRCS := $(wildcard src/device/*.c)
CPP_SRCS := $(wildcard src/device/*.cpp)
S_SRCS := $(wildcard src/device/*.s)

# Object files
C_OBJS := $(C_SRCS:src/device/%.c=$(BUILD_DIR)/device/%.o)
CPP_OBJS := $(CPP_SRCS:src/device/%.cpp=$(BUILD_DIR)/device/%.o)
S_OBJS := $(S_SRCS:src/device/%.s=$(BUILD_DIR)/device/%.o)

# Combine object files
OBJ_FILES += $(C_OBJS) $(CPP_OBJS) $(S_OBJS)
export OBJ_FILES