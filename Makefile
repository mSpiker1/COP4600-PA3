obj-m += lkmasg1_common.o lkmasg1_input.o lkmasg1_output.o

all: lkmasg1_input.o lkmasg1_output.o

lkmasg1_common.o: lkmasg1_common.c lkmasg1_common.h
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) $@

lkmasg1_input.o: lkmasg1_input.c lkmasg1_common.h
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) $@

lkmasg1_output.o: lkmasg1_output.c lkmasg1_common.h
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) $@

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f test