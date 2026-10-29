CC = gcc
CGLAGS = -Wall -O2
SRC = main.c
BIN = q2

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: all
	./$(BIN)

clean:
	rm -f $(BIN)

.PHONY: all run clean