/**
 This is a header file with common dependencies and
 resources that both kernel module files will access
 */

#ifndef LKMASG1_COMMON_H
#define LKMASG1_COMMON_H

#define BUFFER_SIZE 1024
#define CLASS_NAME "char"


// Circular buffer to store data in FIFO fashion

extern char mainBuffer[BUFFER_SIZE];
extern int buffer_head;
extern int buffer_tail;

#endif