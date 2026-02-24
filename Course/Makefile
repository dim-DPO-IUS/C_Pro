# Makefile для программы вычисления площади
# Целевая программа: area

CC = gcc
# CFLAGS = -Wall -Wextra -O2 -Iinclude -Iinclude/math -Iinclude/geometry -Iinclude/ui
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lm
TARGET = area

# Автоматически находим все .c файлы в src и её подпапках
SRC = main.c $(shell find src -name "*.c")

# Объектные файлы и зависимости
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

all: $(TARGET)

# Линковка
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Компиляция с генерацией зависимостей
%.o: %.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

# Включаем сгенерированные файлы зависимостей
-include $(DEP)

# Очистка
clean:
	rm -f $(TARGET) $(OBJ) $(DEP)
	rm -f *.dat *.gp *.png results.log

# Цели для запуска
run: $(TARGET)
	./$(TARGET)

run-p: $(TARGET)
	./$(TARGET) -p

run-i: $(TARGET)
	./$(TARGET) -i

run-v: $(TARGET)
	./$(TARGET) -v

# Отладка - показать найденные файлы
debug:
	@echo "Найденные исходные файлы:"
	@echo "$(SRC)" | tr ' ' '\n'
	@echo "\nОбъектные файлы:"
	@echo "$(OBJ)" | tr ' ' '\n'

.PHONY: all clean run run-p run-i run-v debug