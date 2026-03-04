# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			user-configurable variables			      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
ARCH ?= x86_64
TARGET := $(CURDIR)/target
QEMU64 := qemu-system-$(ARCH)

ifeq ($(ARCH), x86_64)
QEMUFLAGS := -machine pc -smp cores=6 -m 4G
QEMUDEBUGFLAGS := -d int -s -S -monitor stdio
endif

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#				recipes					      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
all: sys


tools:
	$(MAKE) -C tools ROOTDIR=$(CURDIR)

sys:
	$(MAKE) -C sys ROOTDIR=$(CURDIR) ARCH=$(ARCH)

usr:
	$(MAKE) -C usr ROOTDIR=$(CURDIR) ARCH=$(ARCH)

ISOROOT ?= $(TARGET)/$(ARCH)/iso_root
ISOFILE ?= $(TARGET)/$(ARCH)/risx.iso
iso: sys
	@cp -r ./boot $(TARGET)/$(ARCH)/iso_root
	@xorriso					\
		-as mkisofs				\
		-b boot/limine/limine-bios-cd.bin	\
		-boot-info-table			\
		-boot-load-size 4			\
		-no-emul-boot				\
		-o $(ISOFILE)				\
		$(ISOROOT)
	@limine bios-install $(ISOFILE)

clean:
	@rm -rf $(TARGET)

qemu: iso
	$(QEMU64) $(QEMUFLAGS) -cdrom $(ISOFILE)

qemu-serial: iso
	$(QEMU64) $(QEMUFLAGS) -cdrom $(ISOFILE) -nographic

qemu-debug: iso
	$(QEMU64) $(QEMUFLAGS) $(QEMUDEBUGFLAGS) -cdrom $(ISOFILE)

.PHONY: all tools sys iso clean qemu qemu-debug
