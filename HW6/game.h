#ifndef GAME_H
#define GAME_H

#include <ncursesw/ncurses.h>
#include <inttypes.h>
#include "game_config.h"
#include "snake.h"
#include "food.h"


int getch_with_timeout(int milliseconds);
void initAllSnakes(snake_t snakes[], size_t num_snakes, size_t start_size);
void changeAllDirections(snake_t snakes[], size_t num_snakes, const int32_t key);
void drawAllSnakes(snake_t snakes[], size_t num_snakes);
int checkWallCollision(snake_t* snake, int max_x, int max_y);
int checkSnakeEatSnake(snake_t snakes[], size_t num_snakes);
int checkAllCollisions(snake_t snakes[], size_t num_snakes, int max_x, int max_y);
void playGame(snake_t snakes[]);

#endif