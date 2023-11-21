obj-m += input_device.o

obj-m += output_device.o

obj-m += shared_memory.o



all:

	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules



clean:

	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

