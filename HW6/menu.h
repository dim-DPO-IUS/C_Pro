#ifndef MENU_H
#define MENU_H

#include <ncursesw/ncurses.h>

extern int game_running;
extern int can_eat_snakes;
extern int sound_enabled;
extern int snake_colors[2];

void playSound(int type);
void showWelcomeScreen(void);
int startMenu(void);

#endif