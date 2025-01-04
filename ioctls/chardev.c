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


