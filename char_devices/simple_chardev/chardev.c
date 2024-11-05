// The original purpose of device files is to allow processes to communicate with device
// drivers in the kernel, and through them with physical devices (modems, terminals, etc.).

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/wrapper.h>
#include <linux/version.h>
#include <asm/uaccess.h>

// Device configuration
#define DEVICE_NAME "char_dev"
#define BUF_LEN 80

// Device state
static int device_open = 0;
static char message[BUF_LEN];
static char* message_ptr;

