
obj-m += master.o
obj-m += slave.o
#EXTRA_CFLAGS=-DMODDEP

all:
	make -C /lib/modules/2.6.32/build M=`pwd` modules
	cp -a *.ko /lib/modules/2.6.32/kernel/drivers/char/
	depmod

clean:
	make -C /lib/modules/2.6.32/build M=`pwd` clean
	
