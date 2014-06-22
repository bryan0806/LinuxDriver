obj-m += hw1-1.o
hw1-1-objs := start.o stop.o

all:
	make -C /lib/modules/2.6.23.1-42.fc8/build M=`pwd` modules
clean:
	make -C /lib/modules/2.6.23.1-42.fc8/build M=`pwd` clean

