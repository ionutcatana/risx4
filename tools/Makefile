OBJ_DIR ?= target
CC := gcc

all: $(OBJ_DIR)/gdt $(OBJ_DIR)/indices $(OBJ_DIR)/testelf


$(OBJ_DIR)/gdt: src/gdt.c
	@mkdir -p $(dir $@)
	$(CC) -Iinclude -o $@ $<

$(OBJ_DIR)/indices: src/indices.c
	@mkdir -p $(dir $@)
	$(CC) -Iinclude -o $@ $<

$(OBJ_DIR)/testelf: src/testelf.c
	@mkdir -p $(dir $@)
	$(CC) -Iinclude -o $@ $<

.PHONY: all
