/**
 * stop.c - Kernel module cleanup implementation
 * 
 * Demonstrates proper kernel module cleanup handling
 */

#include <linux/init.h>     /* For module_exit macro */
#include <linux/module.h>   /* For module loading */
#include <linux/kernel.h>   /* For pr_info */

/* Module metadata */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple kernel module cleanup example");
MODULE_VERSION("2.0");

/**
 * simple_exit - Module cleanup function
 *
 * Performs cleanup operations when the module is unloaded.
 * Demonstrates proper resource cleanup and logging.
 */
static void __exit simple_exit(void)
{
    pr_info("Goodbye, world - kernel module unloading\n");
    
    /* Additional cleanup operations would go here */
}

module_exit(simple_exit);