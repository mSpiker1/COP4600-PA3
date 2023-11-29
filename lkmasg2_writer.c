#include <linux/module.h>

#include <linux/fs.h>

#include <linux/uaccess.h>

#include <linux/mutex.h>

#include <linux/device.h>



#define DEVICE_NAME "lkmasg2_writer"

#define BUFFER_SIZE 1025



MODULE_LICENSE("GPL");

MODULE_AUTHOR("Group5");

MODULE_DESCRIPTION("lkmasg2 Writer Kernel Module");

MODULE_VERSION("0.1");



static int major_number;

static struct class *writer_class = NULL;

static struct device *writer_device = NULL;

char shared_buffer[BUFFER_SIZE];

struct mutex buffer_mutex;

int buffer_head = 0;



EXPORT_SYMBOL_GPL(shared_buffer); // Export the buffer

EXPORT_SYMBOL_GPL(buffer_mutex);  // Export the mutex

EXPORT_SYMBOL_GPL(buffer_head);   // Export the buffer_head



static ssize_t write(struct file *, const char *, size_t, loff_t *);



static struct file_operations fops = {

    .owner = THIS_MODULE,

    .write = write,

};



int init_module(void)

{

    printk(KERN_INFO "lkmasg2 Writer module successfully installed.\n");



    major_number = register_chrdev(0, DEVICE_NAME, &fops);

    if (major_number < 0)

    {

        printk(KERN_ALERT "lkmasg2 Writer - Error encountered: %s\n", "lkmasg2 Writer could not register number.");

        return major_number;

    }



    writer_class = class_create(THIS_MODULE, DEVICE_NAME);

    if (IS_ERR(writer_class))

    {

        unregister_chrdev(major_number, DEVICE_NAME);

        printk(KERN_ALERT "lkmasg2 Writer - Error encountered: %s\n", "Failed to register device class.");

        return PTR_ERR(writer_class);

    }



    writer_device = device_create(writer_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);

    if (IS_ERR(writer_device))

    {

        class_destroy(writer_class);

        unregister_chrdev(major_number, DEVICE_NAME);

        printk(KERN_ALERT "lkmasg2 Writer - Error encountered: %s\n", "Failed to create the device.");

        return PTR_ERR(writer_device);

    }



    mutex_init(&buffer_mutex);



    return 0;

}



void cleanup_module(void)

{

    printk(KERN_INFO "lkmasg2 Writer: Removing module.\n");

    device_destroy(writer_class, MKDEV(major_number, 0));

    class_unregister(writer_class);

    class_destroy(writer_class);

    unregister_chrdev(major_number, DEVICE_NAME);

}



static ssize_t write(struct file *filep, const char *buffer, size_t len, loff_t *offset)

{

    int space_available;

    int bytes_to_copy;



    printk(KERN_INFO "lkmasg2 Writer - Entered write().\n");



    mutex_lock(&buffer_mutex);

    printk(KERN_INFO "lkmasg2 Writer - Acquired the lock.\n");



    space_available = BUFFER_SIZE - buffer_head - 1; // - 1 for null sentinel

    

    if (len > space_available)

    {

        printk(KERN_INFO "lkmasg2 Writer - Buffer has %d bytes remaining, attempting to write %d, truncating input.\n", space_available, (int)len);

        len = space_available; // Truncate the input to the space available

    }



    if (space_available == 0)

    {

        printk(KERN_INFO "lkmasg2 Writer - Buffer is full, unable to write.\n");

        mutex_unlock(&buffer_mutex);

        return -ENOSPC;

    }



    bytes_to_copy = min((int)len, space_available);



    if (copy_from_user(shared_buffer + buffer_head, buffer, bytes_to_copy) != 0)

    {

        printk(KERN_ALERT "lkmasg2 Writer - Error encountered: %s\n", "Failed to copy data from user space.");

        mutex_unlock(&buffer_mutex);

        return -EFAULT;

    }



    buffer_head += bytes_to_copy;



    // Reset buffer_head to the beginning if it reaches the end

    if (buffer_head == BUFFER_SIZE)

        buffer_head = 0;



    printk(KERN_INFO "lkmasg2 Writer - Wrote %d bytes to the buffer.\n", bytes_to_copy);



    mutex_unlock(&buffer_mutex);



    printk(KERN_INFO "lkmasg2 Writer - Exiting write() function.\n");



    return bytes_to_copy;

}

