#include <curses.h>
#include <unistd.h>

int main() {
    int x = 10, y = 5;
    int ch;

    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    // === INIT WARNA ===
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK); // pink untuk snake
    init_pair(2, COLOR_CYAN, COLOR_BLACK);    // cyan untuk border
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);  // kuning untuk obstacle

    while (1) {
        ch = getch();

        // movement
        if (ch == KEY_UP || ch == 'w') y--;
        else if (ch == KEY_DOWN || ch == 's') y++;
        else if (ch == KEY_LEFT || ch == 'a') x--;
        else if (ch == KEY_RIGHT || ch == 'd') x++;
        else if (ch == 'q') break;

        clear();

        // === DRAW BORDER (CYAN) ===
        attron(COLOR_PAIR(2));
        // Top & Bottom border
        for (int i = 0; i < 50; i++) {
            mvprintw(0, i, "#");
            mvprintw(20, i, "#");
        }
        // Left & Right border
        for (int i = 0; i <= 20; i++) {
            mvprintw(i, 0, "#");
            mvprintw(i, 49, "#");
        }
        attroff(COLOR_PAIR(2));

        // === DRAW OBSTACLES (KUNING) ===
        attron(COLOR_PAIR(3));
        
        // Kotak kecil pojok kiri atas
        for (int i = 3; i <= 5; i++) {
            for (int j = 3; j <= 5; j++) {
                mvprintw(i, j, "#");
            }
        }
        
        // Kotak kecil pojok kanan atas
        for (int i = 3; i <= 5; i++) {
            for (int j = 44; j <= 46; j++) {
                mvprintw(i, j, "#");
            }
        }
        
        // Kotak kecil pojok kiri bawah
        for (int i = 15; i <= 17; i++) {
            for (int j = 3; j <= 5; j++) {
                mvprintw(i, j, "#");
            }
        }
        
        // Kotak kecil pojok kanan bawah
        for (int i = 15; i <= 17; i++) {
            for (int j = 44; j <= 46; j++) {
                mvprintw(i, j, "#");
            }
        }
        
        // Garis horizontal kiri tengah
        for (int j = 8; j <= 15; j++) {
            mvprintw(10, j, "#");
        }
        
        // Garis horizontal kanan tengah
        for (int j = 34; j <= 41; j++) {
            mvprintw(10, j, "#");
        }
        
        // Kotak di tengah
        for (int i = 8; i <= 9; i++) {
            for (int j = 22; j <= 27; j++) {
                mvprintw(i, j, "#");
            }
        }
        
        attroff(COLOR_PAIR(3));

        // === DRAW SNAKE (PINK) ===
        attron(COLOR_PAIR(1));
        mvprintw(y, x, "@");
        attroff(COLOR_PAIR(1));

        // Info di bawah arena
        mvprintw(22, 0, "Arrows/WASD: Move | Q: Quit");
        mvprintw(23, 0, "Position: (%d, %d)", x, y);

        refresh();
        usleep(80000);
    }

    endwin();
    return 0;
}