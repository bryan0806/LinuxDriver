// master.c
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h> //need for __init and __exit
#include <linux/kmod.h> //need for request_module
#include "slave.h"

static int __init master_init(void)
{
    printk("<1>master in\n");
    slave_test();
    return 0;
}

static void __exit master_exit(void)
{
}

module_init(master_init);
module_exit(master_exit);
MODULE_LICENSE("GPL");
