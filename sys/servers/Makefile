# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			user-configurable variables			      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
ARCH ?= x86_64
ROOTDIR ?= .
TARGET ?= $(ROOTDIR)/target/$(ARCH)/build/servers
ISOROOTDIR ?= $(ROOTDIR)/target/$(ARCH)/iso_root

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			toolchain default configuration			      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
CC := $(ARCH)-elf-gcc
CPPFLAGS :=
CFLAGS :=			\
	-ffreestanding		\
	-fno-stack-protector	\
	-fno-PIC		\
	-mno-red-zone		\
	-pedantic-errors	\
	-pipe			\
	-std=c17		\
	-Wall			\
	-Werror			\
	-Wextra
LDFLAGS := -nostdlib -lgcc -T src/server-$(ARCH).ld

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#				x86 options				      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
ifeq ($(ARCH), x86_64)
CFLAGS +=			\
	-m64			\
	-mabi=sysv		\
	-mcmodel=small		\
	-mno-3dnow		\
	-mno-80387		\
	-mno-mmx		\
	-mno-sse		\
	-mno-sse2
endif

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			source and object files detection		      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
OBJS := \
	$(TARGET)/dummy.c.o \
	$(TARGET)/arch/$(ARCH)/crt0.S.o

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#				recipes					      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
all: $(ISOROOTDIR)/servers/dummy.elf


$(ISOROOTDIR)/servers/dummy.elf: $(OBJS) src/server-$(ARCH).ld
	@mkdir -p $(dir $@)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(TARGET)/%.c.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET)/arch/$(ARCH)/%.S.o: src/arch/$(ARCH)/%.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: all
