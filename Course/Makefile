# Makefile для программы вычисления площади
# Целевая программа: area

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lm
TARGET = area

# Автоматически находим все .c файлы в src и её подпапках
SRC = main.c $(shell find src -name "*.c")

# Объектные файлы и зависимости
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

all: $(TARGET)
	@echo "Сборка завершена. Исполняемый файл: $(TARGET)"

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
	@echo "Очистка завершена"

# Дополнительные цели для запуска
run: $(TARGET)
	./$(TARGET)

run-h: $(TARGET)
	./$(TARGET) -h

run-a: $(TARGET)
	./$(TARGET) -a
	
run-p: $(TARGET)
	./$(TARGET) -p

run-i: $(TARGET)
	./$(TARGET) -i

run-v: $(TARGET)
	./$(TARGET) -v

run-t: $(TARGET)
	./$(TARGET) -t 1e-6 1e-4

.PHONY: all clean run run-v run-t