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

static int device_open_func(struct inode *inode, struct file *file) {
    /**
     * Called when the device file is opened
     * 
     * @param inode: The inode of the device file
     * @param file: The file object associated with the open device
     * @return 0 on success, negative value on error 
     */
    printk(KERN_DEBUG "device_open(%p, %p\n)", inode, file);

    // Check if the device is already open
    if (device_open) {
        return -EBUSY;
    }

    device_open++;

    // Initialize the message
    sprintf(message, "If I told you once, I told you %d times - Hello, world\n", device_open);
    message_ptr = message;

    // Increment the module's use count to prevent it from being unloaded
    MOD_INC_USE_COUNT();

    return 0;
}

static int device_release_func(struct inode *inode, struct file *file) {
    /*
     * Called when the device file is closed.
     *
     * @param inode: The inode of the device file.
     * @param file: The file object associated with the closed device.
     * @return 0 on success.
     */
    printk(KERN_DEBUG "device_release(%p, %p)\n", inode, file);

    // Decrement the module's use count
    device_open--;
    MOD_DEC_USE_COUNT();

    return 0;
}