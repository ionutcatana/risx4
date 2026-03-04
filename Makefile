# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			user-configurable variables			      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
OBJ_DIR := target
QEMU64 := qemu-system-x86_64
QEMUFLAGS := -machine pc -smp cores=6 -m 4G
QEMUDEBUGFLAGS := -d int -s -S -monitor stdio
PERL := perl

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#				recipes					      #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
all: tools sys usr servers


tools:
	$(MAKE) -C tools OBJ_DIR=$(CURDIR)/$(OBJ_DIR)

sys:
	$(MAKE) -C sys OBJ_DIR=$(CURDIR)/$(OBJ_DIR)

usr:
	$(MAKE) -C usr OBJ_DIR=$(CURDIR)/$(OBJ_DIR)

servers:
	$(MAKE) -C servers OBJ_DIR=$(CURDIR)/$(OBJ_DIR)

iso: sys usr servers
	@mkdir -p $(OBJ_DIR)/iso_root
	@cp -r ./boot $(OBJ_DIR)/iso_root
	@cp target/risx.elf $(OBJ_DIR)/iso_root
	@cp target/dummy $(OBJ_DIR)/iso_root
	@xorriso -as mkisofs				\
		-b boot/limine/limine-bios-cd.bin	\
		-no-emul-boot				\
		-boot-load-size 4			\
		-boot-info-table			\
		$(OBJ_DIR)/iso_root -o target/risx.iso
	@limine bios-install target/risx.iso

clean:
	@rm -rf $(OBJ_DIR)

qemu: iso
	$(QEMU64) $(QEMUFLAGS) -cdrom target/risx.iso

qemu-serial: iso
	$(QEMU64) $(QEMUFLAGS) -cdrom target/risx.iso -nographic

qemu-debug: iso
	$(QEMU64) $(QEMUFLAGS) $(QEMUDEBUGFLAGS) -cdrom target/risx.iso

.PHONY: all tools servers sys iso clean qemu qemu-debug
