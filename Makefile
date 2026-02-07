CC = gcc
CFLAGS = -Iinclude -g -Wall
TARGET = program

all: $(TARGET)

$(TARGET): main.c src/*.c
	$(CC) $(CFLAGS) main.c src/*.c -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)

.PHONY: all run clean debug
