/**
 * File:	lkmasg1_output.c

 * Class:	COP4600-SP23
 */

#include <linux/module.h>	  // Core header for modules.

#include <linux/device.h>	  // Supports driver model.

#include <linux/kernel.h>	  // Kernel header for convenient functions.

#include <linux/fs.h>		  // File-system support.

#include <linux/uaccess.h>	  // User access copy function support.

#include "lkmasg1_common.h"   // Include access to the header file

#define DEVICE_NAME "lkmasg1_output" // Device name.

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John Aedo");
MODULE_DESCRIPTION("lkmasg1 Output Kernel Module");
MODULE_VERSION("0.1");

//Prototype Functions
static int output_open(struct inode *, struct file *);
static int output_close(struct inode *, struct file *);
static ssize_t output_write(struct file *, const char *, size_t, loff_t *);


// File operations structure and the functions it points to.
static struct file_operations output_fops = {
    .owner = THIS_MODULE,
    .open = output_open,
    .release = output_close,
    .write = output_write,
};

int output_init_module(void) {
    // Initialization code specific to the output module
    return 0;
}

void output_cleanup_module(void) {
    // Cleanup code specific to the output module
}

static int output_open(struct inode *inodep, struct file *filep) {
    // Output module open implementation
    return 0;
}

static int output_close(struct inode *inodep, struct file *filep) {
    // Output module close implementation
    return 0;
}

static ssize_t output_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    // Output module write implementation
    return 0;
}