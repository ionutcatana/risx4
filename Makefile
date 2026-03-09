# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			user-configurable variables			      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
ARCH ?= x86_64
TARGET := $(CURDIR)/target

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
	@mkdir -p $(TARGET)/$(ARCH)/iso_root
	@cp -r ./boot/* $(TARGET)/$(ARCH)/iso_root
	@xorriso				\
		-as mkisofs 			\
		-e limine/limine-uefi-cd.bin	\
		-isohybrid-gpt-basdat		\
		-no-emul-boot			\
		-o $(ISOFILE)			\
		$(ISOROOT)

clean:
	@rm -rf $(TARGET)

QEMU64 := qemu-system-$(ARCH)
ifeq ($(ARCH), x86_64)
QEMUFLAGS := -machine q35 -bios /usr/share/ovmf/OVMF.fd -smp cores=6 -m 4G
QEMUDEBUGFLAGS := -d int -s -S -monitor stdio
endif
qemu: iso
	$(QEMU64) $(QEMUFLAGS) -cdrom $(ISOFILE)

qemu-serial: iso
	$(QEMU64) $(QEMUFLAGS) -cdrom $(ISOFILE) -nographic

qemu-debug: iso
	$(QEMU64) $(QEMUFLAGS) $(QEMUDEBUGFLAGS) -cdrom $(ISOFILE)

.PHONY: all tools sys iso clean qemu qemu-debug
