CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGETS = client server

all: $(TARGETS)

%: %.o
	$(CC) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGETS) *.o

.PHONY: all clean