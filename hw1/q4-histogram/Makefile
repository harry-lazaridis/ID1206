CC = gcc
CFLAGS = -Wall -O2 -pthread
SRC = main.c
BIN = histogram

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: all
	@echo "=== 3 threads, 800 elements, 16 bins ==="
	./$(BIN) 3 800 16
	@echo
	@echo "=== 3 threads, 80000 elements, 16 bins ==="
	./$(BIN) 3 80000 16
	@echo
	@echo "=== 3 threads, 8000000 elements, 16 bins ==="
	./$(BIN) 3 8000000 16

clean:
	rm -f $(BIN)

.PHONY: all run clean