#include "snake.h"
#include "food.h"  

#ifndef MIN_Y
// #define MIN_Y 2
#endif

void initTail(tail_t t[], size_t size)
{
    tail_t init_t = { 0, 0 };
    for (size_t i = 0; i < size; i++) t[i] = init_t;
}

void initHead(snake_t* head, int x, int y)
{
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
    head->score = 0;
    head->is_alive = 1;
}

void initSnake(snake_t* head, size_t size, int x, int y, int color_pair)
{
    tail_t* tail = (tail_t*)malloc(MAX_TAIL_SIZE * sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head, x, y);
    head->tail = tail;
    head->tsize = size + 1;
    head->color_pair = color_pair;
    head->control_type = -1;
}

void go(snake_t* head)
{
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);

    mvprintw(head->y, head->x, " ");

    switch (head->direction)
    {
    case LEFT: head->x = (head->x <= 0) ? max_x - 1 : head->x - 1; break;
    case RIGHT: head->x = (head->x >= max_x - 1) ? 0 : head->x + 1; break;
    case UP: head->y = (head->y <= MIN_Y) ? max_y - 1 : head->y - 1; break;
    case DOWN: head->y = (head->y >= max_y - 1) ? MIN_Y : head->y + 1; break;
    }
}

void goTail(snake_t* head)
{
    if (!head->is_alive) return;

    mvprintw(head->tail[head->tsize - 1].y, head->tail[head->tsize - 1].x, " ");

    for (size_t i = head->tsize - 1; i > 0; i--)
    {
        head->tail[i] = head->tail[i - 1];
    }

    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

int isCrush(snake_t* snake)
{
    if (!snake || snake->tsize < 4)  // Нет смысла проверять при маленьком хвосте
    {
        return 0;
    }
    
    for (size_t i = 3; i < snake->tsize; i++)  // Начинаем с 4-го сегмента
    {
        if (snake->x == snake->tail[i].x && snake->y == snake->tail[i].y)
        {
            return 1;
        }
    }
    return 0;
}

int checkFood(snake_t* snake, struct food* food)  
{
    if (snake->x == food->x && snake->y == food->y)
    {
        snake->tsize++;
        snake->score += 10;
        return 1;
    }
    return 0;
}

int getDirectionFromKeyForSnake(const snake_t* snake, int32_t key)
{
    if (!snake || snake->control_type == -1) return -1;

    int type = snake->control_type;

    switch (type)
    {
    case 0:
        if (key == KEY_DOWN) return DOWN;
        if (key == KEY_UP) return UP;
        if (key == KEY_RIGHT) return RIGHT;
        if (key == KEY_LEFT) return LEFT;
        break;

    case 1:
        if (key == 's') return DOWN;
        if (key == 'w') return UP;
        if (key == 'd') return RIGHT;
        if (key == 'a') return LEFT;
        break;

    case 2:
        if (key == 'S') return DOWN;
        if (key == 'W') return UP;
        if (key == 'D') return RIGHT;
        if (key == 'A') return LEFT;
        break;

    case 3:
        if (key == 's' || key == 'S') return DOWN;
        if (key == 'w' || key == 'W') return UP;
        if (key == 'd' || key == 'D') return RIGHT;
        if (key == 'a' || key == 'A') return LEFT;

        if (key == 1099 || key == 1067) return DOWN;
        if (key == 1094 || key == 1062) return UP;
        if (key == 1092 || key == 1060) return LEFT;
        if (key == 1074 || key == 1042) return RIGHT;
        break;

    default: return -1;
    }

    return -1;
}

int isValidDirection(int current_dir, int new_dir)
{
    if (current_dir == LEFT && new_dir == RIGHT) return 0;
    if (current_dir == RIGHT && new_dir == LEFT) return 0;
    if (current_dir == UP && new_dir == DOWN) return 0;
    if (current_dir == DOWN && new_dir == UP) return 0;
    return 1;
}

int checkDirection(snake_t* snake, int32_t key)
{
    if (!snake || !snake->is_alive) return 0;

    int new_dir = getDirectionFromKeyForSnake(snake, key);
    if (new_dir == -1) return 0;

    return isValidDirection(snake->direction, new_dir);
}