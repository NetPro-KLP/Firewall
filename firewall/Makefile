KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := firewall.o
firewall-objs := queue.o trie.o hook.o list.o hash.o expired.o klp_socket.o firewall_main.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

CC = gcc

ifeq ($(ADDRSAFE), true)
	EXTRA_CFLAGS += -DKLP_SOCKET_ADDR_SAFE
endif

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules 

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -rf *.symvers *.ko *.o *.order .*.o.cmd .tmp_versions .*.ko.cmd *.mod.c
