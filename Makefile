# Makefile for lkmasg1 kernel modules



obj-m += lkmasg1_input.o

obj-m += lkmasg1_output.o



# Path to the kernel source code

KDIR := /lib/modules/$(shell uname -r)/build



# Current directory

PWD := $(shell pwd)



all: lkmasg1_input.ko lkmasg1_output.ko



lkmasg1_input.ko: lkmasg1_input.c lkmasg1_common.h

	$(MAKE) -C $(KDIR) M=$(PWD) modules



lkmasg1_output.ko: lkmasg1_output.c lkmasg1_common.h

	$(MAKE) -C $(KDIR) M=$(PWD) modules



clean:

	$(MAKE) -C $(KDIR) M=$(PWD) clean

