# src/common/build.mk

# Separate source files by type
C_SRCS := $(wildcard src/common/*.c)
CPP_SRCS := $(wildcard src/common/*.cpp)
S_SRCS := $(wildcard src/common/*.s)

# Combine them into a single SRCS variable if needed
SRCS := $(C_SRCS) $(CPP_SRCS) $(S_SRCS)

# Object files based on source type
C_OBJS := $(C_SRCS:src/common/%.c=$(OUT_DIR)/common/%.o)
CPP_OBJS := $(CPP_SRCS:src/common/%.cpp=$(OUT_DIR)/common/%.o)
S_OBJS := $(S_SRCS:src/common/%.s=$(OUT_DIR)/common/%.o)

# Include flags
INC_FLAGS := $(addprefix -I, $(INC_DIRS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP
CFLAGS := $(CPPFLAGS)
CXXFLAGS ?= g++

# Compile rules for C files
$(OUT_DIR)/common/%.o: src/common/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile rules for C++ files
$(OUT_DIR)/common/%.o: src/common/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile rules for Assembly files
$(OUT_DIR)/common/%.o: src/common/%.s
	@mkdir -p $(dir $@)
	$(AS) -c $< -o $@

# Dependency files
-include $(C_OBJS:.o=.d) $(CPP_OBJS:.o=.d) $(S_OBJS:.o=.d)

# Export object files for linking
OBJ_FILES := $(OBJ_FILES) $(C_OBJS) $(CPP_OBJS) $(S_OBJS)
export OBJ_FILES