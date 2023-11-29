obj-m += lkmasg2_writer.o lkmasg2_reader.o



all:

	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

	gcc -o test test.c

clean:

	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

	rm -f test
