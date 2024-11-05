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

static ssize_t device_read_func(struct file *file, char *buffer, size_t length, loff_t *offset) {
    /*
     * Called when the device file is read from.
     *
     * @param file: The file object associated with the read operation.
     * @param buffer: The buffer to fill with data.
     * @param length: The length of the buffer.
     * @param offset: The offset in the file.
     * @return The number of bytes read, and the new offset in the file.
     */
    int bytes_read = 0;

    // If we're at the end of the message, return 0 (which signifies end of file)
    if (*message_ptr == 0) {
        return 0;
    }

    // Copy the data from the message to the buffer
    while (length && *message_ptr) {
        // Because the buffer is in the user data segment, not the kernel data segment,
        // assignment wouldn't work. Instead, we have to use put_user which copies data
        // from the kernel data segment to the user data segment.
        put_user(*message_ptr, buffer);
        message_ptr++;
        buffer++;
        length--;
        bytes_read++;
    }

    printk(KERN_DEBUG "Read %d bytes, %d left\n", bytes_read, length);

    return bytes_read;
}

static ssize_t device_write_func(struct file *file, const char *buffer, size_t length, loff_t *offset) {
    /*
     * Called when the device file is written to, but not implemented in this example.
     *
     * @param file: The file object associated with the write operation.
     * @param buffer: The buffer containing the data to write.
     * @param length: The length of the buffer.
     * @param offset: The offset in the file.
     * @return -EINVAL, indicating that writing to the device is not supported.
     */
    return -EINVAL;
}

static struct file_operations fops = {
    .read = device_read_func,
    .write = device_write_func,
    .open = device_open_func,
    .release = device_release_func,
};

static int __init init_module(void) {
    /*
     * Initializes the module by registering the character device.
     *
     * @return 0 on success, negative value on error.
     */
    int major;

    // Register the character device
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ERR "Sorry, registering the character device failed with %d\n", major);
        return major;
    }

    printk(KERN_INFO "Registration is a success. The major device number is %d.\n", major);
    printk(KERN_INFO "If you want to talk to the device driver, you'll have to create a device file.\n");
    printk(KERN_INFO "We suggest you use: mknod <name> c %d <minor>\n", major);
    printk(KERN_INFO "You can try different minor numbers and see what happens.\n");

    return 0;
}

static void __exit cleanup_module(void) {
    /*
     * Cleans up the module by unregistering the character device.
     */
    int ret;

    // Unregister the device
    ret = unregister_chrdev(major, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ERR "Error in unregister_chrdev: %d\n", ret);
    }
}

module_init(init_module);
module_exit(cleanup_module);