#include "game.h"
#include "snake.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

// Standard control keys
struct control_buttons default_controls[CONTROLS] = {
    // Тип 0: Стрелки
    { KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT },

    // Тип 1: WASD строчные (только строчные англ)
    { 's', 'w', 'a', 'd' },

    // Тип 2: WASD заглавные (только заглавные англ)
    { 'S', 'W', 'A', 'D' },

    // Тип 3: УНИВЕРСАЛЬНЫЙ WASD (оба регистра + русские)
    { 's', 'w', 'a', 'd' } // Базовые - строчные, но будем проверять все варианты
};

struct food foods[MAX_FOOD_SIZE];

// Реализация timeout через clock
int getch_with_timeout(int milliseconds)
{
    if (milliseconds <= 0)
    {
        nodelay(stdscr, milliseconds == 0);
        wint_t ch;
        int ret = wget_wch(stdscr, &ch);
        nodelay(stdscr, FALSE);

        if (ret == KEY_CODE_YES)
        {
            return (int)ch;
        }
        else if (ret == OK)
        {
            return (int)ch;
        }
        return ERR;
    }

    clock_t start = clock();
    long long timeout_ticks = (long long)milliseconds * CLOCKS_PER_SEC / 1000;

    nodelay(stdscr, TRUE);

    while (1)
    {
        wint_t ch;
        int ret = wget_wch(stdscr, &ch);

        if (ret == KEY_CODE_YES || ret == OK)
        {
            nodelay(stdscr, FALSE);
            return (int)ch;
        }

        long long elapsed = (long long)(clock() - start);
        if (elapsed >= timeout_ticks)
        {
            nodelay(stdscr, FALSE);
            return ERR;
        }
    }
}

// Initialize all snakes
void initAllSnakes(snake_t snakes[], size_t num_snakes, size_t start_size)
{
    for (size_t i = 0; i < num_snakes; i++)
    {
        int start_x = 10 + i * 20;
        int start_y = 10 + i * 5;
        int color_idx = i % 7;

        initSnake(&snakes[i], start_size, start_x, start_y, snake_colors[color_idx]);

        if (i == 0)
        {
            snakes[i].control_type = 0;
            snakes[i].controls = default_controls[0];
        }
        else if (i == 1)
        {
            snakes[i].control_type = 3;
            snakes[i].controls = default_controls[3];
        }
    }
}

// Generate food at random position
void spawnFood(food_t* food, int max_x, int max_y)
{
    food->x = rand() % max_x;
    food->y = (rand() % (max_y - MIN_Y)) + MIN_Y;
    food->symbol = '#';
}

// Draw food on screen
void drawFood(food_t* food)
{
    attron(COLOR_PAIR(8));
    mvprintw(food->y, food->x, "%c", food->symbol);
    attroff(COLOR_PAIR(8));
}

// Change direction for all snakes
void changeAllDirections(snake_t snakes[], size_t num_snakes, const int32_t key)
{
    for (size_t i = 0; i < num_snakes; i++)
    {
        if (!snakes[i].is_alive) continue;

        if (checkDirection(&snakes[i], key))
        {
            int new_dir = getDirectionFromKeyForSnake(&snakes[i], key);
            if (new_dir != -1)
            {
                snakes[i].direction = new_dir;
            }
        }
    }
}

// Draw all snakes with different colors
void drawAllSnakes(snake_t snakes[], size_t num_snakes)
{
    for (size_t i = 0; i < num_snakes; i++)
    {
        if (!snakes[i].is_alive) continue;

        attron(COLOR_PAIR(snakes[i].color_pair));

        mvprintw(snakes[i].y, snakes[i].x, "@");

        for (size_t j = 0; j < snakes[i].tsize; j++)
        {
            if (snakes[i].tail[j].y || snakes[i].tail[j].x)
            {
                mvprintw(snakes[i].tail[j].y, snakes[i].tail[j].x, "*");
            }
        }

        attroff(COLOR_PAIR(snakes[i].color_pair));
    }
}

// Check food eating for all snakes
int checkAllFood(snake_t snakes[], size_t num_snakes, food_t* food)
{
    for (size_t i = 0; i < num_snakes; i++)
    {
        if (!snakes[i].is_alive) continue;
        if (checkFood(&snakes[i], food))
        {
            return 1;
        }
    }
    return 0;
}

// Check screen boundary exit
int checkWallCollision(snake_t* snake, int max_x, int max_y)
{
    if (snake->x < 0 || snake->x >= max_x || snake->y < MIN_Y || snake->y >= max_y)
    {
        return 1;
    }
    return 0;
}

