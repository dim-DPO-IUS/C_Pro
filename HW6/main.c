/*
 * 1. Доработать функционал игры "Змейка", реализовать в игре ситуацию, когда змея
 * врезается сама в себя. 2.Выход за границы экрана.
 * 3. * Переписать timeout через clock.
 * */

/*
 *hw-6
 *Tasks
 *1. Реализовать пропущенный код
 *2. Внести изменения в исполняемый файл, чтобы в стандартном режиме змейка не
 * погибала при самопересечении. Для этого необходимо найти вызов функции isCrush() и
 * поменять вызов на нужное нам возвращаемое значение 0
 *3. Добавить цвет для двух змеек и еды.
 *4. Добавить стартовое меню (приветствие, выбор режима, выбор цвета змейки и тд)
 *Написать функцию void startMenu()
 *5. Сделать свои игровые механики
 *
 *
 * Игра "Змейка"
 * Функционал:
 * 1. Движение змейки
 * 2. Поедание еды
 * 3. Проверка столкновений с хвостом и границами
 * 4. Цветная змейка (зелёная)
 * 5. Окончание игры с выводом сообщения Game Over
 */

/*
 * 1. Develop Snake game functionality, implement situation when snake crashes into
 * itself.
 * 2. Screen boundary exit.
 * 3. * Rewrite timeout using clock.
 * */

/*
 * hw-6
 * Tasks
 * 1. Implement missing code
 * 2. Modify executable so that in standard mode snake doesn't die from
 * self-intersection. For this, find isCrush() function call and change it to return
 * value 0
 * 3. Add colors for two snakes and food.
 * 4. Add start menu (greeting, mode selection, snake color selection, etc.)
 * Write function void startMenu()
 * 5. Create your own game mechanics
 *
 *
 * Snake Game
 * Features:
 * 1. Snake movement
 * 2. Eating food
 * 3. Collision detection with tail and boundaries
 * 4. Colored snake (green)
 * 5. Game over with message
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncursesw/ncurses.h>
#include <wchar.h>
#include <ctype.h>
#include <inttypes.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>

#include "snake.h"
#include "menu.h"
#include "game.h"
#include "food.h"

#define _XOPEN_SOURCE_EXTENDED 1

// Убираем дублирующие определения констант

int main()
{
    setlocale(LC_ALL, "");
    
    srand(time(NULL));

    initscr();
    keypad(stdscr, TRUE);
    raw();
    noecho();
    curs_set(FALSE);

    if (!has_colors())
    {
        endwin();
        printf("Your terminal does not support colors\n");
        return 1;
    }

    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_RED, COLOR_BLACK);

    initFood(foods, MAX_FOOD_SIZE);

    showWelcomeScreen();

    while (game_running)
    {
        int menu_result = startMenu();

        if (menu_result == 1)
        {
            snake_t snakes[NUM_SNAKES];
            initAllSnakes(snakes, NUM_SNAKES, START_TAIL_SIZE);
            
            playGame(snakes);
        }
        else if (menu_result == 2)
        {
            initFood(foods, MAX_FOOD_SIZE);
            continue;
        }
        else
        {
            game_running = 0;
        }
    }

    endwin();
    return 0;
}