ROOTDIR ?= .
TARGET ?= $(ROOTDIR)/target/tools
CC := gcc
CPPFLAGS := -Iinclude

all: $(TARGET)/gdt $(TARGET)/indices $(TARGET)/stacks $(TARGET)/testelf


$(TARGET)/gdt: src/gdt.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -o $@ $<

$(TARGET)/indices: src/indices.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -o $@ $<

$(TARGET)/stacks: src/stacks.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -o $@ $<

$(TARGET)/testelf: src/testelf.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -o $@ $<

.PHONY: all
