#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>

// To do: include workqueue.h
#include<linux/workqueue.h>


#include <linux/interrupt.h>

struct my_data{
	int len;
	char *buf;
	unsigned long jiffies;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20)
	struct work_struct mytq_task;
#endif
} mytq_data;

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20)
void mytq_fun (struct work_struct *work)
#else
void mytq_fun (void *ptr)
#endif
{
  // To do: implement your defered codes
  struct my_data *data= container_of(work, struct my_data, mytq_task);

  if( in_interrupt() ) {
   printk("<1>In interruct context\n");
  }
  else {
   printk("<1>In process context\n");
  }

  printk("In mytq_fun, jiffies=%ld, len=%d\n", jiffies, data->len);
  printk("The message is :%s\n", data->buf);
}

int init_module(void)
{
	int len = 100;
	char *buf = "Hello from mytq_fun";
	
	mytq_data.len = len ;
	mytq_data.buf = buf ;
	mytq_data.jiffies = jiffies ;
	
	printk ("queued task at jiffies = %ld\n", jiffies );

	INIT_WORK(&mytq_data.mytq_task, mytq_fun);

	schedule_work(&mytq_data.mytq_task);

  return 0;
}

void cleanup_module(void)
{
	printk ("I cleaned up, jiffies = %ld\n", jiffies );
}

