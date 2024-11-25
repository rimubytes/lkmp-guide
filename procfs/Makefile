# Makefile for procfs kernel module

# Module name
MODULE_NAME := procfs

# Kernel source directory (can be overridden)
KDIR ?= /lib/modules/$(shell uname -r)/build

# Compilation flags
EXTRA_CFLAGS := -DDEBUG
EXTRA_CFLAGS += -Wall -Wextra -Werror

# Source files
obj-m += $(MODULE_NAME).o

# Default target
all: module

# Compile the kernel module
module:
	@echo "Building kernel module $(MODULE_NAME)"
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Clean up compiled files
clean:
	@echo "Cleaning up build files"
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f Module.markers Module.symvers