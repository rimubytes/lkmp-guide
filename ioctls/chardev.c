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