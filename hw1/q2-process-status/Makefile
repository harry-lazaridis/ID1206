CC = gcc
CGLAGS = -Wall -O2
SRC = main.c
BIN = q2

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: all
	./$(BIN) & \
	sleep 1; \
	ps -o pid,ppid,stat,cmd -C $(BIN); \
	wait


clean:
	rm -f $(BIN)

.PHONY: all run clean