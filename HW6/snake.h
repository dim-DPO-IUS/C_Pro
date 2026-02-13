#ifndef SNAKE_H
#define SNAKE_H

#include "game_config.h"
#include <ncursesw/ncurses.h>
#include <wchar.h>
#include <inttypes.h>
#include <stdlib.h>


struct food;

enum { LEFT = 1, UP, RIGHT, DOWN, STOP_GAME = KEY_F(10) };


#define MIN_Y 2

typedef struct tail_t {
    int x;
    int y;
} tail_t;

typedef struct snake_t {
    int x;
    int y;
    int direction;
    size_t tsize;
    struct tail_t* tail;
    struct control_buttons controls;
    int control_type;
    int color_pair;
    int score;
    int is_alive;
} snake_t;

void initTail(tail_t t[], size_t size);
void initHead(snake_t *head, int x, int y);
void initSnake(snake_t *head, size_t size, int x, int y, int color_pair);
void go(snake_t *head);
void goTail(snake_t *head);
int isCrush(snake_t *snake);
int checkFood(snake_t *snake, struct food *food);
int getDirectionFromKeyForSnake(const snake_t* snake, int32_t key);
int checkDirection(snake_t* snake, int32_t key);

#endif