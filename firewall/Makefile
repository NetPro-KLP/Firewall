KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := firewall.o
firewall-objs := queue.o trie.o kstring.o drop.o list.o hash.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

CC = gcc


all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules 

clean:
	rm -rf *.symvers *.ko *.o *.order .drop.* .*.o.cmd .tmp_versions .*.ko.cmd *.mod.c
