OBJ_DIR ?= target
CC := gcc

$(OBJ_DIR)/gdt: gdt.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ $<
