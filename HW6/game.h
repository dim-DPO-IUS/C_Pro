#ifndef GAME_H
#define GAME_H

#include <ncursesw/ncurses.h>
#include <inttypes.h>
#include "snake.h"

// Константы игры
#define NUM_SNAKES 2
#define CONTROLS 4
#define START_TAIL_SIZE 3
#define MAX_FOOD_SIZE 20
#define FOOD_EXPIRE_SECONDS 10
#define GAME_SPEED 100

// Стандартные управления
extern struct control_buttons default_controls[CONTROLS];

// Глобальные переменные игры
// extern clock_t last_frame_time;

// Функции игровой логики
int getch_with_timeout(int milliseconds);
void initAllSnakes(snake_t snakes[], size_t num_snakes, size_t start_size);
void spawnFood(food_t* food, int max_x, int max_y);
void drawFood(food_t* food);
void changeAllDirections(snake_t snakes[], size_t num_snakes, const int32_t key);
void drawAllSnakes(snake_t snakes[], size_t num_snakes);
int checkAllFood(snake_t snakes[], size_t num_snakes, food_t* food);
int checkWallCollision(snake_t* snake, int max_x, int max_y);
int checkSnakeEatSnake(snake_t snakes[], size_t num_snakes);
int checkAllCollisions(snake_t snakes[], size_t num_snakes, int max_x, int max_y);
void playGame(void);

#endif // GAME_H