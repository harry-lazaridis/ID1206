CC      = gcc
CFLAGS  = -O0 -Wall -Wextra -pthread
TARGET  = hw2_page_reclamation
N       ?= 64
M       ?= 200

.PHONY: all run q3 q4 q5 tsan clean

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $<

# Run with any N and M: make run N=64 M=500
run: $(TARGET)
	./$(TARGET) $(N) $(M)

# Question 3: N=64, M=20
q3: $(TARGET)
	@echo "===== Q3: N=64, M=20 ====="
	./$(TARGET) 64 20

# Question 4: N=64, M=200
q4: $(TARGET)
	@echo "===== Q4: N=64, M=200 ====="
	./$(TARGET) 64 200

# Question 5: N=64, M=2000
q5: $(TARGET)
	@echo "===== Q5: N=64, M=2000 ====="
	./$(TARGET) 64 2000

# Correctness check: build with ThreadSanitizer and look for data races
tsan: main.c
	$(CC) -g -fsanitize=thread -pthread -o $(TARGET)_tsan $<
	./$(TARGET)_tsan $(N) 20 > /dev/null && echo "ThreadSanitizer: no data races found"

clean:
	rm -f $(TARGET) $(TARGET)_tsan
