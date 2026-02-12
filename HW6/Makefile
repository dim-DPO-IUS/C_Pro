CC = gcc
CFLAGS = -D_XOPEN_SOURCE_EXTENDED=1 -Wall -g
LIBS = -lncursesw
TARGET = snake_game
SOURCES = main.c snake.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean