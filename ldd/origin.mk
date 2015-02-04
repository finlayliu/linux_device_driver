ifneq ($(KERNELRELEASE),)
	obj-m := char.o
	char-objs := char_main.o
else
	KERNELDIR := /lib/modules/$(shell uname -r)/build
	PWD :=$(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) 
clean:
	rm -rf *.o *.mod.o *.mod.c *.symvers modul* .h* .b* .M* .t*
endif

#obj-m := helloworld.o
#helloworld-objs := helloworld_main.o
#PWD :=$(shell pwd)
#make -C /lib/modules/3.12.22+/build SUBDIRS=$(PWD) modules
