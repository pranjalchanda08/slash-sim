COMMON_SRCS := $(wildcard src/common/*.c)
COMMON_OBJS := $(COMMON_SRCS:src/common/%.c=build/src/common/%.o)

$(COMMON_OBJS): build/src/common/%.o: src/common/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

#SRC_OBJS += $(COMMON_OBJS)
OBJ_EXPORTS := $(COMMON_OBJS)
