OBJ_DIR := target
ISO := target/risx.iso
KERNEL := target/risx.elf
QEMU64 := qemu-system-x86_64
QEMUFLAGS := -machine pc -smp cores=2 -m 4G
QEMUDEBUGFLAGS := -d int -s -S -monitor stdio
ISO_ROOT := $(OBJ_DIR)/iso_root

all: tools sys

tools:
	$(MAKE) -C tools OBJ_DIR=$(CURDIR)/$(OBJ_DIR)

sys:
	$(MAKE) -C sys OBJ_DIR=$(CURDIR)/$(OBJ_DIR)

iso: sys
	@mkdir -p $(ISO_ROOT)
	@cp -r ./boot $(ISO_ROOT)
	@cp $(KERNEL) $(ISO_ROOT)
	@xorriso -as mkisofs				\
		-b boot/limine/limine-bios-cd.bin	\
		-no-emul-boot				\
		-boot-load-size 4			\
		-boot-info-table			\
		$(ISO_ROOT) -o $(ISO)
	@limine bios-install $(ISO)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(ISO_ROOT)
	$(MAKE) -C sys clean OBJ_DIR=$(CURDIR)/$(OBJ_DIR)

qemu: iso
	$(QEMU64) $(QEMUFLAGS) -cdrom $(ISO)

qemu-debug: iso
	$(QEMU64) $(QEMUFLAGS) $(QEMUDEBUGFLAGS) -cdrom $(ISO)

.PHONY: all tools sys iso clean qemu qemu-debug
