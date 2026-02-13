#include "game_config.h"
#include "food.h"      // для struct food
#include "snake.h"     // для struct control_buttons

struct food foods[MAX_FOOD_SIZE];
int game_running = 1;
int can_eat_snakes = 1;
int sound_enabled = 1;
int snake_colors[2] = { 1, 6 };
struct control_buttons default_controls[CONTROLS] = {
    { KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT },
    { 's', 'w', 'a', 'd' },
    { 'S', 'W', 'A', 'D' },
    { 's', 'w', 'a', 'd' }
};

// поведение при самопересечении
int self_collision_enabled = 0; // не гибнет