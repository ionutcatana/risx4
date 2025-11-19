OBJ_DIR ?= target
ARCH ?= x86_64

CC := $(ARCH)-elf-gcc
CPPFLAGS := -I/usr/local/include -Iinclude
CFLAGS := 			\
	-fdata-sections		\
	-ffreestanding		\
	-ffunction-sections	\
	-fno-lto		\
	-fno-PIC		\
	-fno-stack-check	\
	-fno-stack-protector	\
	-g			\
	-m64			\
	-mabi=sysv		\
	-mcmodel=kernel		\
	-mno-3dnow		\
	-mno-80387		\
	-mno-mmx		\
	-mno-red-zone		\
	-mno-sse		\
	-mno-sse2		\
	-O2			\
	-pipe			\
	-std=c17		\
	-Wall			\
	-Wextra
LDFLAGS := -T src/kernel_x86.ld -nostdlib -lgcc

SRCS := $(shell find src -type f -name '*.S' -o -name '*.c')
OBJS := $(patsubst src/%.S,$(OBJ_DIR)/%.S.o,$(filter %.S,$(SRCS))) \
	$(patsubst src/%.c,$(OBJ_DIR)/%.c.o,$(filter %.c,$(SRCS)))

all: $(OBJ_DIR)/risx.elf


$(OBJ_DIR)/risx.elf: $(OBJS) src/kernel_x86.ld
	@mkdir -p $(dir $@)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.S.o: src/%.S
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.c.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

.PHONY: all
