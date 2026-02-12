#include "food.h"
#include <stdlib.h>
#include <time.h>


void initFood(struct food f[], size_t size)
{
    for (size_t i = 0; i < size; i++) {
        f[i].x = 0;
        f[i].y = 0;
        f[i].put_time = 0;
        f[i].point = '#';
        f[i].enable = 0;
    }
}

int isPositionOccupiedBySnake(struct snake_t *head, int x, int y)
{
    if (head->x == x && head->y == y) return 1;
    
    for (size_t i = 0; i < head->tsize; i++) {
        if (head->tail[i].x == x && head->tail[i].y == y) return 1;
    }
    return 0;
}

void repairSeed(struct food f[], size_t nfood, struct snake_t *head)
{
    for (size_t i = 0; i < nfood; i++) {
        if (!f[i].enable) continue;
        
        if (isPositionOccupiedBySnake(head, f[i].x, f[i].y)) {
            f[i].enable = 0;
            continue;
        }
        
        for (size_t j = i + 1; j < nfood; j++) {
            if (f[j].enable && 
                f[j].x == f[i].x && 
                f[j].y == f[i].y) {
                f[j].enable = 0;
            }
        }
    }
}

void refreshFood(struct food f[], size_t size, int max_x, int max_y)
{
    time_t current_time = time(NULL);
    
    for (size_t i = 0; i < size; i++) {
        if (!f[i].enable || 
            (current_time - f[i].put_time) > FOOD_EXPIRE_SECONDS) {
            
            f[i].x = rand() % max_x;
            f[i].y = (rand() % (max_y - MIN_Y)) + MIN_Y;  
            f[i].put_time = current_time;
            f[i].point = '#';
            f[i].enable = 1;
        }
    }
}

int haveEat(struct snake_t *head, struct food f[])
{
    for (size_t i = 0; i < MAX_FOOD_SIZE; i++) {
        if (f[i].enable && 
            head->x == f[i].x && 
            head->y == f[i].y) {
            
            f[i].enable = 0;
            return 1;
        }
    }
    return 0;
}