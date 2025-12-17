OBJ_DIR ?= target
CC := gcc

all: $(OBJ_DIR)/gdt $(OBJ_DIR)/indices


$(OBJ_DIR)/gdt: gdt.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ $<

$(OBJ_DIR)/indices: indices.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ $<

.PHONY: all
