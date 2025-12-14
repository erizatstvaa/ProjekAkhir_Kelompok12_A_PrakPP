#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_SNAKE 100

typedef struct {
    int x, y;
} Point;

Point snake[MAX_SNAKE];
int snake_len = 3;
Point food;
int score = 0;
int dx = 1, dy = 0;
int game_over = 0;

int is_wall(int x, int y) {
    if (x <= 0 || x >= 49 || y <= 0 || y >= 20) return 1;
    
    // Kotak pojok kiri atas
    if (x >= 3 && x <= 5 && y >= 3 && y <= 5) return 1;
    // Kotak pojok kanan atas
    if (x >= 44 && x <= 46 && y >= 3 && y <= 5) return 1;
    // Kotak pojok kiri bawah
    if (x >= 3 && x <= 5 && y >= 15 && y <= 17) return 1;
    // Kotak pojok kanan bawah
    if (x >= 44 && x <= 46 && y >= 15 && y <= 17) return 1;
    
    // Garis horizontal
    if (y == 10 && x >= 8 && x <= 15) return 1;
    if (y == 10 && x >= 34 && x <= 41) return 1;
    
    // Kotak tengah
    if (x >= 22 && x <= 27 && y >= 8 && y <= 9) return 1;
    
    return 0;
}

void spawn_food() {
    int valid = 0;
    int i;
    
    while (!valid) {
        food.x = (rand() % 47) + 1;
        food.y = (rand() % 18) + 1;
        
        valid = 1;
        
        if (is_wall(food.x, food.y)) {
            valid = 0;
            continue;
        }
        
        for (i = 0; i < snake_len; i++) {
            if (snake[i].x == food.x && snake[i].y == food.y) {
                valid = 0;
                break;
            }
        }
    }
}

void reset_game() {
    snake[0].x = 25;
    snake[0].y = 12;
    snake[1].x = 24;
    snake[1].y = 12;
    snake[2].x = 23;
    snake[2].y = 12;
    snake_len = 3;
    score = 0;
    dx = 1;
    dy = 0;
    game_over = 0;
    spawn_food();
}

int show_menu() {
    int selected = 0;
    int ch;
    char *menu[] = {"START GAME", "HOW TO PLAY", "EXIT"};
    int menu_size = 3;
    
    while (1) {
        erase();
        
        attron(COLOR_PAIR(5) | A_BOLD);
        mvprintw(3, 15, "##################################");
        mvprintw(4, 15, "#                                #");
        mvprintw(5, 15, "#         SNAKE GAME             #");
        mvprintw(6, 15, "#        ~ Classic ~             #");
        mvprintw(7, 15, "#                                #");
        mvprintw(8, 15, "##################################");
        attroff(COLOR_PAIR(5) | A_BOLD);
        
        attron(COLOR_PAIR(1));
        mvprintw(10, 22, "    ____");
        mvprintw(11, 22, "   / o o\\");
        mvprintw(12, 22, "  (   >  )");
        mvprintw(13, 22, "   \\_~_/~oooo");
        attroff(COLOR_PAIR(1));
        
        // Menu options
        int i;
        for (i = 0; i < menu_size; i++) {
            if (i == selected) {
                attron(COLOR_PAIR(2) | A_BOLD | A_REVERSE);
                mvprintw(16 + i * 2, 23, "  %s  ", menu[i]);
                attroff(COLOR_PAIR(2) | A_BOLD | A_REVERSE);
            } else {
                attron(COLOR_PAIR(4));
                mvprintw(16 + i * 2, 23, "  %s  ", menu[i]);
                attroff(COLOR_PAIR(4));
            }
        }
        
        // Controls info
        attron(COLOR_PAIR(3));
        mvprintw(23, 12, "Use UP/DOWN arrow keys or W/S to navigate");
        mvprintw(24, 17, "Press ENTER to select");
        attroff(COLOR_PAIR(3));
        
        refresh();
        
        ch = getch();
        
        if (ch == KEY_UP || ch == 'w') {
            selected = (selected - 1 + menu_size) % menu_size;
        }
        else if (ch == KEY_DOWN || ch == 's') {
            selected = (selected + 1) % menu_size;
        }
        else if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) {
            if (selected == 0) {
                return 1; // Start game
            }
            else if (selected == 1) {
                // Info how to play
                erase();
                attron(COLOR_PAIR(2) | A_BOLD);
                mvprintw(3, 18, "===== HOW TO PLAY =====");
                attroff(COLOR_PAIR(2) | A_BOLD);
                
                attron(COLOR_PAIR(4));
                mvprintw(6, 10, "@ = Snake Head (You!)");
                mvprintw(7, 10, "o = Snake Body");
                mvprintw(8, 10, "* = Food");
                mvprintw(9, 10, "# = Wall/Obstacle");
                
                mvprintw(11, 10, "CONTROLS:");
                mvprintw(12, 10, "- Arrow Keys or WASD to move");
                mvprintw(13, 10, "- Eat food (*) to grow longer");
                mvprintw(14, 10, "- Don't hit walls or yourself!");
                mvprintw(15, 10, "- Press Q to quit anytime");
                mvprintw(16, 10, "- Press R to restart");
                
                mvprintw(18, 10, "GOAL: Get the highest score!");
                attroff(COLOR_PAIR(4));
                
                attron(COLOR_PAIR(5));
                mvprintw(21, 14, "Press any key to return to menu...");
                attroff(COLOR_PAIR(5));
                
                refresh();
                nodelay(stdscr, FALSE);
                getch();
                nodelay(stdscr, TRUE);
            }
            else if (selected == 2) {
                return 0;
            }
        }
        
        usleep(50000);
    }
}

