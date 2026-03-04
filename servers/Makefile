OBJ_DIR ?= target
CC := gcc

all: $(OBJ_DIR)/dummy


$(OBJ_DIR)/dummy: src/dummy.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ $<

.PHONY: all
