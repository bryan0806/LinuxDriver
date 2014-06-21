// start.c
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h> // for __init and __exit
// This is how to define global variables
static long mylong=9999;
static char *mystring = "blah";

module_param(mylong,long,0644);
module_param(mystring,charp,0);

static int __init hello_2_init(void)
{
    printk("<1>Hello\n");
#ifdef __TEST__
    printk("<1>define __TEST__\n");
#endif
    printk("<1>my defined long mylong is %li\n",mylong);
    printk("<1>my defined string is %s\n",mystring);	
    return 0;
}
module_init(hello_2_init);

