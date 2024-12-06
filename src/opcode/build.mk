# Source files
C_SRCS := $(wildcard src/opcode/*.c)
CPP_SRCS := $(wildcard src/opcode/*.cpp)
S_SRCS := $(wildcard src/opcode/*.s)

# Object files
C_OBJS := $(C_SRCS:src/opcode/%.c=$(BUILD_DIR)/opcode/%.o)
CPP_OBJS := $(CPP_SRCS:src/opcode/%.cpp=$(BUILD_DIR)/opcode/%.o)
S_OBJS := $(S_SRCS:src/opcode/%.s=$(BUILD_DIR)/opcode/%.o)

# Combine object files
OBJ_FILES += $(C_OBJS) $(CPP_OBJS) $(S_OBJS)
export OBJ_FILES