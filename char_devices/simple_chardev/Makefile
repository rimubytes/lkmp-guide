# Makefile for chardev.c

# Kernel source directory
KDIR ?= /lib/modules/$(shell uname -r)/build

# Module name
MODULE_NAME := chardev

# Source and object files
obj-m := $(MODULE_NAME).o
$(MODULE_NAME)-objs := chardev.o

# Compile flags
EXTRA_CFLAGS := -DDEBUG

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

install:
	sudo insmod $(MODULE_NAME).ko
	sudo chmod 666 /dev/$(MODULE_NAME)

uninstall:
	sudo rmmod $(MODULE_NAME)
	sudo rm /dev/$(MODULE_NAME)

.PHONY: all clean install uninstall