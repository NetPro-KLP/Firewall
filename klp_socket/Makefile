KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := klp_socket.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

CC = gcc

ifeq ($(ADDRSAFE), true)
	EXTRA_CFLAGS += -DKLP_SOCKET_ADDR_SAFE
endif

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules 

clean:
	rm -rf *.symvers *.ko *.o *.order .*.o.cmd .tmp_versions .*.ko.cmd *.mod.c