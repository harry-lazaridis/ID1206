CC = gcc
CFLAGS = -Wall -O2 -pthread
SRC = main.c
BIN = scheduling

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: all
	./$(BIN)

clean:
	rm -f $(BIN)

.PHONY: all run clean