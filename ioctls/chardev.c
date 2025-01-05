/*
 * chardev.c
 *
 * Create an input/output character device with ioctl support
 * Copyright (C) 1998-99 by Ori Pomerantz
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/wrapper.h>
#include <asm/uaccess.h>
#include "chardev.h"

/* Device configuration */
#define DEVICE_NAME     "char_dev"
#define DEVICE_MAX_LEN  80
#define SUCCESS         0

/* Device state */
static int device_open = 0;
static char device_message[DEVICE_MAX_LEN];
static char *message_ptr;

/**
 * device_open - Called when a process attempts to open the device file
 * @inode: Pointer to inode object
 * @file: Pointer to file object
 *
 * Handles device opening and initialization
 * 
 * Return: 0 on success, -EBUSY if device is already open
 */

static int device_open(struct inode *inode, struct file *file)
{
    if (device_open)
        return -EBUSY;

    device_open++;
    message_ptr = device_message;
    MOD_INC_USE_COUNT;
    
    return SUCCESS;
}

/**
 * device_release - Called when a process closes the device file
 * @inode: Pointer to inode object
 * @file: Pointer to file object
 *
 * Handles device cleanup and release
 * 
 * Return: 0 on success
 */
static int device_release(struct inode *inode, struct file *file)
{
    device_open--;
    MOD_DEC_USE_COUNT;
    return SUCCESS;
}

/**
 * device_read - Called when a process reads from the device
 * @file: Pointer to file object
 * @buffer: Buffer to fill with data
 * @length: Length of the buffer
 * @offset: Offset in the file
 *
 * Copies data from kernel space to user space
 * 
 * Return: Number of bytes read
 */

static ssize_t device_write(struct file *file, const char *buffer,
                           size_t length, loff_t *offset)
{
    int i;

    for (i = 0; i < length && i < DEVICE_MAX_LEN; i++)
        get_user(device_message[i], buffer + i);

    message_ptr = device_message;
    return i;
}

/**
 * device_ioctl - Called when a process performs an ioctl on the device
 * @inode: Pointer to inode object
 * @file: Pointer to file object
 * @ioctl_num: IOCTL command number
 * @ioctl_param: IOCTL parameter
 *
 * Handles various IOCTL commands for the device
 * 
 * Return: Depends on the IOCTL command
 */

static int device_ioctl(struct inode *inode, struct file *file,
                       unsigned int ioctl_num, unsigned long ioctl_param)
{
    int i;
    char *temp;
    char ch;

    switch (ioctl_num) {
    case IOCTL_SET_MSG:
        temp = (char *)ioctl_param;
        get_user(ch, temp);
        for (i = 0; ch && i < DEVICE_MAX_LEN; i++, temp++)
            get_user(ch, temp);
        device_write(file, (char *)ioctl_param, i, 0);
        break;

    case IOCTL_GET_MSG:
        i = device_read(file, (char *)ioctl_param, DEVICE_MAX_LEN - 1, 0);
        put_user('\0', (char *)ioctl_param + i);
        break;

    case IOCTL_GET_NTH_BYTE:
        return device_message[ioctl_param];
    }

    return SUCCESS;
}

/* File operations structure */
static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .ioctl = device_ioctl,
    .open = device_open,
    .release = device_release
};

/**
 * init_module - Module initialization function
 *
 * Registers the character device
 * 
 * Return: 0 on success, negative on failure
 */
static int __init init_module(void)
{
    int ret;

    ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register device: %d\n", ret);
        return ret;
    }

    printk(KERN_INFO "Device registered successfully with major number %d\n", MAJOR_NUM);
    printk(KERN_INFO "Create device with: mknod %s c %d 0\n", 
           DEVICE_FILE_NAME, MAJOR_NUM);

    return SUCCESS;
}

/**
 * cleanup_module - Module cleanup function
 *
 * Unregisters the character device
 */
static void __exit cleanup_module(void)
{
    int ret;

    ret = unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
    if (ret < 0)
        printk(KERN_ERR "Error unregistering device: %d\n", ret);
    else
        printk(KERN_INFO "Device unregistered successfully\n");
}

module_init(init_module);
module_exit(cleanup_module);