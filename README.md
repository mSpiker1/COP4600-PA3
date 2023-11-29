
## How to use

## Step 1 - change directories to your folder
First, you need to use *cd /foldername* to change directories into the folder you have the files stored in.

![image](https://github.com/mSpiker1/COP4600-PA3/assets/114204417/267a928d-f10e-4d23-894a-4f51b57df6c2)

## Step 2 - Make the files
Next, you will want to make the files by using *make all*. This will also create the test file. To clean make files or test files before re-creating, you can do *make clean* then *make all*.

![image](https://github.com/mSpiker1/COP4600-PA3/assets/114204417/4380f7c0-4fdf-4a75-b481-e9718ebf8906)

This process makes the modules into .ko files, which we will install next.

## Step 3 - Install files
Now, we must install the .ko files made in the previous step.

To do this, use *sudo insmod lkmasg2_writer.ko* _MAKE SURE YOU INSTALL THE WRITER FIRST_.

![image](https://github.com/mSpiker1/COP4600-PA3/assets/114204417/19160726-d135-45bd-aa56-50805ade85f7)

The writer must be installed first so the reader knows to read from shared memory exported in the writer.

Next, use *sudo insmod lkmasg2_reader.ko* to install the reader.

![image](https://github.com/mSpiker1/COP4600-PA3/assets/114204417/ce147c1a-321c-43f5-bb5b-bf165b832b00)

## Step 4 - Test the program
Finally, run *sudo ./test*. The test file opens the correct directory (/dev/lkmasg2_writer/reader) and runs the message variable to test it.
You can run *sudo dmesg* to view the kernel output.

![image](https://github.com/mSpiker1/COP4600-PA3/assets/114204417/e11c9a97-c61f-479d-b8a6-87cae8ca319f)


To test a different message, go to the test.c file and change the message variable then re-make all to re-compile the code:

![image](https://github.com/mSpiker1/COP4600-PA3/assets/114204417/d3810c4b-450d-4304-b64c-d9c1539ec77a)

## Appendix
Here are some other cases that are handled correctly when a message is blank or over the capacity:

![image](https://github.com/mSpiker1/COP4600-PA3/assets/114204417/31b6cc8a-09e6-456d-9e90-17b1e964c9cc)
![image](https://github.com/mSpiker1/COP4600-PA3/assets/114204417/8c044369-5d74-4aed-bbe6-1834a7ab1a26)



