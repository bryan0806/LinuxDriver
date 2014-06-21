#include <linux/kernel.h>
#include <linux/module.h>

static int __init hello_2_init(void)
{
    printk("<1>Hello!World\n");
    return 0;

}

static void __exit hello_2_exit(void)
{
    printk("<1>Bye bye\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);
