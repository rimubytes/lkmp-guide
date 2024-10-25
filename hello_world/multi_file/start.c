/*
 * start.c - Kernel module initialization
 * A simple kernel module demonstrating initialization.
 */

#include <linux/init.h>     /* For module_init/exit macros */
#include <linux/module.h>   /* For module loading */
#include <linux/kernel.h>   /* For pr_info */

/* Module metadata */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple kernel module initialization example");
MODULE_VERSION("2.0");

/**
 * simple_init - Module initialization function
 *
 * Demonstrates basic kernel module initialization with
 * proper kernel logging.
 *
 * Return: 0 on success, negative error code on failure
 */
static int __init simple_init(void)
{
    pr_info("Hello, world - this is the kernel speaking\n");
    return 0;
}

module_init(simple_init);