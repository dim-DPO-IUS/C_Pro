#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <ncursesw/ncurses.h>
#include <time.h>
#include <inttypes.h>

// ==================== КОНСТАНТЫ ====================
#define MIN_Y 2
#define MAX_TAIL_SIZE 100
#define MAX_FOOD_SIZE 20
#define START_TAIL_SIZE 3
#define FOOD_EXPIRE_SECONDS 10
#define GAME_SPEED 100
#define CONTROLS 4
#define NUM_SNAKES 2

// ==================== СТРУКТУРЫ ====================
struct food {
    int x;
    int y;
    time_t put_time;
    char point;
    uint8_t enable;
};

struct control_buttons {
    int down;
    int up;
    int left;
    int right;
};

// ==================== ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ====================
extern struct food foods[MAX_FOOD_SIZE];
extern int game_running;
extern int can_eat_snakes;
extern int sound_enabled;
extern int snake_colors[2];
extern struct control_buttons default_controls[CONTROLS];
// поведение при самопересечении
extern int self_collision_enabled;  // 1 - гибнет, 0 - не гибнет

#endif