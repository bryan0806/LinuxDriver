// start.c
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h> // for __init and __exit
// This is how to define global variables
//#define VERBOSE 0

static int VERBOSE=0;
static char *name = "Bryan Liu";
static char *id = "MEME2803";
static char *email ="chunwei.liu@gmail.com";

module_param(VERBOSE,int,0644);
module_param(name,charp,0644);
module_param(id,charp,0644);
module_param(email,charp,0644);

static int __init printinfo(void)
{
    //printk("VERBOSE IS %d\n",VERBOSE);
if(VERBOSE != 0){
    printk("<1>Name: %s\n",name);	
    printk("<1>ID: %s\n",id);
    printk("<1>Email: %s\n",email);
}	
else{
    printk("<1>ID: %s\n",id);
//#endif
}
    return 0;
}
module_init(printinfo);

