# ============================================
# УНИВЕРСАЛЬНЫЙ MAKEFILE VSCode (в Geany не проверял) для учебных заданий
# ВНИМАНИЕ!!! Положи этот файл в HW1/, HW2/, HW3/ и т.д.
# ВНИМАНИЕ!!! Этот файл зависим от tasks.json и launch.json
# ============================================

# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g  # -g для отладки
LDFLAGS =  # Библиотеки добавляй здесь

# Настройки для разных типов заданий (раскомментируй нужное)
# LDFLAGS = -lm            # для математических задач
# LDFLAGS = -lncurses      # для работы с терминалом
# LDFLAGS = -lncursesw     # для работы с терминалом в UTF (или Unicode? - не понял пока)
# LDFLAGS = -lpthread      # для многопоточности
# LDFLAGS = -lsocket -lnsl # для сетевых программ

# Автоматическое определение активного файла
ifdef FILE
    # Используем файл, переданный извне (через make FILE=...)
    ACTIVE_FILE = $(FILE)
else ifdef VSCODE_FILE
    # Для VSCode (переменная из tasks.json)
    ACTIVE_FILE = $(notdir $(VSCODE_FILE))
else
    # Берем первый .c файл в текущей папке
    C_FILES = $(wildcard *.c)
    ACTIVE_FILE = $(firstword $(C_FILES))
endif

# Имя программы (без расширения .c)
PROGRAM = $(basename $(ACTIVE_FILE))

# ============================================
# ЦЕЛИ (TARGETS)
# ============================================

.PHONY: active run clean all help debug

# Цель по умолчанию - сборка активного файла
default: active

# Основная цель - сборка текущей программы
active: $(PROGRAM)
	@echo "Собрано: $(PROGRAM)"

# Сборка конкретной программы
$(PROGRAM): $(ACTIVE_FILE)
	@echo "Компиляция: $(ACTIVE_FILE)"
	$(CC) $(CFLAGS) -o $(PROGRAM) $(ACTIVE_FILE) $(LDFLAGS)

# Запуск программы
run: active
	@echo "Запуск: ./$(PROGRAM)"
	@echo "========================================"
	./$(PROGRAM)

# Сборка всех программ в текущей папке
all: $(patsubst %.c, %, $(wildcard *.c))
	@echo "Собраны все программы в $(shell pwd)"

# Сборка конкретного файла по имени (без .c)
%: %.c
	@echo "Компиляция: $<"
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Очистка: удаляет ВСЕ исполняемые файлы и объектные файлы
clean:
	@echo "Очистка в $(shell pwd)"
	rm -f $(basename $(wildcard *.c))  # удаляем все исполняемые файлы
	rm -f *.o *.out core  # удаляем объектные файлы и core дампы
	@echo "Очищено"

# Сборка с отладочной информацией
debug: CFLAGS += -DDEBUG -O0  # добавляем отладочные флаги
debug: clean active
	@echo "Собрано с отладочной информацией: $(PROGRAM)"

# Справка
help:
	@echo "========================================"
	@echo "ИСПОЛЬЗОВАНИЕ MAKEFILE в папке: $(shell basename $(shell pwd))"
	@echo "========================================"
	@echo ""
	@echo "Основные команды:"
	@echo "  make              - собрать активный файл"
	@echo "  make active       - собрать активный файл"
	@echo "  make run          - собрать и запустить"
	@echo "  make A0           - собрать конкретный файл A0.c"
	@echo "  make all          - собрать ВСЕ .c файлы в папке"
	@echo "  make clean        - удалить все исполняемые файлы"
	@echo "  make debug        - собрать с отладочной информацией"
	@echo "  make help         - показать эту справку"
	@echo ""
	@echo "Внешние команды (из корневой папки):"
	@echo "  make -C HW1 FILE=A0.c  - собрать A0.c из HW1"
	@echo "  make -C HW2 run        - собрать и запустить активный в HW2"
	@echo ""
	@echo "Доступные файлы:"
	@for f in *.c; do \
		echo "  - $$f"; \
	done
	@echo ""
	@echo "========================================"