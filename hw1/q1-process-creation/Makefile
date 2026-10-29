CC = gcc
CGLAGS = -Wall -O2
SRC = main.c
BIN = q1

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: all
	./$(BIN)
	@echo "Antal processer:"
	@./$(BIN) | wc -l

clean:
	rm -f $(BIN)

.PHONY: all run clean