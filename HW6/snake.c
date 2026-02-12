#include "snake.h"

// Добавляем недостающую константу MIN_Y (которая используется в go())
#ifndef MIN_Y
#define MIN_Y 2
#endif

// Initialize tail (zero coordinates)
void initTail(tail_t t[], size_t size)
{
    tail_t init_t = { 0, 0 };
    for (size_t i = 0; i < size; i++) t[i] = init_t;
}

// Initialize snake head
void initHead(snake_t* head, int x, int y)
{
    head->x = x;
    head->y = y;
    head->direction = RIGHT; // initial direction right
    head->score = 0;
    head->is_alive = 1;
}

// Initialize whole snake
void initSnake(snake_t* head, size_t size, int x, int y, int color_pair)
{
    tail_t* tail = (tail_t*)malloc(MAX_TAIL_SIZE * sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head, x, y);
    head->tail = tail;
    head->tsize = size + 1; // initial tail
    head->color_pair = color_pair;
    head->control_type = -1; // Инициализируем как "не назначен"
}

// Move snake head and draw
void go(snake_t* head)
{
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x); // terminal sizes

    // Erase previous head position
    mvprintw(head->y, head->x, " ");

    // Move head depending on direction
    switch (head->direction)
    {
    case LEFT: head->x = (head->x <= 0) ? max_x - 1 : head->x - 1; break;
    case RIGHT: head->x = (head->x >= max_x - 1) ? 0 : head->x + 1; break;
    case UP: head->y = (head->y <= MIN_Y) ? max_y - 1 : head->y - 1; break;
    case DOWN: head->y = (head->y >= max_y - 1) ? MIN_Y : head->y + 1; break;
    }
}

// Move snake tail
void goTail(snake_t* head)
{
    if (!head->is_alive) return; // Don't move dead snake's tail

    // Erase last tail segment
    mvprintw(head->tail[head->tsize - 1].y, head->tail[head->tsize - 1].x, " ");

    // Shift all tail segments
    for (size_t i = head->tsize - 1; i > 0; i--)
    {
        head->tail[i] = head->tail[i - 1];
    }

    // First tail segment takes previous head position
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

// Check head collision with own tail
int isCrush(snake_t* snake)
{
    // Проверка для пустого хвоста
    if (!snake || snake->tsize == 0)
    {
        return 0; // Нечего проверять
    }
    // Проверяем столкновение головы с хвостом
    for (size_t i = 0; i < snake->tsize; i++)
    {
        // Пропускаем первые 3 сегмента хвоста (чтобы можно было развернуться)
        if (i < 3) continue;

        // Если голова находится на том же месте, что и сегмент хвоста
        if (snake->x == snake->tail[i].x && snake->y == snake->tail[i].y)
        {
            return 1; // Столкновение!
        }
    }
    return 0; // Нет столкновения
}

// Check if snake ate food
int checkFood(snake_t* snake, food_t* food)
{
    if (snake->x == food->x && snake->y == food->y)
    {
        snake->tsize++; // increase tail
        snake->score += 10; // add points
        // playSound(1); // Food eating sound - эта функция не входит в модуль snake
        return 1; // food eaten
    }
    return 0;
}

// Вспомогательная функция: получить направление из клавиши
// Получить направление из клавиши для конкретной змейки
int getDirectionFromKeyForSnake(const snake_t* snake, int32_t key)
{
    if (!snake || snake->control_type == -1) return -1;

    int type = snake->control_type;

    // Проверяем клавиши в зависимости от типа управления
    switch (type)
    {
    case 0: // Тип 0: Стрелки
        if (key == KEY_DOWN) return DOWN;
        if (key == KEY_UP) return UP;
        if (key == KEY_RIGHT) return RIGHT;
        if (key == KEY_LEFT) return LEFT;
        break;

    case 1: // Тип 1: WASD строчные (только строчные англ)
        if (key == 's') return DOWN;
        if (key == 'w') return UP;
        if (key == 'd') return RIGHT;
        if (key == 'a') return LEFT;
        break;

    case 2: // Тип 2: WASD заглавные (только заглавные англ)
        if (key == 'S') return DOWN;
        if (key == 'W') return UP;
        if (key == 'D') return RIGHT;
        if (key == 'A') return LEFT;
        break;

    case 3: // Тип 3: УНИВЕРСАЛЬНЫЙ WASD (оба регистра + русские)
        // Английские буквы (оба регистра)
        if (key == 's' || key == 'S') return DOWN;
        if (key == 'w' || key == 'W') return UP;
        if (key == 'd' || key == 'D') return RIGHT;
        if (key == 'a' || key == 'A') return LEFT;

        // Русские буквы
        if (key == 1099 || key == 1067) return DOWN; // Ы ы
        if (key == 1094 || key == 1062) return UP; // Ц ц
        if (key == 1092 || key == 1060) return LEFT; // Ф ф
        if (key == 1074 || key == 1042) return RIGHT; // В в
        break;

    default: return -1; // Неизвестный тип
    }

    return -1; // Клавиша не соответствует этому типу управления
}

// Проверка, не является ли новое направление противоположным текущему
int isValidDirection(int current_dir, int new_dir)
{
    // Если змея движется влево, нельзя сразу пойти вправо
    if (current_dir == LEFT && new_dir == RIGHT) return 0;
    // Если змея движется вправо, нельзя сразу пойти влево
    if (current_dir == RIGHT && new_dir == LEFT) return 0;
    // Если змея движется вверх, нельзя сразу пойти вниз
    if (current_dir == UP && new_dir == DOWN) return 0;
    // Если змея движется вниз, нельзя сразу пойти вверх
    if (current_dir == DOWN && new_dir == UP) return 0;
    // Во всех остальных случаях разрешено
    return 1;
}

// Основная функция проверки
int checkDirection(snake_t* snake, int32_t key)
{
    if (!snake || !snake->is_alive) return 0;

    // 1. Получаем направление из клавиши для ЭТОГО типа управления
    int new_dir = getDirectionFromKeyForSnake(snake, key);
    if (new_dir == -1) return 0; // Эта клавиша не для этой змейки

    // 2. Проверяем, не противоположное ли это направление
    return isValidDirection(snake->direction, new_dir);
}