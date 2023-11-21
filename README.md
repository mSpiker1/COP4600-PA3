## How To Use
First, make sure you have all files in the same directory.
Use *cd directory_name* in your terminal to navigate to the directory.

## Step 1 - Make the files

Run *make clean* to clean any make files before re-making them. You're going to want to do this when you recompile code to make sure you have the latest build.

Run *make* to create the share_memory.ko, input_device.ko, and output_device.ko files.

## Step 2 - Install the modules

If you already have modules installed from previous build attempts, go ahead and remove them using *sudo rmmod module_name*.
To see if you have any previous modules loaded that you built, you can use *ls /dev/* to view the kernel modules.

To install the modules, run:

*sudo insmod shared_memory.ko* - Do this first, as both devices need the shared memory to be initialized.
*sudo insmod input_device.ko*
*sudo insmod output_device.ko*

These installs the modules created in the previous step.

## Step 3 - Testing

If you run *sudo dmesg* you can see all the messages written to the kernel, which is where we are going to get out printk statements from out modules.

Let's test the program. First, use *sudo dmesg* to view messages.

Then, use *echo "This is a test sentence." | sudo tee /dev/input_device*. This command inputs the sentence to the input device where it is written to the shared memory.

Use *sudo dmesg* to view updates.

Finally, use *sudo cat /dev/output_device* This command reads from the output device, which should show the test message written above.

Use one last *sudo dmesg* to view the kernel print statements after executing.
