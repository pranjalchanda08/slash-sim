

# Separate source files by type
C_SRCS := $(wildcard src/opcode/*.c)
CPP_SRCS := $(wildcard src/opcode/*.cpp)
S_SRCS := $(wildcard src/opcode/*.s)

# Combine them into a single SRCS variable if needed
SRCS := $(C_SRCS) $(CPP_SRCS) $(S_SRCS)

# Object files based on source type
C_OBJS := $(C_SRCS:src/opcode/%.c=$(BUILD_DIR)/opcode/%.o)
CPP_OBJS := $(CPP_SRCS:src/opcode/%.cpp=$(BUILD_DIR)/opcode/%.o)
S_OBJS := $(S_SRCS:src/opcode/%.s=$(BUILD_DIR)/opcode/%.o)

# Include flags
INC_FLAGS := $(addprefix -I, $(INC_DIRS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP
CFLAGS := $(CPPFLAGS)
CXXFLAGS := g++

# Compile rules for C files
$(BUILD_DIR)/opcode/%.o: src/opcode/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile rules for C++ files
$(BUILD_DIR)/opcode/%.o: src/opcode/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile rules for Assembly files
$(BUILD_DIR)/opcode/%.o: src/opcode/%.s
	@mkdir -p $(dir $@)
	$(AS) -c $< -o $@

# Dependency files
-include $(C_OBJS:.o=.d) $(CPP_OBJS:.o=.d) $(S_OBJS:.o=.d)

# Export object files for linking
OBJ_FILES := $(OBJ_FILES) $(C_OBJS) $(CPP_OBJS) $(S_OBJS)
export OBJ_FILES
