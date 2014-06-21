// stop.c
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

static void __exit hello_2_exit(void)
{
    printk("<1>bye \n");
}
module_exit(hello_2_exit);
