ifneq ($(KERNELRELEASE),)
obj-m := hw1-csprobe.o
else
CC     = gcc
ITERS ?= 100000
CFLAGS = -Wall -O2 -DITERS=$(ITERS)
KDIR  ?= /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)

all: ipc module

ipc: main.c
	$(CC) $(CFLAGS) -o $@ $^

module:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

run: ipc
	./ipc

strace: ipc
	strace -f -c -e trace=read,write ./ipc

load: module
	sudo insmod hw1-csprobe.ko
	sudo dmesg | tail -n 20

unload:
	sudo rmmod hw1_csprobe
	sudo dmesg | tail -n 20

clean:
	rm -f ipc
	$(MAKE) -C $(KDIR) M=$(PWD) clean

.PHONY: all module run strace load unload clean
endif