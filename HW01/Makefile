obj-m += module_param.o
module_param-objs := module_start.o module_stop.o

all:
	make -C /lib/modules/2.6.32/build M=`pwd` modules
clean:
	make -C /lib/modules/2.6.32/build M=`pwd` clean

