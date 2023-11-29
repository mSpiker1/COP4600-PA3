#include <linux/module.h>

#include <linux/device.h>

#include <linux/fs.h>

#include <linux/uaccess.h>

#include <linux/mutex.h>

#include "shared_memory.h"

#include <linux/kernel.h>

#include <linux/slab.h>



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

    pr_info("lkmasg2 Input Device module installing.\n");



    major_number = register_chrdev(0, INPUT_DEVICE_NAME, &input_fops);

    if (major_number < 0)

    {

        pr_alert("lkmasg2 input_device: could not register number.\n");

        return major_number;

    }



    pr_info("lkmasg2 input_device: registered correctly with major number %d\n", major_number);



    inputClass = class_create(THIS_MODULE, "char");

    if (IS_ERR(inputClass))

    {

        unregister_chrdev(major_number, INPUT_DEVICE_NAME);

        pr_alert("lkmasg2 Failed to register device class\n");

        return PTR_ERR(inputClass);

    }



    inputDevice = device_create(inputClass, NULL, MKDEV(major_number, 0), NULL, INPUT_DEVICE_NAME);

    if (IS_ERR(inputDevice))

    {

        class_destroy(inputClass);

        unregister_chrdev(major_number, INPUT_DEVICE_NAME);

        pr_alert("lkmasg2 Failed to create the device\n");

        return PTR_ERR(inputDevice);

    }



    mutex_init(&input_mutex);



    pr_info("lkmasg2 Input Device module installed.\n");



    return 0;

}



void cleanup_module(void)

{

    device_destroy(inputClass, MKDEV(major_number, 0));

    class_unregister(inputClass);

    class_destroy(inputClass);

    unregister_chrdev(major_number, INPUT_DEVICE_NAME);

    pr_info("lkmasg2 input_device: Removing Module. Goodbye from the input device!\n");

}



static int input_open(struct inode *inodep, struct file *filep)

{

    pr_info("lkmasg2 input_device: device opened.\n");

    return 0;

}



static int input_close(struct inode *inodep, struct file *filep)

{

    pr_info("lkmasg2 input_device: device closed.\n");

    return 0;

}



static ssize_t input_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)

{

    int space_available;

    size_t bytes_to_copy;



    pr_info("lkmasg2 Writer - Entered write().\n");



    mutex_lock(&input_mutex);



    pr_info("lkmasg2 Writer - Acquired the lock.\n");



    space_available = SHARED_MEM_SIZE - strlen(shared_memory);



    if (space_available == 0)

    {

        mutex_unlock(&input_mutex);

        pr_info("lkmasg2 Writer - Buffer is full, unable to write.\n");

        return -ENOSPC; // Insufficient space

    }



    if (len > space_available)

    {

        pr_info("lkmasg2 Writer - Buffer has %d bytes remaining, attempting to write %zu, truncating input.\n", space_available, len);

        len = space_available; // Truncate to available space

    }



    bytes_to_copy = min_t(size_t, len, space_available);



    // Copy data from user space to shared_memory

    if (copy_from_user(shared_memory + strlen(shared_memory), buffer, bytes_to_copy) != 0)

    {

        mutex_unlock(&input_mutex);

        pr_alert("lkmasg2 Writer - Error encountered: %s\n", "Failed to copy data from user space");

        return -EFAULT;

    }



    // Update the null-terminated buffer size

    shared_memory[SHARED_MEM_SIZE - 1] = '\0';



    pr_info("lkmasg2 Writer - Wrote %zu bytes to the buffer.\n", bytes_to_copy);



    mutex_unlock(&input_mutex);

    pr_info("lkmasg2 Writer - Exiting write() function\n");



    return bytes_to_copy;

}















