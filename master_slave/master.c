#include <linux/kernel.h>
#include <linux/module.h>
#ifdef MODDEP
#include "slave.h"
#else
#include <linux/kmod.h>
#endif

int init_module(void) {
	printk("<1>Master Hello\n");
#ifdef MODDEP
	slave_test();
#else
	request_module("slave");
#endif

	return 0;
}

void cleanup_module(void) {
	printk("<1>Master Bye\n");

}
