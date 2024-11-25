/**
 * /proc file system is designed to allow easy access to information about
 * process, its used by every bit of the kernel which has something interesting
 * to report.
 * 
 * Creates a file in /proc filesystem that demonstrates basic procfs usage.
 * The file will display a message with an incrementing counter each time it's read.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/version.h>

/* Configuration */
#define PROC_FILENAME "test"
#define MAX_BUFFER_SIZE 80
#define PROC_PERMS (S_IFREG | S_IRUGO) /* Regular file, read-only for all */

/* Static variables */
static char message_buffer[MAX_BUFFER_SIZE];
static int access_count = 1;

/**
 * procfile_read - Callback function for reading the proc file
 * @buffer: Buffer provided by the kernel to store data
 * @buffer_location: Pointer to the actual data location
 * @offset: Current position in the file
 * @buffer_length: Size of the provided buffer
 * @zero: Unused parameter (reserved for future use)
 *
 * This function is called when a user reads from our proc file.
 * It generates a message with an incrementing counter and appropriate ordinal suffix.
 *
 * Return: Number of bytes written, 0 if EOF, negative on error
 */

static int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int zero)
{
    int len:
    const char *suffix;

    /* Return EOF if offset is non-zero (we only support single read) */
    if (offset > 0)
        return 0;

    /* Determine the appropriate ordinal suffix */
    if (access_count % 100 >= 11 && access_count % 100 <= 13)
        suffix = "th";
    else {
        switch (access_count % 10) {
            case 1: suffix = "st"; break;
            case 2: suffix = "nd"; break;
            case 3: suffix = "rd"; break;
            default: suffix = "th"; break;
        }
    }
    /* Generate the message */
    len = snprintf(message_buffer, MAX_BUFFER_SIZE, "For the %d%s time, go away!\n",
    access_count, suffix);

    access_count++;

    /* Set the buffer location and return length */
    *buffer_location = message_buffer;
    return len;
}