CC = gcc
CFLAGS = -Wall -O2 -pthread
BIN = scheduling scheduling-aging

all: $(BIN)

scheduling: main.c
	$(CC) $(CFLAGS) -o $@ $^

scheduling-aging: main-aging.c
	$(CC) $(CFLAGS) -o $@ $^

run: all
	@echo "=== Original: non-preemptive priority ==="
	./scheduling
	@echo
	@echo "=== Modified: time quantum + aging ==="
	./scheduling-aging

clean:
	rm -f $(BIN)

.PHONY: all run clean
