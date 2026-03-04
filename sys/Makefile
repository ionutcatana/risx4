ROOTDIR ?= .
ARCH ?= x86_64

all: drivers risx servers


drivers:
	$(MAKE) -C $@ ROOTDIR=$(ROOTDIR) ARCH=$(ARCH)

risx:
	$(MAKE) -C $@ ROOTDIR=$(ROOTDIR) ARCH=$(ARCH)

servers:
	$(MAKE) -C $@ ROOTDIR=$(ROOTDIR) ARCH=$(ARCH)

.PHONY: all drivers risx servers
