
CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread
TARGET = q6

all:
	$(CC) $(CFLAGS) hw1-scheduling.c -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
