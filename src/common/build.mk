# Source files
C_SRCS := $(wildcard src/common/*.c)
CPP_SRCS := $(wildcard src/common/*.cpp)
S_SRCS := $(wildcard src/common/*.s)

# Object files
C_OBJS := $(C_SRCS:src/common/%.c=$(BUILD_DIR)/common/%.o)
CPP_OBJS := $(CPP_SRCS:src/common/%.cpp=$(BUILD_DIR)/common/%.o)
S_OBJS := $(S_SRCS:src/common/%.s=$(BUILD_DIR)/common/%.o)

# Combine object files
OBJ_FILES += $(C_OBJS) $(CPP_OBJS) $(S_OBJS)
export OBJ_FILES