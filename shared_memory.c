#include <linux/module.h>

#include <linux/init.h>

#include "shared_memory.h"



MODULE_LICENSE("GPL");



char shared_memory[SHARED_MEM_SIZE];



EXPORT_SYMBOL(shared_memory);



static int __init shared_memory_init(void)

{

    return 0;

}



static void __exit shared_memory_exit(void)

{

}



module_init(shared_memory_init);

module_exit(shared_memory_exit);