// Check if one snake can eat another
int checkSnakeEatSnake(snake_t snakes[], size_t num_snakes)
{
    int snake_eaten = 0;

    for (size_t i = 0; i < num_snakes; i++)
    {
        if (!snakes[i].is_alive) continue;

        for (size_t j = 0; j < num_snakes; j++)
        {
            if (i == j || !snakes[j].is_alive) continue;

            if (snakes[i].x == snakes[j].x && snakes[i].y == snakes[j].y)
            {
                snakes[j].is_alive = 0;
                snakes[i].score += 50;
                snake_eaten = 1;
                playSound(2);

                int max_x = 0, max_y = 0;
                getmaxyx(stdscr, max_y, max_x);
                mvprintw(3, 0, "Snake %zd ate snake %zd! +50 points!", i + 1, j + 1);
            }

            for (size_t k = 0; k < snakes[j].tsize; k++)
            {
                if (snakes[i].x == snakes[j].tail[k].x && snakes[i].y == snakes[j].tail[k].y)
                {
                    snakes[j].tsize = k;
                    snakes[i].score += 20;
                    snake_eaten = 1;
                    playSound(1);

                    int max_x = 0, max_y = 0;
                    getmaxyx(stdscr, max_y, max_x);
                    mvprintw(4, 0, "Snake %zd bit tail of snake %zd! +20 points!",
                        i + 1, j + 1);
                    break;
                }
            }
        }
    }

    return snake_eaten;
}

// Check all collisions for all snakes
int checkAllCollisions(snake_t snakes[], size_t num_snakes, int max_x, int max_y)
{
    int alive_count = 0;

    for (size_t i = 0; i < num_snakes; i++)
    {
        if (!snakes[i].is_alive) continue;
        alive_count++;

        if (checkWallCollision(&snakes[i], max_x, max_y))
        {
            snakes[i].is_alive = 0;
            playSound(3);
            mvprintw(2, 0, "Snake %zd crashed into wall!              ", i + 1);
        }

        if (isCrush(&snakes[i]))
        {
            // This code never executes as isCrush always returns 0
            snakes[i].is_alive = 0;
        }
    }

    if (can_eat_snakes)
    {
        checkSnakeEatSnake(snakes, num_snakes);
    }

    return (alive_count <= 1);
}

// Main game function
//================================================================
void playGame(void)
{
    // ==================== ИНИЦИАЛИЗАЦИЯ ====================
    snake_t snakes[NUM_SNAKES];
    initAllSnakes(snakes, NUM_SNAKES, START_TAIL_SIZE);

    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);

    // Инициализация множества еды
    struct food foods[MAX_FOOD_SIZE];
    initFood(foods, MAX_FOOD_SIZE);
    refreshFood(foods, MAX_FOOD_SIZE, max_x, max_y);

    int key_pressed = 0;
    int game_over = 0;
    clock_t game_start_time = clock();

    if (sound_enabled) playSound(4);

    // ==================== ИГРОВОЙ ЦИКЛ ====================
    while (!game_over && game_running)
    {
        key_pressed = getch_with_timeout(100);

        // Выход из игры
        if (key_pressed == STOP_GAME) break;

        // Переключение звука
        if (key_pressed == 'm' || key_pressed == 'M')
        {
            sound_enabled = !sound_enabled;
            mvprintw(2, 0, "Sound %s", sound_enabled ? "ON" : "OFF");
        }

        // Смена направления для всех змеек
        if (key_pressed != ERR)
        {
            changeAllDirections(snakes, NUM_SNAKES, key_pressed);
        }

        // Движение всех змеек
        for (size_t i = 0; i < NUM_SNAKES; i++)
        {
            if (snakes[i].is_alive)
            {
                go(&snakes[i]);
                goTail(&snakes[i]);
            }
        }

        // ============ ПРОВЕРКА ПОЕДАНИЯ ЕДЫ ============
        for (size_t i = 0; i < NUM_SNAKES; i++)
        {
            if (!snakes[i].is_alive) continue;
            
            if (haveEat(&snakes[i], foods))
            {
                snakes[i].tsize++;      // Увеличиваем хвост
                snakes[i].score += 10;  // Добавляем очки
                playSound(1);           // Звук еды
            }
        }

        // Обновление еды (спавн новой, проверка срока годности)
        refreshFood(foods, MAX_FOOD_SIZE, max_x, max_y);

        // Проверка корректности еды (не на змеях)
        for (size_t i = 0; i < NUM_SNAKES; i++)
        {
            if (snakes[i].is_alive)
            {
                repairSeed(foods, MAX_FOOD_SIZE, &snakes[i]);
            }
        }

        // ============ ОТРИСОВКА ============
        clear();

        // Интерфейс: очки
        mvprintw(0, 0, "P1: %d | P2: %d | M-sound | F10-exit", 
                 snakes[0].score, snakes[1].score);

        // Время игры
        clock_t current_time = clock();
        long game_ticks = current_time - game_start_time;
        int seconds = (game_ticks / CLOCKS_PER_SEC) % 60;
        int minutes = game_ticks / CLOCKS_PER_SEC / 60;
        
        // Счетчик еды
        int active_food = 0;
        for (int i = 0; i < MAX_FOOD_SIZE; i++) {
            if (foods[i].enable) active_food++;
        }
        mvprintw(1, 0, "Time: %02d:%02d | Food: %d/%d", 
                 minutes, seconds, active_food, MAX_FOOD_SIZE);

        // Отрисовка змеек
        drawAllSnakes(snakes, NUM_SNAKES);

        // Отрисовка всей еды
        for (int i = 0; i < MAX_FOOD_SIZE; i++)
        {
            if (foods[i].enable)
            {
                attron(COLOR_PAIR(8));  // Красный цвет для еды
                mvprintw(foods[i].y, foods[i].x, "%c", foods[i].point);
                attroff(COLOR_PAIR(8));
            }
        }

        // Проверка столкновений
        if (checkAllCollisions(snakes, NUM_SNAKES, max_x, max_y))
        {
            game_over = 1;
            playSound(3);
        }

        refresh();
    }

    // ==================== GAME OVER ====================
    if (game_over)
    {
        clear();
        
        // Определение победителя
        int winner = -1;
        int max_score = -1;

        for (size_t i = 0; i < NUM_SNAKES; i++)
        {
            if (snakes[i].is_alive)
            {
                winner = i;
            }
            if (snakes[i].score > max_score)
            {
                max_score = snakes[i].score;
                winner = i;
            }
        }

        // Вывод результата
        if (winner != -1)
        {
            mvprintw(max_y / 2, (max_x - 20) / 2, "Snake %d wins!", winner + 1);
            mvprintw(max_y / 2 + 1, (max_x - 20) / 2, "Score: %d points",
                     snakes[winner].score);
        }
        else
        {
            mvprintw(max_y / 2, (max_x - 9) / 2, "Draw!");
        }

        mvprintw(max_y / 2 + 2, (max_x - 40) / 2, 
                 "Press any key to return to menu...");
        refresh();
        getch();
    }

    // ==================== ОЧИСТКА ПАМЯТИ ====================
    for (size_t i = 0; i < NUM_SNAKES; i++)
    {
        free(snakes[i].tail);
    }

    nodelay(stdscr, FALSE);
}

