# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			user-configurable variables			      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
PROFILE ?= DEBUG
VERSION ?= 0.0.7
OBJ_DIR ?= target
ARCH ?= x86_64

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			toolchain default configuration			      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
CC := $(ARCH)-elf-gcc
CPPFLAGS := -Iinclude -Ivendors
CFLAGS := 			\
	-DLIMINE_API_REVISION=4	\
	-fdata-sections		\
	-ffreestanding		\
	-ffunction-sections	\
	-fno-lto		\
	-fno-PIC		\
	-fno-stack-check	\
	-fno-stack-protector	\
	-mno-ms-bitfields	\
	-mno-red-zone		\
	-pedantic-errors	\
	-pipe			\
	-std=c17		\
	-Wall			\
	-Werror			\
	-Wextra
LDFLAGS := -nostdlib -lgcc -T src/kernel-$(ARCH).ld

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#				x86 options				      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
ifeq ($(ARCH), x86_64)
CFLAGS +=			\
	-m64			\
	-mabi=sysv		\
	-mcmodel=kernel		\
	-mno-3dnow		\
	-mno-80387		\
	-mno-mmx		\
	-mno-red-zone		\
	-mno-sse		\
	-mno-sse2

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#				arm options				      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
else ifeq ($(ARCH), aarch64)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#				risc-v options				      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
else ifeq ($(ARCH), riscv64)
endif

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			build profile configuration			      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
ifeq ($(PROFILE), DEBUG)
CFLAGS +=			\
	-g			\
	-O0
endif

ifeq ($(PROFILE), RELEASE)
CFLAGS +=
	-O2			\
	-DVERSION
endif
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			source and object files detection		      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
SRCS := $(shell find vendors -type f -name '*.S' -o -name '*.c') \
	$(shell find src     -type f -name '*.S' -o -name '*.c')
OBJS := $(patsubst src/%.S,$(OBJ_DIR)/%.S.o,$(filter src/%.S,$(SRCS))) \
	$(patsubst src/%.c,$(OBJ_DIR)/%.c.o,$(filter src/%.c,$(SRCS)))
OBJS += $(patsubst vendors/%.S,$(OBJ_DIR)/vendors/%.S.o,$(filter vendors/%.S,$(SRCS))) \
	$(patsubst vendors/%.c,$(OBJ_DIR)/vendors/%.c.o,$(filter vendors/%.c,$(SRCS)))

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#				recipes					      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
all: $(OBJ_DIR)/risx.elf


$(OBJ_DIR)/risx.elf: $(OBJS) src/kernel-$(ARCH).ld
	@mkdir -p $(dir $@)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.S.o: src/%.S
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.c.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/vendors/%.S.o: vendors/%.S
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/vendors/%.c.o: vendors/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

.PHONY: all
