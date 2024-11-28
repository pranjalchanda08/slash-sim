DEVICE_SRCS := $(wildcard src/device/*.c)
DEVICE_OBJS := $(DEVICE_SRCS:src/device/%.c=build/src/device/%.o)

$(DEVICE_OBJS): build/src/device/%.o: src/device/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

#SRC_OBJS += $(DEVICE_OBJS)
OBJ_EXPORTS := $(DEVICE_OBJS)
