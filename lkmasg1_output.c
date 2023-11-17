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


// Important variables that store data and keep track of relevant information.
static int major_number;

static struct class *lkmasg1OutClass = NULL;	///< The device-driver class struct pointer
static struct device *lkmasg1OutDevice = NULL; ///< The device-driver device struct pointer


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


//Initializes module at installation
int output_init_module(void){

	printk(KERN_INFO "lkmasg1_output: installing module.\n");


	// Allocate a major number for the device.
	major_number = register_chrdev(0, DEVICE_NAME, &fops);

	if (major_number < 0){

		printk(KERN_ALERT "lkmasg1_output could not register number.\n");

		return major_number;

	}

	printk(KERN_INFO "lkmasg1_output: registered correctly with major number %d\n", major_number);


	// Register the device class
	lkmasg1OutClass = class_create(THIS_MODULE, CLASS_NAME);


    // Check for error and clean up if there is
	if (IS_ERR(lkmasg1OutClass)){

		unregister_chrdev(major_number, DEVICE_NAME);

		printk(KERN_ALERT "Failed to register device class\n");

		return PTR_ERR(lkmasg1OutClass); // Correct way to return an error on a pointer

	}

	printk(KERN_INFO "lkmasg1_output: device class registered correctly\n");


	// Register the device driver
	lkmasg1OutDevice = device_create(lkmasg1OutClass, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);


    // Clean up if there is an error
	if (IS_ERR(lkmasg1OutDevice)){

        // Repeated code but the alternative is goto statements
		class_destroy(lkmasg1OutClass);

		unregister_chrdev(major_number, DEVICE_NAME);

		printk(KERN_ALERT "Failed to create the device\n");

		return PTR_ERR(lkmasg1OutDevice);

	}


	printk(KERN_INFO "lkmasg1_output: device class created correctly\n"); // Made it! device was initialized
	return 0;
}

// Removes module, sends appropriate message to kernel
void cleanup_module(void){

	printk(KERN_INFO "lkmasg1_output: removing module.\n");

	// Remove the device
	device_destroy(lkmasg1OutClass, MKDEV(major_number, 0));

	// Unregister the device class
	class_unregister(lkmasg1OutClass);

	// Remove the device class
	class_destroy(lkmasg1OutClass);

	// Unregister the major number
	unregister_chrdev(major_number, DEVICE_NAME);

	printk(KERN_INFO "lkmasg1_output: Goodbye from the LKM!\n");
	unregister_chrdev(major_number, DEVICE_NAME);

	return;
}


// Print to kernel that the device was opened
static int output_open(struct inode *inodep, struct file *filep) {

    printk(KERN_INFO "lkmasg1_output: device opened.\n");
	return 0;
}


// Print to kernel that the device was closed
static int output_close(struct inode *inodep, struct file *filep) {

    printk(KERN_INFO "lkmasg1_output: device closed.\n");
	return 0;
}

// Writes from buffer to user space
static ssize_t output_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {

    int space_available = BUFFER_SIZE - buffer_head;

    // Ensure the necessary memory exists within the buffer
    if (space_available == 0) {
        printk(KERN_INFO "lkmasg1_output: no space available for writing.\n");
        return -ENOSPC;
    }


    int bytes_to_copy = min(len, space_available);

    // Catch any issues copying data from user space
    if (copy_from_user(mainBuffer + buffer_head, buffer, bytes_to_copy) != 0) {
        printk(KERN_ALERT "lkmasg1_output: failed to copy data from user space.\n");
        return -EFAULT;
    }


    buffer_head += bytes_to_copy;


    printk(KERN_INFO "lkmasg1_output: %d bytes written to the output device.\n", bytes_to_copy);
    return bytes_to_copy;
}

module_init(output_init_module);
module_exit(output_cleanup_module);