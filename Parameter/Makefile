obj-m += startstop.o
startstop-objs := start.o stop.o
EXTRA_CFLAGS += -D__TEST__

all:
	make -C /lib/modules/2.6.32/build M=/root/driver/Parameter modules
clean:
	make -C /lib/modules/2.6.32/build M=/root/driver/Parameter clean

