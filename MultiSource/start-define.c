// start.c
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h> // for __init and __exit

static int __init hello_2_init(void)
{
    printk("<1>Hello\n");
#ifdef __TEST__
    printk("<1>define __TEST__\n");
#endif
    return 0;
}
module_init(hello_2_init);

