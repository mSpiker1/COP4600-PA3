/**

 * This is a header file containing common dependencies &

 * resources that both kernel module files will access

 */

#ifndef LKMASG1_COMMON_H
#define LKMASG1_COMMON_H
#include <linux/mutex.h>
#define BUFFER_SIZE 1024
#define CLASS_NAME "char"

// Circular buffer to store data in FIFO fashion

// Declared with extern to keep open to both modules

extern char mainBuffer[BUFFER_SIZE];

extern int buffer_head;

extern int buffer_tail;
// Mutex to guard access to shared data

extern struct mutex buffer_mutex;
#endif

