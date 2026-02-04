

# Модификации кода Snake Game

## Что реализовано в этом задании

### 1. **Поддержка wide characters через ncursesw (UTF-8)**
- Заменен `#include <ncurses.h>` на `#include <ncursesw/ncurses.h>`
- Добавлена инициализация локализации: `setlocale(LC_ALL, "")` в `main()`
- Обновлена функция `getch_with_timeout()` для работы с `wget_wch()`
- Линковка с `-lncursesw` вместо `-lncurses`
- **Результат:** Корректная работа с русской раскладкой и правильные Unicode коды символов

### 2. **4 типа управления**
```c
#define CONTROLS 4  // Добавлена новая константа

struct control_buttons default_controls[CONTROLS] = {
    {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT},  // Тип 0: стрелки
    {'s', 'w', 'a', 'd'},                      // Тип 1: WASD строчные
    {'S', 'W', 'A', 'D'},                      // Тип 2: WASD заглавные  
    {'s', 'w', 'a', 'd'}                       // Тип 3: Универсальный WASD
};
```

### 3. **Поддержка русской и английской раскладок**
- Добавлено поле `control_type` в структуру `snake_t`
- Функция `getDirectionFromKeyForSnake()` проверяет оба набора кодов:
  ```c
  // Английские + Русские коды Unicode
  case 'w': case 'W': case 1094: case 1062: return UP;    // w/W + ц/Ц
  case 's': case 'S': case 1099: case 1067: return DOWN;  // s/S + ы/Ы
  case 'a': case 'A': case 1092: case 1060: return LEFT;  // a/A + ф/Ф
  case 'd': case 'D': case 1074: case 1042: return RIGHT; // d/D + в/В
  ```
- **Змейка 1** назначен тип 3 (Универсальный WASD)

### 4. **Проверка корректности направления движения**
- Реализована функция `checkDirection(snake_t* snake, int32_t key)` с требуемой сигнатурой
- Функция использует существующую `isValidDirection()` для проверки противоположных направлений
- Запрещены: RIGHT→LEFT, LEFT→RIGHT, UP→DOWN, DOWN→UP

### 5. **Архитектурные изменения**
- Добавлен цикл по типам управления в `changeAllDirections()` как требовалось:
  ```c
  for (int ctrl = 0; ctrl < CONTROLS; ctrl++) {
      // Проверка для каждого типа управления
  }
  ```
- Обновлена функция `initAllSnakes()` для назначения типов управления
- Упрощена логика обработки ввода

## Технические детали реализации

### Функция `checkDirection()`:
```c
int checkDirection(snake_t* snake, int32_t key) {
    if (!snake || !snake->is_alive) return 0;
    
    // 1. Получаем направление из клавиши для ЭТОГО типа управления
    int new_dir = getDirectionFromKeyForSnake(snake, key);
    if (new_dir == -1) return 0; // Эта клавиша не для этой змейки
    
    // 2. Проверяем, не противоположное ли это направление
    return isValidDirection(snake->direction, new_dir);
}
```

### Функция `getch_with_timeout()` с wide chars:
```c
int getch_with_timeout(int milliseconds) {
    // Использует wget_wch() для поддержки Unicode
    wint_t ch;
    int ret = wget_wch(stdscr, &ch);
    
    if (ret == KEY_CODE_YES || ret == OK) {
        return (int)ch;  // Возвращаем Unicode код символа
    }
    return ERR;
}
```

## Результат

1. **Корректная работа русской раскладки** - символы 'ц', 'ы', 'ф', 'в' работают как 'w', 's', 'a', 'd'
2. **Раздельное управление** - каждая змейка реагирует только на свои клавиши
3. **Запрет противоположных направлений** - змейка не может развернуться на 180°
4. **Масштабируемая архитектура** - легко добавить новые змейки и типы управления
5. **Поддержка Unicode** - корректное отображение и обработка символов

## Состояние игры после изменений

- Змейка 0: управляется стрелками (тип 0)
- Змейка 1: управляется WASD (англ. + рус., тип 3)
- Проверка корректности направления работает
- Русская раскладка поддерживается
- Все специальные клавиши (стрелки, F10) работают
- Архитектура готова к расширению
