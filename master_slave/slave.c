#include <linux/kernel.h>
#include <linux/module.h>

#ifdef MODDEP
void slave_test(void) {
	printk("slave_test() called\n");
}

EXPORT_SYMBOL(slave_test);
#endif

int init_module(void) {
	printk("<1>Slave Hello\n");

	return 0;
}

void cleanup_module(void) {
	printk("<1>Slave Bye\n");

}

