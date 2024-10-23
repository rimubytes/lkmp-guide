# Makefile for the hello kernel module
obj-m := hello.o

# Get the kernel build directory
KDIR := /lib/modules/$(shell uname -r)/build

# Get current working directory
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

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Build the kernel module (default)"
	@echo "  clean   - Remove generated files"
	@echo "  install - Install the module and update dependencies"
	@echo "  help    - Show this help message"