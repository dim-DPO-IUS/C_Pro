#ifndef FOOD_H
#define FOOD_H

#include "game_config.h"
#include <time.h>
#include <inttypes.h>
#include "snake.h"


void initFood(struct food f[], size_t size);
void refreshFood(struct food f[], size_t size, int max_x, int max_y);
int haveEat(struct snake_t *head, struct food f[]);
void repairSeed(struct food f[], size_t nfood, struct snake_t *head);
int isPositionOccupiedBySnake(struct snake_t *head, int x, int y);
void drawFood(struct food *food);

#endif