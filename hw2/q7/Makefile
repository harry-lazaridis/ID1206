CC = gcc
CFLAGS = -Wall -O0

my_allocator: q7.c
	$(CC) $(CFLAGS) -o my_allocator q7.c

run: my_allocator
	/usr/bin/time --verbose ./my_allocator 1024

clean:
	rm -f my_allocator

.PHONY: run clean