int main() {
    int ch;
    int i, j;
    int new_x, new_y;
    
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK); 
    init_pair(2, COLOR_CYAN, COLOR_BLACK);    
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);  
    init_pair(4, COLOR_GREEN, COLOR_BLACK);   
    init_pair(5, COLOR_RED, COLOR_BLACK);     
    
    srand(time(NULL));

    if (!show_menu()) {
        endwin();
        return 0;
    }

    reset_game();
    
    while (1) {
        ch = getch();
        
        // Control (tidak bisa balik 180 derajat)
        if ((ch == KEY_UP || ch == 'w') && dy == 0) {
            dx = 0; dy = -1;
        }
        else if ((ch == KEY_DOWN || ch == 's') && dy == 0) {
            dx = 0; dy = 1;
        }
        else if ((ch == KEY_LEFT || ch == 'a') && dx == 0) {
            dx = -1; dy = 0;
        }
        else if ((ch == KEY_RIGHT || ch == 'd') && dx == 0) {
            dx = 1; dy = 0;
        }
        else if (ch == 'q') break;
        else if (ch == 'r') {
            reset_game();
        }
        else if (ch == 'm') {
            if (!show_menu()) {
                break;
            }
            reset_game();
        }
        
        // Update game logic hanya jika belum game over
        if (!game_over) {
            new_x = snake[0].x + dx;
            new_y = snake[0].y + dy;
            
            // Cek tabrakan dengan dinding
            if (is_wall(new_x, new_y)) {
                game_over = 1;
            }
            
            // Cek tabrakan dengan tubuh sendiri
            if (!game_over) {
                for (i = 1; i < snake_len; i++) {
                    if (snake[i].x == new_x && snake[i].y == new_y) {
                        game_over = 1;
                        break;
                    }
                }
            }
            
            // Jika tidak tabrakan, update snake
            if (!game_over) {
                // Geser tubuh
                for (i = snake_len - 1; i > 0; i--) {
                    snake[i] = snake[i - 1];
                }
                
                // Update kepala
                snake[0].x = new_x;
                snake[0].y = new_y;
                
                // Cek makan food
                if (snake[0].x == food.x && snake[0].y == food.y) {
                    score += 10;
                    if (snake_len < MAX_SNAKE) {
                        snake_len++;
                    }
                    spawn_food();
                }
            }
        }
        
        erase();
        
        // Draw border (cyan)
        attron(COLOR_PAIR(2));
        for (i = 0; i < 50; i++) {
            mvprintw(0, i, "#");
            mvprintw(20, i, "#");
        }
        for (i = 0; i <= 20; i++) {
            mvprintw(i, 0, "#");
            mvprintw(i, 49, "#");
        }
        attroff(COLOR_PAIR(2));
        
        attron(COLOR_PAIR(3));
        
        for (i = 3; i <= 5; i++) {
            for (j = 3; j <= 5; j++) {
                mvprintw(i, j, "#");
            }
        }
        for (i = 3; i <= 5; i++) {
            for (j = 44; j <= 46; j++) {
                mvprintw(i, j, "#");
            }
        }
        for (i = 15; i <= 17; i++) {
            for (j = 3; j <= 5; j++) {
                mvprintw(i, j, "#");
            }
        }
        for (i = 15; i <= 17; i++) {
            for (j = 44; j <= 46; j++) {
                mvprintw(i, j, "#");
            }
        }
        
        for (j = 8; j <= 15; j++) {
            mvprintw(10, j, "#");
        }
        for (j = 34; j <= 41; j++) {
            mvprintw(10, j, "#");
        }
        
        for (i = 8; i <= 9; i++) {
            for (j = 22; j <= 27; j++) {
                mvprintw(i, j, "#");
            }
        }
        attroff(COLOR_PAIR(3));
        
        // Draw food (red)
        attron(COLOR_PAIR(5));
        mvaddch(food.y, food.x, '*');
        attroff(COLOR_PAIR(5));
        
        // Draw snake body (green)
        attron(COLOR_PAIR(4));
        for (i = 1; i < snake_len; i++) {
            mvaddch(snake[i].y, snake[i].x, 'o');
        }
        attroff(COLOR_PAIR(4));
        
        // Draw snake head (pink)
        attron(COLOR_PAIR(1));
        mvaddch(snake[0].y, snake[0].x, '@');
        attroff(COLOR_PAIR(1));
        
        // Info
        mvprintw(22, 0, "Score: %d | Length: %d", score, snake_len);
        mvprintw(23, 0, "WASD/Arrows: Move | Q: Quit | R: Restart | M: Menu");        

        if (game_over) {
            attron(COLOR_PAIR(5) | A_BOLD);
            mvprintw(10, 16, "*** GAME OVER! ***");
            mvprintw(11, 13, "Final Score: %d", score);
            mvprintw(12, 11, "Press R to Restart");
            mvprintw(13, 13, "Press M for Menu");
            attroff(COLOR_PAIR(5) | A_BOLD);
        }
        
        refresh();
        usleep(90000);
    }
    
    endwin();
    return 0;
}