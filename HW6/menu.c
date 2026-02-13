#include "game_config.h"
#include "menu.h"
#include "snake.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// playSound() 
void playSound(int type)
{
    if (!sound_enabled) return;
    switch (type)
    {
    case 1:
        printf("\a");
        fflush(stdout);
        break;
    case 2:
        for (int i = 0; i < 3; i++)
        {
            printf("\a");
            fflush(stdout);
            usleep(100000);
        }
        break;
    case 3:
        for (int i = 0; i < 5; i++)
        {
            printf("\a");
            fflush(stdout);
            usleep(50000);
        }
        break;
    case 4:
        for (int i = 0; i < 2; i++)
        {
            printf("\a");
            fflush(stdout);
            usleep(200000);
        }
        break;
    }
}

// showWelcomeScreen() 
void showWelcomeScreen(void)
{
    clear();
    attron(A_BOLD);
    mvprintw(2, 15, "=== DOUBLE SNAKE GAME ===");
    attroff(A_BOLD);
    mvprintw(4, 5, "Welcome to 'Double Snake' game!");
    mvprintw(5, 5, "Control two snakes simultaneously and collect food.");
    mvprintw(7, 5, "MAIN MENU FUNCTIONS:");
    mvprintw(8, 8, "• Color selection for each snake");
    mvprintw(9, 8, "• Control configuration");
    mvprintw(10, 8, "• Start game with selected settings");
    mvprintw(11, 8, "• Restart menu");
    mvprintw(12, 8, "• Sound on/off");
    mvprintw(13, 8, "• Exit game");
    mvprintw(15, 5, "IN-GAME CONTROLS:");
    mvprintw(16, 8, "Snake 1: Arrows ← ↑ → ↓");
    mvprintw(17, 8, "Snake 2: WASD (any layout)");
    mvprintw(18, 8, "Exit: F10");
    mvprintw(20, 5, "GAME FEATURES:");
    mvprintw(21, 8, "• Two independent snakes");
    mvprintw(22, 8, "• No death from self-intersection");
    mvprintw(23, 8, "• Teleportation through boundaries");
    mvprintw(24, 8, "• Colorful graphics");
    mvprintw(25, 8, "• Sound effects");
    mvprintw(26, 8, "• You can eat another snake if you catch it!");
    mvprintw(27, 8, "• +50 points for eating snake, +20 for tail");
    mvprintw(29, 5, "MENU CONTROLS:");
    mvprintw(30, 8, "Arrow keys ↑↓ - Navigate options");
    mvprintw(31, 8, "Arrow keys ←→ - Select color");
    mvprintw(32, 8, "TAB - Switch between snakes");
    mvprintw(33, 8, "ENTER - Select option");
    mvprintw(34, 8, "M - Toggle sound");
    mvprintw(35, 8, "Q or F10 - Exit");
    mvprintw(37, 5, "DEVELOPMENT:");
    mvprintw(38, 8, "Game created as educational project");
    mvprintw(39, 8, "using ncurses library");
    attron(A_BOLD);
    mvprintw(41, 15, "Press any key to continue...");
    attroff(A_BOLD);
    refresh();
    getch();
}

// startMenu() 
int startMenu(void)
{
    int selected_snake = 0;
    int selected_option = 0;
    int color_index[2] = { 0, 4 };
    int available_colors[] = { 1, 2, 4, 3, 5, 6, 7 };
    char* color_names[] = { "Green", "Red", "Blue", "Yellow", "Magenta", "Cyan", "White" };
    int num_colors = 7;
    int key;

    while (1)
    {
        clear();
        attron(A_BOLD);
        mvprintw(2, 10, "=== DOUBLE SNAKE GAME ===");
        attroff(A_BOLD);
        mvprintw(3, 5, "Sound: %s", sound_enabled ? "ON" : "OFF");
        mvprintw(5, 5, "Select snake for color configuration:");
        
        for (int i = 0; i < 2; i++)
        {
            if (i == selected_snake) attron(A_REVERSE);
            mvprintw(7, 5 + i * 20, "Snake %d", i + 1);
            if (i == selected_snake) attroff(A_REVERSE);
        }

        mvprintw(9, 5, "Color for snake %d:", selected_snake + 1);
        for (int i = 0; i < num_colors; i++)
        {
            attron(COLOR_PAIR(available_colors[i]));
            if (color_index[selected_snake] == i) attron(A_REVERSE);
            mvprintw(11, 5 + i * 12, "[%s]", color_names[i]);
            if (color_index[selected_snake] == i) attroff(A_REVERSE);
            attroff(COLOR_PAIR(available_colors[i]));
        }

        mvprintw(13, 5, "Current controls:");
        mvprintw(14, 8, "Snake 1: Arrow keys");
        mvprintw(15, 8, "Snake 2: WASD keys");
        mvprintw(16, 5, "New mechanic:");
        mvprintw(17, 8, "You can eat another snake! +50 points");
        mvprintw(19, 5, "Options:");
        
        const char* options[] = { "Start Game", "Restart", "Sound ON/OFF", "Exit" };
        for (int i = 0; i < 4; i++)
        {
            if (i == selected_option) attron(A_REVERSE);
            mvprintw(21 + i, 8, "%s", options[i]);
            if (i == selected_option) attroff(A_REVERSE);
        }

        mvprintw(26, 5, "Menu controls:");
        mvprintw(27, 8, "Arrow keys ↑↓ - navigation, ENTER - select, TAB - switch snakes");
        mvprintw(28, 8, "M - toggle sound");
        refresh();

        key = getch();
        switch (key)
        {
        case KEY_UP: selected_option = (selected_option - 1 + 4) % 4; break;
        case KEY_DOWN: selected_option = (selected_option + 1) % 4; break;
        case KEY_LEFT:
            if (selected_option == 0)
                color_index[selected_snake] = (color_index[selected_snake] - 1 + num_colors) % num_colors;
            break;
        case KEY_RIGHT:
            if (selected_option == 0)
                color_index[selected_snake] = (color_index[selected_snake] + 1) % num_colors;
            break;
        case '\t': selected_snake = (selected_snake + 1) % 2; break;
        case 'm': case 'M':
            sound_enabled = !sound_enabled;
            if (sound_enabled) playSound(4);
            break;
        case '\n': case '\r':
            for (int i = 0; i < 2; i++) snake_colors[i] = available_colors[color_index[i]];
            if (selected_option == 0) { playSound(4); return 1; }
            if (selected_option == 1) return 2;
            if (selected_option == 2) { sound_enabled = !sound_enabled; if (sound_enabled) playSound(4); continue; }
            if (selected_option == 3) return 0;
            break;
        case 'q': case 'Q': return 0;
        case STOP_GAME: return 0;
        }
    }
}