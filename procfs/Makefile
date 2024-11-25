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

# Install the module
install:
	@echo "Installing $(MODULE_NAME) module"
	sudo insmod ./$(MODULE_NAME).ko
	@echo "Checking module status"
	lsmod | grep $(MODULE_NAME)
	@echo "Module proc file created at /proc/$(MODULE_NAME)"

# Uninstall the module
uninstall:
	@echo "Removing $(MODULE_NAME) module"
	sudo rmmod $(MODULE_NAME)
	@echo "Verifying module removal"
	lsmod | grep $(MODULE_NAME) || echo "Module successfully unloaded"

# Show module information
info:
	@echo "Module Name: $(MODULE_NAME)"
	@echo "Kernel Version: $(shell uname -r)"
	@echo "Kernel Source: $(KDIR)"

# Debugging target to check module parameters
debug:
	@echo "Debug information:"
	modinfo ./$(MODULE_NAME).ko

# Check module for potential issues
checkpatch:
	@if [ -x /usr/src/linux/scripts/checkpatch.pl ]; then \
		/usr/src/linux/scripts/checkpatch.pl -f $(MODULE_NAME).c; \
	else \
		echo "checkpatch.pl not found"; \
	fi

# Create documentation
docs:
	@mkdir -p docs
	@kerneldoc -html $(MODULE_NAME).c > docs/$(MODULE_NAME)_docs.html

# Phony targets
.PHONY: all module clean install uninstall info debug checkpatch docs

