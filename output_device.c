#include <linux/module.h>

#include <linux/device.h>

#include <linux/fs.h>

#include <linux/uaccess.h>

#include <linux/mutex.h>

#include "shared_memory.h"



#define OUTPUT_DEVICE_NAME "output_device"



MODULE_LICENSE("GPL");

MODULE_AUTHOR("John Aedo");

MODULE_DESCRIPTION("Output Device Kernel Module");

MODULE_VERSION("0.1");



static int major_number;

static struct class *outputClass = NULL;

static struct device *outputDevice = NULL;

static struct mutex output_mutex;



// Function prototypes

static int output_open(struct inode *inodep, struct file *filep);

static int output_close(struct inode *inodep, struct file *filep);

static ssize_t output_read(struct file *filep, char *buffer, size_t len, loff_t *offset);



static struct file_operations output_fops = {

    .owner = THIS_MODULE,

    .open = output_open,

    .release = output_close,

    .read = output_read,

};



int init_module(void)

{

    printk(KERN_INFO "output_device: installing module.\n");



    major_number = register_chrdev(0, OUTPUT_DEVICE_NAME, &output_fops);

    if (major_number < 0)

    {

        printk(KERN_ALERT "output_device: could not register number.\n");

        return major_number;

    }



    printk(KERN_INFO "output_device: registered correctly with major number %d\n", major_number);



    outputClass = class_create(THIS_MODULE, "char_output");

    if (IS_ERR(outputClass))

    {

        unregister_chrdev(major_number, OUTPUT_DEVICE_NAME);

        printk(KERN_ALERT "Failed to register device class\n");

        return PTR_ERR(outputClass);

    }



    outputDevice = device_create(outputClass, NULL, MKDEV(major_number, 0), NULL, OUTPUT_DEVICE_NAME);

    if (IS_ERR(outputDevice))

    {

        class_destroy(outputClass);

        unregister_chrdev(major_number, OUTPUT_DEVICE_NAME);

        printk(KERN_ALERT "Failed to create the device\n");

        return PTR_ERR(outputDevice);

    }



    mutex_init(&output_mutex);



    return 0;

}



void cleanup_module(void)

{

    printk(KERN_INFO "output_device: removing module.\n");

    device_destroy(outputClass, MKDEV(major_number, 0));

    class_unregister(outputClass);

    class_destroy(outputClass);

    unregister_chrdev(major_number, OUTPUT_DEVICE_NAME);

    printk(KERN_INFO "output_device: Goodbye from the output device!\n");

}



static int output_open(struct inode *inodep, struct file *filep)

{

    printk(KERN_INFO "output_device: device opened.\n");

    return 0;

}



static int output_close(struct inode *inodep, struct file *filep)

{

    printk(KERN_INFO "output_device: device closed.\n");

    return 0;

}



static ssize_t output_read(struct file *filep, char *buffer, size_t len, loff_t *offset)

{

    int bytes_to_copy;

    int data_length;



    mutex_lock(&output_mutex);



    data_length = strlen(shared_memory);



    if (data_length == 0)

    {

        mutex_unlock(&output_mutex);

        printk(KERN_INFO "output_device: no data available for reading.\n");

        return 0; // EOF

    }



    bytes_to_copy = min_t(size_t, len, data_length);



    if (copy_to_user(buffer, shared_memory, bytes_to_copy) != 0)

    {

        mutex_unlock(&output_mutex);

        printk(KERN_ALERT "output_device: failed to copy data to user space.\n");

        return -EFAULT;

    }



    // Shift remaining data to the beginning of the shared memory

    memmove(shared_memory, shared_memory + bytes_to_copy, data_length - bytes_to_copy + 1);



    printk(KERN_INFO "output_device: %d bytes read from the device.\n", bytes_to_copy);



    mutex_unlock(&output_mutex);



    return bytes_to_copy;

}

