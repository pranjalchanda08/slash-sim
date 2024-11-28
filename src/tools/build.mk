TOOLS_SRCS := $(wildcard src/tools/*.c)
TOOLS_OBJS := $(TOOLS_SRCS:src/tools/%.c=build/src/tools/%.o)

$(TOOLS_OBJS): build/src/tools/%.o: src/tools/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Export TOOLS_OBJS so it can be aggregated by src.mk
#OBJ_EXPORTS += $(TOOLS_OBJS)
OBJ_EXPORTS := $(TOOLS_OBJS)