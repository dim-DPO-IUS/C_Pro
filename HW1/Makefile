CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Все исполняемые файлы
TARGETS = program1 program2 program3

all: $(TARGETS)

# Правило для каждого задания
program1: A0.c
	$(CC) $(CFLAGS) -o $@ $^

program2: A1.c
	$(CC) $(CFLAGS) -o $@ $^

#~ program3: A2.c
#~ 	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS) *.o
