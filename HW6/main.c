
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
// #include <ncurses/ncurses.h>
//~ #include <ncurses.h>
//-----
#include <ncursesw/ncurses.h> // Широкие символы
#include <wchar.h> // Широкие символы
//-----
#include <ctype.h> // for toupper()
#include <inttypes.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
//-----
#include "snake.h"
#include "menu.h"
#include "game.h"
#include "food.h"
//-----

// нужно для использования функции wget_wch
#define _XOPEN_SOURCE_EXTENDED 1

#define NUM_SNAKES 2 // количество змеек
#define CONTROLS 4 // количество типов управления (стрелки, WASD, wasd ...)

// Для управления временем через clock()
#define GAME_SPEED 100 // 100ms между кадрами (10 FPS)

struct food foods[MAX_FOOD_SIZE];

// ======================= MAIN FUNCTION ==========================
int main()
{
    // Для правильных кодов клавиш русской раскладки:
    // Ы ы: 1099, 1067
    // Ц ц: 1094, 1062
    // Ф ф: 1092, 1060
    // В в: 1074, 1042

    // Без использования этого подхода коды клавиш будут 3-х значными
    // а по условию задачи они должны быть 4-х значными

    setlocale(LC_ALL, ""); // Включить локализацию
    //------------------------------------------------

    srand(time(NULL)); // random number generator

    // Initialize curses
    initscr();
    keypad(stdscr, TRUE);
    raw();
    noecho();
    curs_set(FALSE);

    // Check color support
    if (!has_colors())
    {
        endwin();
        printf("Your terminal does not support colors\n");
        return 1;
    }

    // Enable color and set color pairs
    start_color();
    // Use standard ncurses color constants
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_RED, COLOR_BLACK); // Food

    //
    initFood(foods, MAX_FOOD_SIZE);

    // Show welcome screen on startup
    showWelcomeScreen();

    // Main program loop
    while (game_running)
    {
        int menu_result = startMenu();

        if (menu_result == 1)
        {
            // Start game
            playGame();
        }
        else if (menu_result == 2)
        {
            // Restart (do nothing, just continue loop)
            continue;
        }
        else
        {
            // Exit
            game_running = 0;
        }
    }

    endwin(); // end curses mode
    return 0;
}
