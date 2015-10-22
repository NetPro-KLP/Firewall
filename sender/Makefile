KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := sender.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

CC = gcc


all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules 

clean:
	rm -rf *.symvers *.ko *.o *.order .*.o.cmd .tmp_versions .*.ko.cmd *.mod.c
