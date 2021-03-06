/*
 * Tiny TTY driver
 *
 * Copyright (C) 2002 Greg Kroah-Hartman (greg@kroah.com)
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 * This driver shows how to create a minimal tty driver.  It does not rely on
 * any backing hardware, but creates a timer that emulates data being received
 * from some kind of hardware.
 */

#include <linux/version.h>
#include <linux/config.h>
//#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/module.h>
#include <linux/sched.h>

#define DRIVER_VERSION "v1.0"
#define DRIVER_AUTHOR "Greg Kroah-Hartman <greg@kroah.com>"
#define DRIVER_DESC "Tiny TTY driver"

/* Module information */
MODULE_AUTHOR( DRIVER_AUTHOR );
MODULE_DESCRIPTION( DRIVER_DESC );
MODULE_LICENSE("GPL");

#define DELAY_TIME		HZ * 2	/* 2 seconds per character */
#define TINY_DATA_CHARACTER	't'

#define TINY_TTY_MAJOR		240	/* experimental range */
#define TINY_TTY_MINORS		255	/* use the whole major up */

struct tiny_serial {
	struct tty_struct	*tty;		/* pointer to the tty for this device */
	int			open_count;	/* number of times this port has been opened */
	struct semaphore	sem;		/* locks this structure */
	struct timer_list	*timer;
};

static int			tiny_refcount;
static struct tty_driver	tiny_tty_driver;
static struct tty_struct	*tiny_tty[TINY_TTY_MINORS];
static struct termios		*tiny_termios[TINY_TTY_MINORS];
static struct termios		*tiny_termios_locked[TINY_TTY_MINORS];
static struct tiny_serial	*tiny_table[TINY_TTY_MINORS];	/* initially all NULL */


static void tiny_timer (unsigned long data)
{
	struct tiny_serial *tiny = (struct tiny_serial *)data;
	struct tty_struct *tty;

	if (!tiny)
		return;

	tty = tiny->tty;

	if (tty->flip.count >= TTY_FLIPBUF_SIZE)
		tty_flip_buffer_push(tty);

	/* add two characters to the tty port */
	/* this doesn't actually push the data through unless tty->low_latency is set */
	tty_insert_flip_char(tty, TINY_DATA_CHARACTER, 0);
	tty_insert_flip_char(tty, '\n', 0);

	tty_flip_buffer_push(tty);
	tty_schedule_flip (tty);
	
	/* resubmit the timer again */
	tiny->timer->expires = jiffies + DELAY_TIME;
	add_timer (tiny->timer);
}

static int tiny_open (struct tty_struct *tty, struct file * filp)
{
	struct tiny_serial *tiny;
	struct timer_list *timer;

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
        try_module_get(THIS_MODULE);
#else
	MOD_INC_USE_COUNT;
#endif

	/* initialize the pointer incase something fails */
	tty->driver_data = NULL;

	/* get the serial object associated with this tty pointer */
	tiny = tiny_table[minor(tty->device)];
	if (tiny == NULL) {
		/* first time accessing this device, let's create it */
		tiny = kmalloc (sizeof (*tiny), GFP_KERNEL);
		if (!tiny) {
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
                    try_module_get(THIS_MODULE);
#else
                    MOD_INC_USE_COUNT;
#endif
			return -ENOMEM;
		}

		init_MUTEX (&tiny->sem);
		tiny->open_count = 0;
		tiny->timer = NULL;

		tiny_table[minor(tty->device)] = tiny;
	}

	down (&tiny->sem);

	/* save our structure within the tty structure */
	tty->driver_data = tiny;
	tiny->tty = tty;


	++tiny->open_count;
	if (tiny->open_count == 1) {
		/* this is the first time this port is opened */
		/* do any hardware initialization needed here */

		/* create our timer and submit it */
		if (!tiny->timer) {
			timer = kmalloc (sizeof (*timer), GFP_KERNEL);
			if (!timer) {
				up (&tiny->sem);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
                    try_module_get(THIS_MODULE);
#else
                    MOD_INC_USE_COUNT;
#endif
				return -ENOMEM;
			}
			tiny->timer = timer;
		}
		init_timer (tiny->timer);
		tiny->timer->data = (unsigned long )tiny;
		tiny->timer->expires = jiffies + DELAY_TIME;
		tiny->timer->function = tiny_timer;
		add_timer (tiny->timer);
	}

	up (&tiny->sem);
	return 0;
}

