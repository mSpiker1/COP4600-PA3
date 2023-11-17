---
tags: [teaching/cop4600, assignments, linux]
---
## Learning Objective 
To gain a fuller understanding of kernel modules by creating one.

## Development 
Use gcc and the C programming language, on the version of Linux we have in our VM.

## Skeleton Project
A skeleton project has been made available via Github for your convenience:
https://github.com/johnaedo/kernel-mod-skeleton

Of note, it includes a Makefile that is capable of building both the module and the test program.  You can `make clean` if you need to rebuild from scratch.  It will remove all of the intermediate files, the test program, and the kernel module itself.  *It will not uninstall the kernel module, however.*  You will need to rmmod to unload the module.

Also, remember that this code runs entirely in privileged mode.  You will need to sudo for all commands, including testing:
- insmod, rmmod, dmesg, and test all require sudo.

## Assignment
Write a character-mode Linux device driver as a kernel module.

### Your driver must:

#### Handle its lifecycle:
- Successfully initialize and de-initialize itself, including registering itself and obtaining a new major device number.
- Report using **printk** each time its character device is opened, closed, read or written.  (This is done for you in the project skeleton.)

#### Handle data:

- Store bytes written to it up to a constant buffer size (at least 1KB)
- Allow them to be read back out in FIFO fashion
- Remove them from the buffer as they are read back out
- Implement these features via the following functions:
	- open()
	- close()
	- read()
	- write()

#### Handle errors:

- If not enough buffer is available to store a write request, the driver must store only up to the amount available
- If not enough data is available to service a read request, the driver must respond with only the amount available (including 0 bytes)

## Testing
The skeleton project includes the code for a testing utility that you can use to test your driver.  

USAGE:  test \<path to driver\>
EXAMPLE:  test /dev/lkmpa1


## Helpful References
Below are some resources that will help you structure a device driver.  Feel free to ask the professor and the teaching assistants for advice, but you should read over the relevant bits of these resources first.

Peter Jay Salzman, Michael Burian, Ori Pomerantz.  _The Linux Kernel Module Programming Guide._  [http://www.tldp.org/LDP/lkmpg/2.6/html/](http://www.tldp.org/LDP/lkmpg/2.6/html/)

Derek Molloy.  _Writing a Linux Kernel Module - Part 2: A Character Device_.  [http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/](http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/)

## Submitting

- You will provide ONE C source file and ONE Makefile.  If you use the skeleton project, just fill in the missing code and turn in the lkmasg1.c file.