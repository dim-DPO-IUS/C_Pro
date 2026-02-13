#ifndef MENU_H
#define MENU_H

#include <ncursesw/ncurses.h>
#include "game_config.h"

void playSound(int type);
void showWelcomeScreen(void);
void setColor(int objectType);
int startMenu(void);

#endif