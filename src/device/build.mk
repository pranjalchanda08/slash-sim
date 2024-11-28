# Separate source files by type
C_SRCS := $(wildcard src/device/*.c)
CPP_SRCS := $(wildcard src/device/*.cpp)
S_SRCS := $(wildcard src/device/*.s)

# Combine them into a single SRCS variable if needed
SRCS := $(C_SRCS) $(CPP_SRCS) $(S_SRCS)

# Object files based on source type
C_OBJS := $(C_SRCS:src/device/%.c=$(BUILD_DIR)/device/%.o)
CPP_OBJS := $(CPP_SRCS:src/device/%.cpp=$(BUILD_DIR)/device/%.o)
S_OBJS := $(S_SRCS:src/device/%.s=$(BUILD_DIR)/device/%.o)

# Include flags
INC_FLAGS := $(addprefix -I, $(INC_DIRS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP
CFLAGS := $(CPPFLAGS)
CXXFLAGS ?= g++

# Compile rules for C files
$(BUILD_DIR)/device/%.o: src/device/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile rules for C++ files
$(BUILD_DIR)/device/%.o: src/device/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile rules for Assembly files
$(BUILD_DIR)/device/%.o: src/device/%.s
	@mkdir -p $(dir $@)
	$(AS) -c $< -o $@

# Dependency files
-include $(C_OBJS:.o=.d) $(CPP_OBJS:.o=.d) $(S_OBJS:.o=.d)

# Export object files for linking
OBJ_FILES := $(OBJ_FILES) $(C_OBJS) $(CPP_OBJS) $(S_OBJS)
export OBJ_FILES
