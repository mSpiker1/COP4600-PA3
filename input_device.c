#include <linux/module.h>

#include <linux/device.h>

#include <linux/fs.h>

#include <linux/uaccess.h>

#include <linux/mutex.h>

#include "shared_memory.h"



#define INPUT_DEVICE_NAME "input_device"



MODULE_LICENSE("GPL");

MODULE_AUTHOR("John Aedo");

MODULE_DESCRIPTION("Input Device Kernel Module");

MODULE_VERSION("0.1");



static int major_number;

static struct class *inputClass = NULL;

static struct device *inputDevice = NULL;

static struct mutex input_mutex;



// Function prototypes

static int input_open(struct inode *inodep, struct file *filep);

static int input_close(struct inode *inodep, struct file *filep);

static ssize_t input_write(struct file *filep, const char *buffer, size_t len, loff_t *offset);



static struct file_operations input_fops = {

    .owner = THIS_MODULE,

    .open = input_open,

    .release = input_close,

    .write = input_write,

};



int init_module(void)

{

    printk(KERN_INFO "input_device: installing module.\n");



    major_number = register_chrdev(0, INPUT_DEVICE_NAME, &input_fops);

    if (major_number < 0)

    {

        printk(KERN_ALERT "input_device: could not register number.\n");

        return major_number;

    }



    printk(KERN_INFO "input_device: registered correctly with major number %d\n", major_number);



    inputClass = class_create(THIS_MODULE, "char");

    if (IS_ERR(inputClass))

    {

        unregister_chrdev(major_number, INPUT_DEVICE_NAME);

        printk(KERN_ALERT "Failed to register device class\n");

        return PTR_ERR(inputClass);

    }



    inputDevice = device_create(inputClass, NULL, MKDEV(major_number, 0), NULL, INPUT_DEVICE_NAME);

    if (IS_ERR(inputDevice))

    {

        class_destroy(inputClass);

        unregister_chrdev(major_number, INPUT_DEVICE_NAME);

        printk(KERN_ALERT "Failed to create the device\n");

        return PTR_ERR(inputDevice);

    }



    mutex_init(&input_mutex);



    return 0;

}



void cleanup_module(void)

{

    printk(KERN_INFO "input_device: removing module.\n");

    device_destroy(inputClass, MKDEV(major_number, 0));

    class_unregister(inputClass);

    class_destroy(inputClass);

    unregister_chrdev(major_number, INPUT_DEVICE_NAME);

    printk(KERN_INFO "input_device: Goodbye from the input device!\n");

}



static int input_open(struct inode *inodep, struct file *filep)

{

    printk(KERN_INFO "input_device: device opened.\n");

    return 0;

}



static int input_close(struct inode *inodep, struct file *filep)

{

    printk(KERN_INFO "input_device: device closed.\n");

    return 0;

}



static ssize_t input_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)

{

    int space_available;

    int bytes_to_copy;



    mutex_lock(&input_mutex);



    space_available = SHARED_MEM_SIZE - strlen(shared_memory);



    if (space_available == 0)

    {

        mutex_unlock(&input_mutex);

        printk(KERN_INFO "input_device: no space available for writing.\n");

        return -ENOSPC;

    }



    bytes_to_copy = min_t(size_t, len, space_available);



    if (copy_from_user(shared_memory + strlen(shared_memory), buffer, bytes_to_copy) != 0)

    {

        mutex_unlock(&input_mutex);

        printk(KERN_ALERT "input_device: failed to copy data from user space.\n");

        return -EFAULT;

    }



    printk(KERN_INFO "input_device: %d bytes written to the device.\n", bytes_to_copy);



    mutex_unlock(&input_mutex);



    return bytes_to_copy;

}

