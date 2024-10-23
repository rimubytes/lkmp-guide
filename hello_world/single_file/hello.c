#include <linux/init.h>  /*For module_init and module_exit macros*/
#include <linux/module.h> /*For all modules*/
#include <linux.kernel.h> /*For KERN_INFO and printk*/

MODULE_DESCRIPTION("A simple Kello World Linux Kernel module");
MODULE_VERSION("2.0");

/**
 * hello_init - Initialize the kernel module
 * Prints a hello message when the module is loaded
 * Return: 0 on success, negative error code in failure
 */

static int __init hello_init(void) {
    pr_info("Hello, world - this is the kernel speaking\n");
    return 0;
}

/**
 * hello_exit - Cleanup when the module is unloaded
 * Prints a goodbye message when the module is unloaded
 */

static void __exit hello_exit(void) {
    pr_info("Goodbye, world - kernel module\n");
}

module_init(hello_init); /* Register module entry point*/
module_exit(hello_exit); /* Register module exit point*/