/**
 * File:	lkmasg1_input.c

 * Class:	COP4600-SP23
*/

#include <linux/module.h>	  // Core header for modules.

#include <linux/device.h>	  // Supports driver model.

#include <linux/kernel.h>	  // Kernel header for convenient functions.

#include <linux/fs.h>		  // File-system support.

#include <linux/uaccess.h>	  // User access copy function support.

#include "lkmasg1_common.h"   // Include access to the header file

#define DEVICE_NAME "lkmasg1_input" // Device name.

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John Aedo");
MODULE_DESCRIPTION("lkmasg1 Input Kernel Module");
MODULE_VERSION("0.1");


// Important variables that store data and keep track of relevant information.
static int major_number;

static struct class *lkmasg1InClass = NULL;	///< The device-driver class struct pointer
static struct device *lkmasg1InDevice = NULL; ///< The device-driver device struct pointer


// Prototype functions for file operations.
static int input_open(struct inode *, struct file *);
static int input_close(struct inode *, struct file *);
static ssize_t input_read(struct file *, char *, size_t, loff_t *);


// File operations structure and the functions it points to.
static struct file_operations fops = {
		.owner = THIS_MODULE,
		.open = open,
		.release = close,
		.read = read,
		.write = write,
};


//Initializes module at installation
int input_init_module(void){

	printk(KERN_INFO "lkmasg1_input: installing module.\n");

	// Allocate a major number for the device.
	major_number = register_chrdev(0, DEVICE_NAME, &fops);

	if (major_number < 0){

		printk(KERN_ALERT "lkmasg1_input could not register number.\n");
		return major_number;
	}

	printk(KERN_INFO "lkmasg1_input: registered correctly with major number %d\n", major_number);

	// Register the device class
	lkmasg1InClass = class_create(THIS_MODULE, CLASS_NAME);

	// Check for error and clean up if there is
	if (IS_ERR(lkmasg1InClass)){

		unregister_chrdev(major_number, DEVICE_NAME);

		printk(KERN_ALERT "Failed to register device class\n");

		return PTR_ERR(lkmasg1InClass); // Correct way to return an error on a pointer

	}

	printk(KERN_INFO "lkmasg1_input: device class registered correctly\n");

	// Register the device driver
	lkmasg1InDevice = device_create(lkmasg1InClass, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);


	// Clean up if there is an error
	if (IS_ERR(lkmasg1InDevice)){

		// Repeated code but the alternative is goto statements
		class_destroy(lkmasg1InClass);

		unregister_chrdev(major_number, DEVICE_NAME);

		printk(KERN_ALERT "Failed to create the device\n");

		return PTR_ERR(lkmasg1InDevice);
	}

	printk(KERN_INFO "lkmasg1_input: device class created correctly\n"); // Made it! device was initialized
	return 0;
}


// Removes module, sends appropriate message to kernel
void input_cleanup_module(void){

	printk(KERN_INFO "lkmasg1_input: removing module.\n");

	// Remove the device
	device_destroy(lkmasg1InClass, MKDEV(major_number, 0));

	// Unregister the device class
	class_unregister(lkmasg1InClass);

	// Remove the device class
	class_destroy(lkmasg1InClass);

	// Unregister the major number
	unregister_chrdev(major_number, DEVICE_NAME);

	printk(KERN_INFO "lkmasg1_input: Goodbye from the LKM!\n");
	unregister_chrdev(major_number, DEVICE_NAME);

	return;
}


// Print to kernel that the device was opened
static int input_open(struct inode *inodep, struct file *filep){

	printk(KERN_INFO "lkmasg1_input: device opened.\n");
	return 0;
}


// Print to kernel that the device was closed
static int input_close(struct inode *inodep, struct file *filep){

	printk(KERN_INFO "lkmasg1_input: device closed.\n");
	return 0;
}


// Reads from device, displays in userspace, and deletes the read data
static ssize_t input_read(struct file *filep, char *buffer, size_t len, loff_t *offset){

    int bytes_to_copy;
    int bytes_available = buffer_head - buffer_tail;

    if (bytes_available == 0){

        printk(KERN_INFO "lkmasg1_input: nothing to read.\n");
        return 0;
    }


    bytes_to_copy = min(len, bytes_available);


    if (copy_to_user(buffer, mainBuffer + buffer_tail, bytes_to_copy) != 0){

        printk(KERN_ALERT "lkmasg1_input: failed to copy data to user space.\n");
        return -EFAULT;
    }


    buffer_tail += bytes_to_copy;


    // Remove the read data from mainBuffer
    memmove(mainBuffer, mainBuffer + buffer_tail, buffer_head - buffer_tail);
    buffer_head -= buffer_tail;
    buffer_tail = 0;

    
    printk(KERN_INFO "lkmasg1_input: %d bytes read from the device.\n", bytes_to_copy);
    return bytes_to_copy;
}

module_init(input_init_module);
module_exit(input_cleanup_module);