# Source files
C_SRCS := $(wildcard src/tools/*.c)
CPP_SRCS := $(wildcard src/tools/*.cpp)
S_SRCS := $(wildcard src/tools/*.s)

# Object files
C_OBJS := $(C_SRCS:src/tools/%.c=$(BUILD_DIR)/tools/%.o)
CPP_OBJS := $(CPP_SRCS:src/tools/%.cpp=$(BUILD_DIR)/tools/%.o)
S_OBJS := $(S_SRCS:src/tools/%.s=$(BUILD_DIR)/tools/%.o)

# Combine object files
OBJ_FILES += $(C_OBJS) $(CPP_OBJS) $(S_OBJS)
export OBJ_FILES