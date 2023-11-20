#include <linux/module.h>

#include <linux/device.h>

#include <linux/kernel.h>

#include <linux/fs.h>

#include <linux/uaccess.h>

#include "lkmasg1_common.h"



#define DEVICE_NAME "lkmasg1_output"

MODULE_LICENSE("GPL");

MODULE_AUTHOR("John Aedo");

MODULE_DESCRIPTION("lkmasg1 Output Kernel Module");

MODULE_VERSION("0.1");



static int major_number;

static struct class *lkmasg1OutClass = NULL;

static struct device *lkmasg1OutDevice = NULL;



//Prototype Functions

static int output_open(struct inode *, struct file *);

static int output_close(struct inode *, struct file *);

static ssize_t output_write(struct file *, const char *, size_t, loff_t *);



static struct file_operations output_fops = {

    .owner = THIS_MODULE,

    .open = output_open,

    .release = output_close,

    .write = output_write,

};



int output_init_module(void) {

    printk(KERN_INFO "lkmasg1_output: installing module.\n");



    major_number = register_chrdev(0, DEVICE_NAME, &output_fops);



    if (major_number < 0) {

        printk(KERN_ALERT "lkmasg1_output could not register number.\n");

        return major_number;

    }



    printk(KERN_INFO "lkmasg1_output: registered correctly with major number %d\n", major_number);



    lkmasg1OutClass = class_create(THIS_MODULE, CLASS_NAME);



    if (IS_ERR(lkmasg1OutClass)) {

        unregister_chrdev(major_number, DEVICE_NAME);

        printk(KERN_ALERT "Failed to register device class\n");

        return PTR_ERR(lkmasg1OutClass);

    }



    printk(KERN_INFO "lkmasg1_output: device class registered correctly\n");



    lkmasg1OutDevice = device_create(lkmasg1OutClass, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);



    if (IS_ERR(lkmasg1OutDevice)) {

        class_destroy(lkmasg1OutClass);

        unregister_chrdev(major_number, DEVICE_NAME);

        printk(KERN_ALERT "Failed to create the device\n");

        return PTR_ERR(lkmasg1OutDevice);

    }



    printk(KERN_INFO "lkmasg1_output: device class created correctly\n");

    return 0;

}



void output_cleanup_module(void) {

    printk(KERN_INFO "lkmasg1_output: removing module.\n");



    device_destroy(lkmasg1OutClass, MKDEV(major_number, 0));

    class_unregister(lkmasg1OutClass);

    class_destroy(lkmasg1OutClass);

    unregister_chrdev(major_number, DEVICE_NAME);



    printk(KERN_INFO "lkmasg1_output: Goodbye from the LKM!\n");

    unregister_chrdev(major_number, DEVICE_NAME);



    return;

}



static int output_open(struct inode *inodep, struct file *filep) {

    printk(KERN_INFO "lkmasg1_output: device opened.\n");

    return 0;

}



static int output_close(struct inode *inodep, struct file *filep) {

    printk(KERN_INFO "lkmasg1_output: device closed.\n");

    return 0;

}



static ssize_t output_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {

    size_t bytes_available;

    int bytes_to_copy;



    mutex_lock(&buffer_mutex);



    bytes_available = BUFFER_SIZE - buffer_head;



    if (bytes_available == 0) {

        mutex_unlock(&buffer_mutex);

        printk(KERN_INFO "lkmasg1_output: no space available for writing.\n");

        return -ENOSPC;

    }



    bytes_to_copy = min(len, bytes_available);



    if (copy_from_user(mainBuffer + buffer_head, buffer, bytes_to_copy) != 0) {

        mutex_unlock(&buffer_mutex);

        printk(KERN_ALERT "lkmasg1_output: failed to copy data from user space.\n");

        return -EFAULT;

    }



    buffer_head += bytes_to_copy;



    mutex_unlock(&buffer_mutex);



    printk(KERN_INFO "lkmasg1_output: %d bytes written to the output device.\n", bytes_to_copy);

    return bytes_to_copy;

}



module_init(output_init_module);

module_exit(output_cleanup_module);

