#KERNELDIR = /lib/modules/$(shell uname -r)/build
KERNELDIR = /lib/modules/$(shell uname -mrsn | awk '{print $3}')/build

obj-m := firewall.o
firewall-objs := queue.o trie.o hook.o list.o hash.o expired.o ksocket.o firewall_main.o request.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

CC = gcc

ifeq ($(ADDRSAFE), true)
	EXTRA_CFLAGS += -DKSOCKET_ADDR_SAFE
endif

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules 

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -rf *.symvers *.ko *.o *.order .*.o.cmd .tmp_versions .*.ko.cmd *.mod.c
