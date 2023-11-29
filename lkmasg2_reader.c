#include <linux/module.h>

#include <linux/fs.h>

#include <linux/uaccess.h>

#include <linux/mutex.h>

#include <linux/device.h>



#define DEVICE_NAME "lkmasg2_reader"

#define BUFFER_SIZE 1025



MODULE_LICENSE("GPL");

MODULE_AUTHOR("Group5");

MODULE_DESCRIPTION("lkmasg2 Reader Kernel Module");

MODULE_VERSION("0.1");



static int major_number;

static struct class *reader_class = NULL;

static struct device *reader_device = NULL;



extern char shared_buffer[BUFFER_SIZE]; // Declaration without definition

extern struct mutex buffer_mutex;       // Declaration without definition

extern int buffer_head;



static ssize_t read(struct file *, char *, size_t, loff_t *);

static struct file_operations fops = {

    .owner = THIS_MODULE,

    .read = read,

};



int init_module(void)

{

    printk(KERN_INFO "lkmasg2 Reader module successfully installed.\n");



    major_number = register_chrdev(0, DEVICE_NAME, &fops);

    if (major_number < 0)

    {

        printk(KERN_ALERT "lkmasg2 Reader - Error encountered: %s\n", "lkmasg2 Reader could not register number.");

        return major_number;

    }



    reader_class = class_create(THIS_MODULE, DEVICE_NAME);

    if (IS_ERR(reader_class))

    {

        unregister_chrdev(major_number, DEVICE_NAME);

        printk(KERN_ALERT "lkmasg2 Reader - Error encountered: %s\n", "Failed to register device class.");

        return PTR_ERR(reader_class);

    }



    reader_device = device_create(reader_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);

    if (IS_ERR(reader_device))

    {

        class_destroy(reader_class);

        unregister_chrdev(major_number, DEVICE_NAME);

        printk(KERN_ALERT "lkmasg2 Reader - Error encountered: %s\n", "Failed to create the device.");

        return PTR_ERR(reader_device);

    }



    mutex_init(&buffer_mutex);



    return 0;

}



void cleanup_module(void)

{

    printk(KERN_INFO "lkmasg2 Reader: Removing module.\n");

    device_destroy(reader_class, MKDEV(major_number, 0));

    class_unregister(reader_class);

    class_destroy(reader_class);

    unregister_chrdev(major_number, DEVICE_NAME);

}



static ssize_t read(struct file *filep, char *buffer, size_t len, loff_t *offset)

{

    int bytes_available;

    int bytes_to_copy;

    size_t start_position;



    printk(KERN_INFO "lkmasg2 Reader - Entered read().\n");



    mutex_lock(&buffer_mutex);

    printk(KERN_INFO "lkmasg2 Reader - Acquired the lock.\n");

    

    bytes_available = buffer_head;

    

    if (bytes_available == 0)

    {

        printk(KERN_INFO "lkmasg2 Reader - Buffer is empty, unable to read.\n");

        mutex_unlock(&buffer_mutex);

        printk(KERN_INFO "lkmasg2 Reader - Exiting read() function.\n");

        return 0;

    }



    bytes_to_copy = min_t(int, len, bytes_available); // Automatically truncates



    // Adjust the starting position in the buffer based on buffer_head

    start_position = (BUFFER_SIZE + buffer_head - bytes_available) % BUFFER_SIZE;



    // Displays truncation message if necessary

    if (bytes_to_copy > bytes_available)

    {

        printk(KERN_INFO "lkmasg2 Reader - Buffer has %d bytes of content, requested %d\n", bytes_available, bytes_to_copy);

    }



    if (copy_to_user(buffer, shared_buffer + start_position, bytes_to_copy) != 0)

    {

        printk(KERN_ALERT "lkmasg2 Reader - Error encountered: %s\n", "Failed to copy data to user space.");

        mutex_unlock(&buffer_mutex);

        return -EFAULT;

    }



    buffer_head -= bytes_to_copy;



    // Reset buffer_head to the end if it reaches the beginning

    if (buffer_head < 0)

        buffer_head = BUFFER_SIZE - 1;



    printk(KERN_INFO "lkmasg2 Reader - Read %d bytes from the buffer.\n", bytes_to_copy);



    mutex_unlock(&buffer_mutex);



    printk(KERN_INFO "lkmasg2 Reader - Exiting read() function.\n");



    return bytes_to_copy;

}