static void do_close (struct tiny_serial *tiny)
{
	down (&tiny->sem);

	if (!tiny->open_count) {
		/* port was never opened */
		return;
	}

	--tiny->open_count;
	if (tiny->open_count <= 0) {
		/* The port is being closed by the last user. */
		/* Do any hardware specific stuff here */

		/* shut down our timer */
		del_timer (tiny->timer);
	}

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0)
    module_put(THIS_MODULE);
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,4,0)
    MOD_DEC_USE_COUNT;
#endif

	up (&tiny->sem);
}

static void tiny_close (struct tty_struct *tty, struct file * filp)
{
	struct tiny_serial *tiny = tty->driver_data;

	if (!tiny) {
		return;
	}

	do_close (tiny);
}	

static int tiny_write (struct tty_struct * tty, int from_user, const unsigned char *buf, int count)
{
	struct tiny_serial *tiny = tty->driver_data;
	int retval = -EINVAL;

	if (!tiny) {
		return -ENODEV;
	}

	down (&tiny->sem);

	if (!tiny->open_count) {
		/* port was not opened */
		goto exit;
	}

	/* now send the data out the harware port */
	retval = count;

exit:
	up (&tiny->sem);
	return retval;
}

static int tiny_write_room (struct tty_struct *tty) 
{
	struct tiny_serial *tiny = tty->driver_data;
	int room;

	if (!tiny) {
		return -ENODEV;
	}

	down (&tiny->sem);
	
	if (!tiny->open_count) {
		/* port was not opened */
		return -EINVAL;
	}

	/* calculate how much room is left in the device */
	room = 255;

	up (&tiny->sem);

	return room;
}

static struct tty_driver tiny_tty_driver = {
	magic:			TTY_DRIVER_MAGIC,
	driver_name:		"tiny_tty",
#ifndef CONFIG_DEVFS_FS
	name:			"ttty",
#else
	name:			"tts/ttty%d",
#endif
	major:			TINY_TTY_MAJOR,
	num:			TINY_TTY_MINORS,
	type:			TTY_DRIVER_TYPE_SERIAL,
	subtype:		SERIAL_TYPE_NORMAL,
	flags:			TTY_DRIVER_REAL_RAW,
	
	refcount:		&tiny_refcount,
	table:			tiny_tty,
	termios:		tiny_termios,
	termios_locked:		tiny_termios_locked,

	open:			tiny_open,
	close:			tiny_close,
	write:			tiny_write,
	write_room:		tiny_write_room,
};

static int __init tiny_init(void)
{
	/* register the tty driver */
	tiny_tty_driver.init_termios          = tty_std_termios;
	tiny_tty_driver.init_termios.c_cflag  = B9600 | CS8 | CREAD | HUPCL | CLOCAL;
	if (tty_register_driver (&tiny_tty_driver)) {
		printk (KERN_ERR "failed to register tiny tty driver\n");
		return -1;
	}

	printk (KERN_INFO DRIVER_DESC " " DRIVER_VERSION "\n");

	return 0;
}

static void __exit tiny_exit(void)
{
	struct tiny_serial *tiny;
	int i;

	tty_unregister_driver(&tiny_tty_driver);

	/* shut down all of the timers and free the memory */
	for (i = 0; i < TINY_TTY_MINORS; ++i) {
		tiny = tiny_table[i];
		if (tiny) {
			/* close the port */
			while (tiny->open_count)
				do_close (tiny);

			/* shut down our timer and free the memory */
			del_timer (tiny->timer);
			kfree (tiny->timer);
			kfree (tiny);
			tiny_table[i] = NULL;
		}
	}
}

module_init(tiny_init);
module_exit(tiny_exit);
