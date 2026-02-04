# Makefile для активного файла

CC = gcc
CFLAGS = -Wall -Wextra
#~ LDFLAGS = -lncurses
LDFLAGS = -lncursesw

# Активный файл - Geany передает через %f в команде сборки
# В Geany Build команда должна быть: make active FILE="%f"

ifdef FILE
    ACTIVE_FILE = $(FILE)
else
    # Если не передано, берем первый .c файл
    ACTIVE_FILE = $(firstword $(wildcard *.c))
endif

PROGRAM = $(basename $(notdir $(ACTIVE_FILE)))

.PHONY: active run clean

active: $(PROGRAM)

$(PROGRAM): $(ACTIVE_FILE)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(ACTIVE_FILE) $(LDFLAGS)

run: active
	./$(PROGRAM)

clean:
	rm -f $(basename $(wildcard *.c)) *.o
