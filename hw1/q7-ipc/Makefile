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

# Track the CHILD of ipc with csprobe (load the module first: make load)
# Use a large ITERS so the run lasts long enough, e.g. make track ITERS=10000000
track: ipc
	./ipc & \
	sleep 0.5; \
	CHILD=$$(pgrep -nx ipc); \
	echo "tracking child pid $$CHILD"; \
	echo $$CHILD | sudo tee /sys/module/hw1_csprobe/parameters/target_pid; \
	for i in 1 2 3; do cat /proc/csprobe; echo; sleep 1; done; \
	wait

unload:
	sudo rmmod hw1_csprobe
	sudo dmesg | tail -n 20

clean:
	rm -f ipc
	$(MAKE) -C $(KDIR) M=$(PWD) clean

.PHONY: all ipc module run strace load unload track clean
endif