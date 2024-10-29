# Makefile for multi-file kernel module
# Defines the module name and its source

# Module name
obj-m := hello.o

# Source files that make up the module
hello-objs := start.o stop.o

# Get kernel build directory
KDIR := /lib/modules/$(shell uname -r)/build

# Get current directory
PWD := $(shell pwd)

# Default target
all: module

# Build the module
module:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Clean built files
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

# Install the module
install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install
	depmod -a

# Debug target - shows variables
debug:
	@echo "Module name: $(obj-m)"
	@echo "Object files: $(hello-objs)"
	@echo "Kernel directory: $(KDIR)"
	@echo "Current directory: $(PWD)"

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Build the kernel module (default)"
	@echo "  clean   - Remove generated files"
	@echo "  install - Install the module and update dependencies"
	@echo "  debug   - Show build variables"
	@echo "  help    - Show this help message"

.PHONY: all module clean install debug help