OPCODE_SRCS := $(wildcard src/opcode/*.c)
OPCODE_OBJS := $(OPCODE_SRCS:src/opcode/%.c=build/src/opcode/%.o)

$(OPCODE_OBJS): build/src/opcode/%.o: src/opcode/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

#SRC_OBJS += $(OPCODE_OBJS)
OBJ_EXPORTS := $(OPCODE_OBJS)
