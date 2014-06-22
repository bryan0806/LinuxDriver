obj-m += master.o
obj-m += slave.o

all:
	make -C /lib/modules/2.6.32/build M=`pwd` modules
clean:
	make -C /lib/modules/2.6.32/build M=`pwd` clean

