CC = gcc
CFLAGS = -Wall -O2 -pthread
SRC = main.c
BIN = parallel-sum

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

run: all
	@for t in 1 2 4; do \
		echo "=== $$t thread(s) ==="; \
		./$(BIN) $$t; \
		echo; \
	done

clean:
	rm -f $(BIN)

.PHONY: all run clean