//================================================================
// void playGame(void)
// {
//     snake_t snakes[NUM_SNAKES];
//     initAllSnakes(snakes, NUM_SNAKES, START_TAIL_SIZE);

//     int max_x = 0, max_y = 0;
//     getmaxyx(stdscr, max_y, max_x);

//     food_t food;
//     spawnFood(&food, max_x, max_y);

//     int key_pressed = 0;
//     int game_over = 0;

//     clock_t game_start_time = clock();

//     if (sound_enabled)
//     {
//         playSound(4);
//     }

//     while (!game_over && game_running)
//     {
//         key_pressed = getch_with_timeout(100);

//         if (key_pressed == STOP_GAME)
//         {
//             break;
//         }

//         if (key_pressed == 'm' || key_pressed == 'M')
//         {
//             sound_enabled = !sound_enabled;
//             mvprintw(2, 0, "Sound %s", sound_enabled ? "ON" : "OFF");
//         }

//         if (key_pressed != ERR)
//         {
//             changeAllDirections(snakes, NUM_SNAKES, key_pressed);
//         }

//         for (size_t i = 0; i < NUM_SNAKES; i++)
//         {
//             if (snakes[i].is_alive)
//             {
//                 go(&snakes[i]);
//                 goTail(&snakes[i]);
//             }
//         }

//         clear();
//         mvprintw(0, 0, "P1: %d | P2: %d | M-sound | F10-exit", snakes[0].score,
//             snakes[1].score);

//         clock_t current_time = clock();
//         long game_ticks = current_time - game_start_time;
//         int seconds = (game_ticks / CLOCKS_PER_SEC) % 60;
//         int minutes = game_ticks / CLOCKS_PER_SEC / 60;
//         mvprintw(1, 0, "Time: %02d:%02d (timeout via clock)", minutes, seconds);

//         drawAllSnakes(snakes, NUM_SNAKES);
//         drawFood(&food);

//         if (checkAllFood(snakes, NUM_SNAKES, &food))
//         {
//             spawnFood(&food, max_x, max_y);
//         }

//         if (checkAllCollisions(snakes, NUM_SNAKES, max_x, max_y))
//         {
//             game_over = 1;
//             playSound(3);
//         }

//         refresh();
//     }

//     if (game_over)
//     {
//         clear();
//         int winner = -1;
//         int max_score = -1;

//         for (size_t i = 0; i < NUM_SNAKES; i++)
//         {
//             if (snakes[i].is_alive)
//             {
//                 winner = i;
//             }
//             if (snakes[i].score > max_score)
//             {
//                 max_score = snakes[i].score;
//                 winner = i;
//             }
//         }

//         if (winner != -1)
//         {
//             mvprintw(max_y / 2, (max_x - 20) / 2, "Snake %d wins!", winner + 1);
//             mvprintw(max_y / 2 + 1, (max_x - 20) / 2, "Score: %d points",
//                 snakes[winner].score);
//         }
//         else
//         {
//             mvprintw(max_y / 2, (max_x - 9) / 2, "Draw!");
//         }

//         mvprintw(max_y / 2 + 2, (max_x - 40) / 2, "Press any key to return to menu...");
//         refresh();
//         getch();
//     }

//     for (size_t i = 0; i < NUM_SNAKES; i++)
//     {
//         free(snakes[i].tail);
//     }

//     nodelay(stdscr, FALSE);
// }