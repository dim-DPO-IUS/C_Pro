#ifndef FOOD_H
#define FOOD_H

#include <time.h>
#include <inttypes.h>
#include "snake.h"  

// Константы
#define MAX_FOOD_SIZE 20
#define FOOD_EXPIRE_SECONDS 10
#define SEED_NUMBER MAX_FOOD_SIZE

struct food {
    int x;
    int y;
    time_t put_time;
    char point;
    uint8_t enable;
};

void initFood(struct food f[], size_t size);
void refreshFood(struct food f[], size_t size, int max_x, int max_y);
int haveEat(struct snake_t *head, struct food f[]);
void repairSeed(struct food f[], size_t nfood, struct snake_t *head);
int isPositionOccupiedBySnake(struct snake_t *head, int x, int y);

#endif