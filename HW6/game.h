#ifndef GAME_H
#define GAME_H

#include <ncursesw/ncurses.h>
#include <inttypes.h>
#include "snake.h"
#include "food.h"

#define NUM_SNAKES 2
#define CONTROLS 4
#define GAME_SPEED 100

extern struct control_buttons default_controls[CONTROLS];
extern struct food foods[MAX_FOOD_SIZE];

int getch_with_timeout(int milliseconds);
void initAllSnakes(snake_t snakes[], size_t num_snakes, size_t start_size);
void changeAllDirections(snake_t snakes[], size_t num_snakes, const int32_t key);
void drawAllSnakes(snake_t snakes[], size_t num_snakes);
int checkWallCollision(snake_t* snake, int max_x, int max_y);
int checkSnakeEatSnake(snake_t snakes[], size_t num_snakes);
int checkAllCollisions(snake_t snakes[], size_t num_snakes, int max_x, int max_y);
void playGame(snake_t snakes[]);

// Удаляем неиспользуемые объявления функций

#endif