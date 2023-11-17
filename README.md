---
tags: [teaching/cop4600, assignments, linux]
---
## Learning Objective 
To gain a fuller understanding of critical sections in the Linux kernel by programming them.

## Development 
Use gcc and the C programming language, on the version of Linux we have in our VM.

## Assignment
Re-implement your module from the device driver assignment as two separate kernel modules - one for an input device, and one for an output device, using shared memory between the two modules to manage the communication.

Don’t overthink the shared memory in the kernel – do a bit of Googling about sharing memory between kernel modules, and you’ll find that it’s as simple as an **extern** declaration in the right place.

What you *will* need to put some work into is properly guarding the critical sections.  Investigate the functionality in **linux/mutex.h**.  Derek Molloy’s documentation can help you here as well.

## Helpful References
Below are some resources that will help you structure a device driver.  Feel free to ask the professor and the teaching assistants for advice, but you should read over the relevant bits of these resources first.

Peter Jay Salzman, Michael Burian, Ori Pomerantz.  _The Linux Kernel Module Programming Guide._  [http://www.tldp.org/LDP/lkmpg/2.6/html/](http://www.tldp.org/LDP/lkmpg/2.6/html/)

Derek Molloy.  _Writing a Linux Kernel Module - Part 2: A Character Device_.  [http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/](http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/)

## Submitting

Zip up your code, and upload it to Webcourses.  Include the makefile.
