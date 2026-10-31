
CC = gcc
CFLAGS = -Wall -Wextra
TARGET = q1

all:
	$(CC) $(CFLAGS) q1.c -o $(TARGET)

run: all
	./$(TARGET) | wc -l

clean:
	rm -f $(TARGET)
