#include <linux/module.h>

#include <linux/device.h>

#include <linux/kernel.h>

#include <linux/fs.h>

#include <linux/uaccess.h>

#include "lkmasg1_common.h"



#define DEVICE_NAME "lkmasg1_input"

MODULE_LICENSE("GPL");

MODULE_AUTHOR("John Aedo");

MODULE_DESCRIPTION("lkmasg1 Input Kernel Module");

MODULE_VERSION("0.1");



static int major_number;

static struct class *lkmasg1InClass = NULL;

static struct device *lkmasg1InDevice = NULL;



// Prototype functions for file operations.

static int input_open(struct inode *, struct file *);

static int input_close(struct inode *, struct file *);

static ssize_t input_read(struct file *, char *, size_t, loff_t *);



static struct file_operations input_fops = {

    .owner = THIS_MODULE,

    .open = input_open,

    .release = input_close,

    .read = input_read,

};



int input_init_module(void) {

    printk(KERN_INFO "lkmasg1_input: installing module.\n");



    major_number = register_chrdev(0, DEVICE_NAME, &input_fops);



    if (major_number < 0) {

        printk(KERN_ALERT "lkmasg1_input could not register number.\n");

        return major_number;

    }



    printk(KERN_INFO "lkmasg1_input: registered correctly with major number %d\n", major_number);



    lkmasg1InClass = class_create(THIS_MODULE, CLASS_NAME);



    if (IS_ERR(lkmasg1InClass)) {

        unregister_chrdev(major_number, DEVICE_NAME);

        printk(KERN_ALERT "Failed to register device class\n");

        return PTR_ERR(lkmasg1InClass);

    }



    printk(KERN_INFO "lkmasg1_input: device class registered correctly\n");



    lkmasg1InDevice = device_create(lkmasg1InClass, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);



    if (IS_ERR(lkmasg1InDevice)) {

        class_destroy(lkmasg1InClass);

        unregister_chrdev(major_number, DEVICE_NAME);

        printk(KERN_ALERT "Failed to create the device\n");

        return PTR_ERR(lkmasg1InDevice);

    }



    printk(KERN_INFO "lkmasg1_input: device class created correctly\n");

    return 0;

}



void input_cleanup_module(void) {

    printk(KERN_INFO "lkmasg1_input: removing module.\n");



    device_destroy(lkmasg1InClass, MKDEV(major_number, 0));

    class_unregister(lkmasg1InClass);

    class_destroy(lkmasg1InClass);

    unregister_chrdev(major_number, DEVICE_NAME);



    printk(KERN_INFO "lkmasg1_input: Goodbye from the LKM!\n");

    unregister_chrdev(major_number, DEVICE_NAME);



    return;

}



static int input_open(struct inode *inodep, struct file *filep) {

    printk(KERN_INFO "lkmasg1_input: device opened.\n");

    return 0;

}



static int input_close(struct inode *inodep, struct file *filep) {

    printk(KERN_INFO "lkmasg1_input: device closed.\n");

    return 0;

}



static ssize_t input_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {

    int bytes_to_copy;

    size_t bytes_available;



    mutex_lock(&buffer_mutex);



    bytes_available = buffer_head - buffer_tail;



    if (bytes_available == 0) {

        mutex_unlock(&buffer_mutex);

        printk(KERN_INFO "lkmasg1_input: nothing to read.\n");

        return 0;

    }



    bytes_to_copy = min(len, bytes_available);



    if (copy_to_user(buffer, mainBuffer + buffer_tail, bytes_to_copy) != 0) {

        mutex_unlock(&buffer_mutex);

        printk(KERN_ALERT "lkmasg1_input: failed to copy data to user space.\n");

        return -EFAULT;

    }



    buffer_tail += bytes_to_copy;



    memmove(mainBuffer, mainBuffer + buffer_tail, buffer_head - buffer_tail);

    buffer_head -= buffer_tail;

    buffer_tail = 0;



    mutex_unlock(&buffer_mutex);



    printk(KERN_INFO "lkmasg1_input: %d bytes read from the device.\n", bytes_to_copy);

    return bytes_to_copy;

}



module_init(input_init_module);

module_exit(input_cleanup_module);

